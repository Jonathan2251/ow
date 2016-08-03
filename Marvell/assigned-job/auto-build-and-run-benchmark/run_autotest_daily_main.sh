#!/bin/bash
# Program:
#	Gcc regression daily test.
# History:
# 2010/05/11	J.H.	First release

# check number of parameters.
#if [ $# -eq 0 ]; then
#  echo 1>&2 "Usage: $0 [FORCE_TEST] [RELEASE_TEST] [DONT_CHECK_UPDATE]"
#  exit 127
#fi

#BOARD=AXP
BOARD=BG2

#CUR_PATH=`pwd` # get current path    !!! don't use `pwd` since this script is called by crontab which meaning run in dir/home/cschen.
CUR_PATH=~/marvell/work/assigned-job/auto-build-and-run-benchmark # get current path
FILEINFO="4.8.4-2015_BG2_1Ghz-1GMem_opt"
TOOLCHAIN_DIR=Release/install/armv7-marvell-linux-gnueabihf-hard_i686_Dev
TOOLCHAIN_DOCUMENT_DIR=${TOOLCHAIN_DIR}/document/marvell-doc
TOOLCHAIN_CMD=arm-marvell-linux-gnueabihf-gcc # for 4.8

source ~/marvell/work/assigned-job/auto-build-and-run-benchmark/run_benchmark_decl.sh

#============== main loop ==============#

set -e 
set -x

#if [ -f "$RUNNING" ]; then
#  echo "+++ AutoTest is still running!! +++"
#  if [ "$SENDMAIL" == "1" ] ; then
#	  echo "--- send mail ---";
#    send_mail FAIL > ${LOGFILE_SENDMAIL} 2>&1
#  fi
#  exit 1;
#else
  echo "+++ Starting AutoTest!! +++"
  echo "${DATETIME}" > $RUNNING
  echo "PID = $$" > $RUNNING
  echo "${DATETIME}"
  echo "PID = $$"
#fi  

#rev_no=1

#= remove MVL_TOOLCHAIN dir to force to do full download and full build
#rm -rf ${MVL_TOOLCHAIN} || true
prolog;

#= check update of source code: if no, exit directory.
check_update;
if [ "$IsUpdate" -eq "0" ]; then
  echo "=== Src No Updated ==="
  if [ "$FORCE_TEST" != "1" ]; then
    rm $RUNNING
    exit 0;
  fi
else
  echo "=== Src Updated ==="
fi  

echo "=== build_marvell_toolchain beginning!! ==="
build_marvell_gcc_toolchain;
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
backup_log;
echo "=== backup_log end!! ==="

#= generate url for result compare
#gen_result_cmp_url;

rm $RUNNING
echo "=== Done!! ==="
trap - EXIT
#============== end: main loop ==============#
