#!/bin/bash
SENDMAIL=1

bash ~/marvell/work/assigned-job/auto-build-and-run-benchmark/run_autotest_daily_main.sh $1
RESULT=$?

echo "result =" ${RESULT}
#= send mail and attach log file
if [ "$SENDMAIL" == "1" ] ; then
  echo "--- send mail ---";
  bash ~/marvell/work/assigned-job/auto-build-and-run-benchmark/send_mail.sh ${RESULT}
fi

