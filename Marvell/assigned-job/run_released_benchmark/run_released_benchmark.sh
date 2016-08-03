#!/bin/bash
SENDMAIL=1

bash run_released_benchmark_main.sh
RESULT=$?

echo "result =" ${RESULT}
#= send mail and attach log file
if [ "$SENDMAIL" == "1" ] ; then
  echo "--- send mail ---";
  bash ~/marvell/work/assigned-job/auto-build-and-run-benchmark/send_mail.sh ${RESULT}
fi

