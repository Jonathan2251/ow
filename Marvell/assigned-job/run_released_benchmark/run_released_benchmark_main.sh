#!/bin/bash
# Program:
#	Compile benchmark with specific gcc toolchain version and test it.

# check number of parameters.
#if [ $# -eq 0 ]; then
#  echo 1>&2 "Usage: $0 [FORCE_TEST] [RELEASE_TEST] [DONT_CHECK_UPDATE]"
#  exit 127
#fi

#BOARD=AXP
BOARD=BG2

CUR_PATH=`pwd` # get current path

if test "$BOARD" = "BG2"; then
  FILEINFO="4.8.4-20150109_BG2_1Ghz-1GMem_opt"
else
#  FILEINFO="4.6.4-20140402_AXP_1.3Ghz-2GMem_opt"
  FILEINFO="4.6.4-20141016_AXP_1.3Ghz-2GMem_opt"
fi
#TOOLCHAIN_DIR=armv7-marvell-linux-gnueabi-hard_x86_64 # for Marvell_4.6.4_toolchain_20130716
#TOOLCHAIN_DIR=armv7-marvell-linux-gnueabi-hard-4.6.4_x86_64_Dev # for test_build_4.6.4_20131213
TOOLCHAIN_DIR= # for 4.8.3_0715, 4.6.4_1016
TOOLCHAIN_DOCUMENT_DIR=${TOOLCHAIN_DIR}/marvell-doc # 4.6.4, 4.8.4
#TOOLCHAIN_DOCUMENT_DIR=${TOOLCHAIN_DIR}/document/marvell-doc # 4.8.3
#TOOLCHAIN_CMD=arm-marvell-linux-gnueabi-gcc # for 4.6.4
TOOLCHAIN_CMD=arm-marvell-linux-gnueabihf-gcc # for 4.8.3 after

source ~/marvell/work/assigned-job/auto-build-and-run-benchmark/run_benchmark_decl.sh

#LOGDIR=${CUR_PATH}/log/board-AXP-1.3Ghzx4-2GBMem/4.6.4_opt_20140402/${DATE}
LOGDIR=${CUR_PATH}/log/board-BG2-1Ghz-1GBMem/4.8.4_opt_20150109/${DATE}

backup_released_log() {
  rm -rf ${LOGDIR}
  mkdir -p ${LOGDIR} # force create dir and sub-dir.
  cp -f benchmark_run_${FILEINFO}_${DATE}.log ${LOGDIR}/benchmark_run_${FILEINFO}_${DATE}.log
  cp -f benchmark_run_${FILEINFO}_${DATE}.csv ${LOGDIR}/benchmark_run_${FILEINFO}_${DATE}.csv
  cp -f benchmark_run_multi_${FILEINFO}_${DATE}.log ${LOGDIR}/benchmark_run_multi_${FILEINFO}_${DATE}.log
  cp -f benchmark_run_multi_${FILEINFO}_${DATE}.csv ${LOGDIR}/benchmark_run_multi_${FILEINFO}_${DATE}.csv
  cp -f toolchain_version_${FILEINFO}_${DATE}.log ${LOGDIR}/toolchain_version_${FILEINFO}_${DATE}.log
  cp -f VERSION-INFO_${FILEINFO}_${DATE}.log ${LOGDIR}/VERSION-INFO_${FILEINFO}_${DATE}.log
  cp -rf ${MVL_BENCHMARK}/Global_Config.mak ${LOGDIR}/Global_Config.mak
}

#============== main loop ==============#

set -e 
set -x

if [ -f "$RUNNING" ]; then
  echo "+++ AutoTest is still running!! +++"
  if [ "$SENDMAIL" == "1" ] ; then
	  echo "--- send mail ---";
    send_mail FAIL > ${LOGFILE_SENDMAIL} 2>&1
  fi
  exit 1;
else
  echo "+++ Starting AutoTest!! +++"
  echo "${DATETIME}" > $RUNNING
fi  

#rev_no=1

echo "=== build_marvell_benchmark beginning!! ==="
build_marvell_benchmark;
# dont check_cpu_idle;
echo "=== run_benchmark beginning!! ==="
run_and_get_benchmark;
echo "=== convert_benchmark_log_to_csv beginning!! ==="
convert_benchmark_log_to_csv;
echo "=== convert_benchmark_log_to_csv end!! ==="
log_toolchain_version;
echo "=== log_toolchain_version end!! ==="
backup_released_log;
echo "=== backup_released_log end!! ==="

rm $RUNNING
echo "=== Done!! ==="
trap - EXIT
#============== end: main loop ==============#
