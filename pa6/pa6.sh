#!/bin/bash
# cse101-pt.s22 grading
# usage: pa6.sh
# (run within your pa6 directory to test your code)

SRCDIR=https://raw.githubusercontent.com/agavgavi/cse101-pt.s22/master/pa6

EXE_ALL=( pa6-func-check.sh pa6-lunit-check.sh pa6-bunit-check.sh pa6-make-check.sh )
EXE_RANGE=$((${#EXE_ALL[*]} - 1))
echo $EXE_RANGE
for i in $(seq 0 $EXE_RANGE); do
  curl $SRCDIR/${EXE_ALL[i]} > ${EXE_ALL[i]}
  chmod +x ${EXE_ALL[i]}
  ./${EXE_ALL[i]}
  rm -f ${EXE_ALL[i]}
done
