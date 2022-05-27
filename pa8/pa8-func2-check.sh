#!/usr/bin/bash
RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m' # No Color

SRCDIR=https://raw.githubusercontent.com/agavgavi/cse101-pt.s22/master/pa8
NUMTESTS=3
PNTSPERTEST=3
MAXTIME=10
let MAXPTS=$NUMTESTS*$PNTSPERTEST

if [ ! -e backup ]; then
   echo "WARNING: a backup has been created for you in the \"backup\" folder"
   mkdir backup
fi


cp *.cpp *.h Makefile backup   # copy all files of importance into backup

for NUM in $(seq 1 $NUMTESTS); do
   curl $SRCDIR/WF-infile$NUM.txt > WF-infile$NUM.txt
   curl $SRCDIR/Model-WF-outfile$NUM.txt > Model-WF-outfile$NUM.txt
done

echo ""
echo ""

g++ -std=c++17 -Wall -c -g WordFrequency.cpp Dictionary.cpp
g++ -std=c++17 -Wall -o WordFrequency WordFrequency.o Dictionary.o

lextestspassed=$(expr 0)
echo "WordFrequency tests: If nothing between '=' signs, then test is passed"
echo "Press enter to continue"
read verbose
for NUM in $(seq 1 $NUMTESTS); do
  rm -f WF-outfile$NUM.txt
  echo "WordFrequency Test $NUM:"
  timeout 20 /usr/bin/time -po WF-time$NUM.txt ./WordFrequency WF-infile$NUM.txt WF-outfile$NUM.txt
  t=$?
  userTime=`perl -ane 'print $F[1] if $F[0] eq "user"' WF-time$NUM.txt`
  tooSlow=$(echo "$userTime > $MAXTIME" |bc -l)
  diff -bBwu --speed-large-files WF-outfile$NUM.txt Model-WF-outfile$NUM.txt &> WF-diff$NUM.txt >> WF-diff$NUM.txt
  echo "=========="
  cat WF-diff$NUM.txt
  if [[ $tooSlow -eq 1 ]] || [[ $t -eq 124 ]]; then
    echo -e "${RED}ORDER TEST TIMED OUT (Slower than $MAXTIME) ${NC}"
  fi
  echo "=========="
  if [ -e WF-diff$NUM.txt ] && [[ ! -s WF-diff$NUM.txt ]] && [[ $tooSlow -eq 0 ]] && [[ ! $t -eq 124 ]]; then
    let lextestspassed+=1
  fi
done

let lextestpoints=${PNTSPERTEST}*lextestspassed

echo "Passed $lextestspassed / $NUMTESTS WordFrequency tests"
echo "This gives a total of $lextestpoints / $MAXPTS points"
echo ""
echo ""

rm -f *.o WordFrequency WF-diff*
