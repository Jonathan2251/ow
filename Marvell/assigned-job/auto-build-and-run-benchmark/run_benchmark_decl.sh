
if [ $BOARD == AXP ]; then
IPADDR=10.19.132.50 # AXP
SSH_PORT="-p 8008"
SCP_OPT="-P 8008"
else # BG2
IPADDR=10.19.132.185 # BG2
SSH_PORT=
fi

# Without yes prompt after board reboot
SSH_OPT="StrictHostKeyChecking no"

SENDMAIL=1
# if FORCE_TEST == 1, do test even if src no updated
FORCE_TEST=$1
# if RELEASE_TEST == 1, download src from south(release test), else from s03(developing)
RELEASE_TEST=$2
# if DONT_CHECK_UPDATE == 1, don't check src updated
DONT_CHECK_UPDATE=$3

DATE=`date +%Y%m%d`
TIME=`date +%H%M%S`
DATETIME=${DATE}.${TIME}
THIS_SH_LOCATION=~/marvell/work/assigned-job/auto-build-and-run-benchmark
S02LOGDIR=${THIS_SH_LOCATION}/log/board-BG2-1Ghz-1GBMem/4.8.4-2015_opt/${DATE}

export DEJA_CROSSTEST_ROOT="/home/tester1/deja_crosstest"
export QEMU_SCRIPT_ROOT="/home/jwhsu/work/qemu/gcctest"

#LOGFILE=${CUR_PATH}/log/autotest.${DATETIME}.log
LOGFILE_SENDMAIL=${CUR_PATH}/log/autotest.${DATETIME}-sendmail.log
mkdir -p ${CUR_PATH}/log  # make log dir for $LOGFILE_SENDMAIL
rm -f ${CUR_PATH}/log/autotest*.log
RUNNING=${CUR_PATH}/running

SUCCESS=0
FAIL=1

#= convert stdout & stderr to ${LOGFILE}
#exec >& ${LOGFILE}
#echo "log file: ${LOGFILE}"

set -x

HOSTNM=`hostname`
if test "$HOSTNM" = "s04"; then
  LENNY_ROOT="lenny_root/build_gcc"
  BUILD_TARGET="build_target_lenny"
else
  LENNY_ROOT=""
  BUILD_TARGET="build_target"
fi

MVL_TOOLCHAIN="${CUR_PATH}/mvl-toolchain"
MVL_BENCHMARK="${CUR_PATH}/build-marvell-benchmark"

FULL_BUILD=0
QUICK_BUILD=0
IsUpdate=0

TARGET_LIST_FILE="${CUR_PATH}/test_target.list"
target_list=
target_list_done=
target_list_num=0
target_list_idx=0
current_target=""

result_cmp_arr=
result_cmp_num=0

#============== function declaration ==============#

prolog()
{
  ssh ${SSH_PORT} -o "${SSH_OPT}" root@${IPADDR} "if [ -d cschen/run ] ; then echo \"cschen/run exists\"; else mkdir -p cschen/run; fi"
}

print_error()
{
  echo "Some errors occur, Please see log files!"
  rm $RUNNING
  ### send mail and attach log file =#
  if [ "$SENDMAIL" == "1" ] ; then
    echo "--- send mail ---";
    send_mail FAIL > ${LOGFILE_SENDMAIL} 2>&1
  fi
  exit 1
}

# function: git_pull_src
git_pull_src()
{
  pushd ./src
  dirs=$(ls -d *-src)
  for dir1 in $dirs ; do
    #if test -e $dir1 ; then
    if [ "zlib-src" != $dir1 ] ; then
      pushd $dir1
      git pull
      popd
    fi
  done
  popd
}

