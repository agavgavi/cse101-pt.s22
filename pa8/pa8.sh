#!/bin/bash
# cse101-pt.s22
# usage: pa8.sh
# (run within your pa8 directory to test your code)

SRCDIR=https://raw.githubusercontent.com/agavgavi/cse101-pt.s22/master/pa8

EXE_ALL=( pa8-func-check.sh pa8-func2-check.sh pa8-unit-check.sh pa8-make-check.sh )
EXE_RANGE=$((${#EXE_ALL[*]} - 1))
echo $EXE_RANGE
for i in $(seq 0 $EXE_RANGE); do
  curl $SRCDIR/${EXE_ALL[i]} > ${EXE_ALL[i]}
  chmod +x ${EXE_ALL[i]}
  ./${EXE_ALL[i]}
  rm -f ${EXE_ALL[i]}
done