# function: check_update
#    To check if gcc source is updated.
# input:
# output: FULL_BUILD, QUICK_BUILD
# return: IsUpdate=0, no update; IsUpdate>0, updated; 
check_update() {
  echo "--- check update ---";

  if [ "$DONT_CHECK_UPDATE" == "1" ] ; then
    echo "=== skip check_update ===";
    FULL_BUILD=0
    QUICK_BUILD=0
    IsUpdate=0;
    return;
  fi
	
  #FULL_BUILD=0
  #QUICK_BUILD=0
  IsUpdate=0;
	
  #rm -rf ${MVL_TOOLCHAIN}
  if [ ! -d "${MVL_TOOLCHAIN}" ]; then
    echo "=== dir [${MVL_TOOLCHAIN}] Don't exist!! ==="
    mkdir ${MVL_TOOLCHAIN}
    FULL_BUILD=1
    IsUpdate=1
  #else
  #  echo " dir [${MVL_TOOLCHAIN}] exist!!"
  fi
  pushd ${MVL_TOOLCHAIN}
  # To get scripts
  if [ ! -d "script" ]; then
    echo "=== script/ dir do NOT exist!!! ==="
    echo "******** Please Download First *********"
    if [ "$SENDMAIL" == "1" ] ; then
      echo "--- send mail ---";
      send_mail FAIL > ${LOGFILE_SENDMAIL} 2>&1
    fi
    exit 1;
  else
    echo "=== pull script ==="
    cd script/
    git pull > ../update-script.log
    cd -
  fi
  #echo;
  # To get source
  if [ ! -d "src" ]; then
    echo "=== src/ dir do NOT exist!!! ==="
    echo "******** Please Download First *********"
    if [ "$SENDMAIL" == "1" ] ; then
      echo "--- send mail ---";
      send_mail FAIL > ${LOGFILE_SENDMAIL} 2>&1
    fi
    exit 1;
  else
    echo "=== pull src ==="
    num1=$(ls -ld ./src/*-src | grep -v zlib-src | wc -l)
    echo "num1 : $num1"
    git_pull_src  > update-src.log
    num2=$(grep "Already up-to-date" update-src.log | wc -l)
    echo " # of Already up-to-date : $num2"
    /usr/local/bin/repo sync  > update-zlib-src.log 2>&1 #update zlib-src
    set -- `ls -l update-zlib-src.log`
    echo "variable \$5 : $5"
    if [ "$num1" != "$num2" ] || [ $5 -gt 1 ]; then
      echo " ===Src UPDATED==="
      QUICK_BUILD=1
      IsUpdate=1
      #save update log
      if [ ! -d "log" ]; then
        mkdir -p log
      fi
      cat update-src.log >> ./log/update-src.log.${DATETIME}
      cat update-zlib-src.log >> ./log/update-src.log.${DATETIME}
    else
      echo " ===Src NO UPDATED==="
    fi
  fi
  popd #${MVL_TOOLCHAIN}

  #IsUpdate=$[$FULL_BUILD + $QUICK_BUILD];
  #echo "--- [end] check update ---";
}

build_marvell_gcc_toolchain() {
  pushd ${MVL_TOOLCHAIN}
# Remove all directories in Release/install/, otherwise it will has two gcc version,
# (such as gcc4.8.3 and gcc4.8.4). Then it is possible to make symbolic link (ln) to wrong gcc 
# distribution at later. Leave only one gcc distribution (for instance gcc4.8.4), then no
# this issue.
  rm -rf Release/install/*
# for 4.6
#  make -f ./script/Makefile.marvell armv7-marvell-linux-gnueabi-hard VERBOSE=1 SHOw_LOG=1
# for 4.8
#  make -f ./script/Makefile.marvell armv7-marvell-linux-gnueabihf-hard VERBOSE=1 SHOw_LOG=1 FORCE_BUILD=1
  make armv7-marvell-linux-gnueabihf-hard VERBOSE=1 SHOw_LOG=1 FORCE_BUILD=1 EXTRA=1
  gccinstalldir=${MVL_TOOLCHAIN}/Release/install
  set -- "$gccinstalldir"/*
  echo "Directory: $gccinstalldir"
  echo "Number of files: $#"
  echo "List of files:"
  for gccdir do
    echo "  $gccdir"
  done
  ln -s $gccdir ${MVL_TOOLCHAIN}/Release/install/armv7-marvell-linux-gnueabihf-hard_i686_Dev

  popd
}

build_marvell_benchmark() {
  pushd ${MVL_BENCHMARK}
  rm -rf output
  make -f build/Makefile cleanall
  make -f build/Makefile all_bm VERBOSE=1 SHOW_LOG=1
  popd
}

check_cpu_idle() {
  local CPUIDLE=0%
  local cpu_idle=0
  pushd ${MVL_BENCHMARK}
  ssh ${SSH_PORT} -o "${SSH_OPT}" root@${IPADDR} "top -n 1 | grep "CPU:" > aa"
  scp ${SCP_OPT} root@${IPADDR}:/root/aa aa
  cat aa | awk '{print $8}' > aa1
  while read CPUIDLE < aa1; do
    if [ ${CPUIDLE} == "99%" -o ${CPUIDLE} == "100%" ]; then
#     if [ ${CPUIDLE} == "100%" ]; then
      let cpu_idle=1
    else
      let cpu_idle=0
    fi
    break
  done 
  echo "CPUIDLE = $CPUIDLE"
  echo "cpu_idle = $cpu_idle"
  if [ cpu_idle == 0 ]; then
    echo "CPU not 100% idle"
    rm $RUNNING
    if [ "$SENDMAIL" == "1" ] ; then
      echo "--- send mail ---";
      send_mail FAIL > ${LOGFILE_SENDMAIL} 2>&1
    fi
    exit 1
  fi
  popd
}

copy_benchmark_to_board() {
  pushd ${MVL_BENCHMARK}
  ssh ${SSH_PORT} -o "${SSH_OPT}" root@${IPADDR} "rm -rf cschen/run/*"
  scp -r ${SCP_OPT} output/* root@${IPADDR}:/root/cschen/run
  popd
}

run_single_benchmark() {
  pushd ${MVL_BENCHMARK}
  ssh ${SSH_PORT} -o "${SSH_OPT}" root@${IPADDR} 'bash -l' < ${THIS_SH_LOCATION}/invoke_0run.sh
  popd
}

get_single_benchmark_log() {
  scp ${SCP_OPT} root@${IPADDR}:/root/cschen/run/benchmark_run.log benchmark_run_${FILEINFO}_${DATE}.log
  if [ ! -f benchmark_run_${FILEINFO}_${DATE}.log ]; then
    echo "=== get_benchmark_log() fail!! (benchmark_run_${FILEINFO}_${DATE}.log not exists) ==="
    rm $RUNNING
    if [ "$SENDMAIL" == "1" ] ; then
      echo "--- send mail ---";
      send_mail FAIL > ${LOGFILE_SENDMAIL} 2>&1
    fi
    exit 1
  fi
}

run_multi_benchmark() {
  pushd ${MVL_BENCHMARK}
  ssh ${SSH_PORT} -o "${SSH_OPT}" root@${IPADDR} 'bash -l' < ${THIS_SH_LOCATION}/invoke_0run-multi.sh
  popd
}

get_multi_benchmark_log() {
  scp ${SCP_OPT} root@${IPADDR}:/root/cschen/run/benchmark_run_multi.log benchmark_run_multi_${FILEINFO}_${DATE}.log
  if [ ! -f benchmark_run_multi_${FILEINFO}_${DATE}.log ]; then
    echo "=== get_benchmark_multi_log() fail!! (benchmark_run_multi_${FILEINFO}_${DATE}.log not exists) ==="
    rm $RUNNING
    if [ "$SENDMAIL" == "1" ] ; then
      echo "--- send mail ---";
      send_mail FAIL > ${LOGFILE_SENDMAIL} 2>&1
    fi
    exit 1
  fi
}

run_and_get_benchmark() {
  copy_benchmark_to_board;
  run_single_benchmark;
  get_single_benchmark_log;
  run_multi_benchmark;
  get_multi_benchmark_log;
}

log_toolchain_version() {
  ${MVL_TOOLCHAIN}/${TOOLCHAIN_DIR}/bin/${TOOLCHAIN_CMD} -v 2> toolchain_version_${FILEINFO}_${DATE}.log
  cp ${MVL_TOOLCHAIN}/${TOOLCHAIN_DOCUMENT_DIR}/VERSION-INFO VERSION-INFO_${FILEINFO}_${DATE}.log
}

convert_benchmark_log_to_csv() {
  cp -f benchmark_run_${FILEINFO}_${DATE}.log benchmark_run_${FILEINFO}_${DATE}.csv
  bash ${THIS_SH_LOCATION}/format-bm-log.sh benchmark_run_${FILEINFO}_${DATE}.csv
  cp -f benchmark_run_multi_${FILEINFO}_${DATE}.log benchmark_run_multi_${FILEINFO}_${DATE}.csv
  bash ${THIS_SH_LOCATION}/format-bm-log.sh benchmark_run_multi_${FILEINFO}_${DATE}.csv
}

backup_log() {
  rm -rf ${S02LOGDIR}
  mkdir ${S02LOGDIR}
  cp -f benchmark_run_${FILEINFO}_${DATE}.log ${S02LOGDIR}/benchmark_run_${FILEINFO}_${DATE}.log
  cp -f benchmark_run_${FILEINFO}_${DATE}.csv ${S02LOGDIR}/benchmark_run_${FILEINFO}_${DATE}.csv
  cp -f benchmark_run_multi_${FILEINFO}_${DATE}.log ${S02LOGDIR}/benchmark_run_multi_${FILEINFO}_${DATE}.log
  cp -f benchmark_run_multi_${FILEINFO}_${DATE}.csv ${S02LOGDIR}/benchmark_run_multi_${FILEINFO}_${DATE}.csv
  cp -f toolchain_version_${FILEINFO}_${DATE}.log ${S02LOGDIR}/toolchain_version_${FILEINFO}_${DATE}.log
  cp -f VERSION-INFO_${FILEINFO}_${DATE}.log ${S02LOGDIR}/VERSION-INFO_${FILEINFO}_${DATE}.log
  cp -rf ${MVL_BENCHMARK}/Global_Config.mak ${S02LOGDIR}/Global_Config.mak
  rm -rf ${DATE}
  mkdir ${DATE}
  cp -f benchmark_run_${FILEINFO}_${DATE}.log ${DATE}/benchmark_run.log
  cp -f benchmark_run_multi_${FILEINFO}_${DATE}.log ${DATE}/benchmark_run_multi.log
  cp -f toolchain_version_${FILEINFO}_${DATE}.log ${DATE}/toolchain_version
  cp -f ${MVL_TOOLCHAIN}/${TOOLCHAIN_DIR}/document/marvell-doc/VERSION-INFO ${DATE}/.
  cp -rf ${MVL_BENCHMARK}/Global_Config.mak ${DATE}/.
  scp -r ${SCP_OPT} ${DATE} cschen@10.19.132.33:/work/share/test_results/benchmark/
  scp ${SCP_OPT} cschen@10.19.132.33:/work/share/test_results/benchmark/board_info cschen@10.19.132.33:/work/share/test_results/benchmark/${DATE}/board_info
}

# function: gen_result_cmp_url
#    generate url for result compare.
# input: result_cmp, result_cmp_baseline
# output: result_cmp_url
gen_result_cmp_url() {
  echo "--- gen_result_cmp_url ---";
  if [ -f result_cmp_url ]; then
    rm result_cmp_url
  fi
  #= remove space and tab
  sed 's/[ \t]//g' result_cmp > ttt; cat ttt > result_cmp;
  sed 's/[ \t]//g' result_cmp_baseline > ttt; cat ttt > result_cmp_baseline;
  rm ttt;
  _IFS=$IFS #backup original IFS
  IFS=$'\n'; #change IFS
  result_cmp_arr=(`cat "./result_cmp"`);
  #result_cmp_arr=(`sed 's/[ \t]//g' result_cmp`);
  #result_cmp_arr=(`cut -d'/' -f1- --output-delimiter=' ' ./result_cmp`);
  #echo "result_cmp_arr=${result_cmp_arr[*]}";
  IFS=$_IFS #restore IFS
  result_cmp_num=${#result_cmp_arr[*]};
  printf '# of result_cmp_arr = %d\n' "$result_cmp_num";
  #local i=0;for tg in ${result_cmp_arr[*]}; do echo "result_cmp_arr[$i]=$tg"; i=$[i + 1]; done
  local n=0;
  while [ "$n" -lt "$result_cmp_num" ]
  do
    #echo "$n : ${result_cmp_arr[$n]}"
    #cur_item=`echo ${result_cmp_arr[$n]} | cut -d'/' -f1- --output-delimiter=' '`;
    grep_patern=`echo ${result_cmp_arr[$n]} | cut -d'/' -f8-12 `;
    #echo $grep_patern;
    #for tg in $cur_item
    #do
    #	if [ "$tg" = "v5" -o "$tg" = "v7" ]; then
    #		echo "$tg"
    #	fi 
    #done
    baseline_item=`grep -e $grep_patern ./result_cmp_baseline`
    #echo $baseline_item
    echo "http://10.19.133.162/tester1/gcc_regression/web/GccCompare1.php?VER[]=${baseline_item}&VER[]=${result_cmp_arr[$n]}&GccGpp=Gcc" >> result_cmp_url
    n=$(( $n + 1 )) 
  done
}

# function: send_mail
#    send notice mail and attach log file.
# input: $1:SUCCESS|FAIL
# output: 
# return: 
send_mail() {
  set -x
  echo "====== send mail and attach log file ======"
  MAILER="${THIS_SH_LOCATION}/PHPMailer/sendmail-autotest.sh"
  #tar czvf loginfo.tar.gz ${LOGFILE} start-qemu.log stdout.log stderr.log issuetest.log temporal.log || true
  tar czvf loginfo.tar.gz start-qemu.log stdout.log stderr.log issuetest.log temporal.log result_cmp_url || true
  #EMAIL="jwhsu@marvell.com;yhwang@marvell.com;ahsu@marvell.com"
  EMAIL="cschen@marvell.com"
  if [ $1 == ${SUCCESS} ]; then
    RESULTS="SUCCESS"
  else
    RESULTS="FAIL"
  fi
  #ATTACH="${THIS_SH_LOCATION}/loginfo.tar.gz"
  ATTACH=
  RESULT_CMP_URL="${THIS_SH_LOCATION}/result_cmp_url"
  $MAILER MAILTO="$EMAIL" ATTACH=$ATTACH RESULT=${RESULTS} DATETIME=$DATETIME GCCVERS=$GCCVERS RELEASE=$RELEASE RESULT_CMP_URL=$RESULT_CMP_URL
  set +x
}

#============== end: function declaration ==============#
