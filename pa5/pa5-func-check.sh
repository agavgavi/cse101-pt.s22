#!/usr/bin/bash

SRCDIR=https://raw.githubusercontent.com/agavgavi/cse101-pt.s22/master/pa5
NUMTESTS=3
PNTSPERTEST=5
INPUTS=( 10 50 100 )
let MAXPTS=$NUMTESTS*$PNTSPERTEST

if [ ! -e backup ]; then
   echo "WARNING: a backup has been created for you in the \"backup\" folder"
   mkdir backup
fi


cp *.cpp *.h Makefile README* backup   # copy all files of importance into backup

for NUM in "${INPUTS[@]}"; do
   curl $SRCDIR/Model-out$NUM.txt > Model-out$NUM.txt
done

echo ""
echo ""

g++ -std=c++17 -Wall -c -g Shuffle.cpp List.cpp
g++ -std=c++17 -Wall -o Shuffle Shuffle.o List.o

lextestspassed=$(expr 0)
echo "Please be warned that the following tests discard all output to stdout while reserving stderr for valgrind output"
echo "Shuffle tests: If nothing between '=' signs, then test is passed"
echo "Press enter to continue"
read verbose
for NUM in "${INPUTS[@]}"; do
  rm -f out$NUM.txt
  timeout 5 valgrind --leak-check=full -v ./Shuffle $NUM > out$NUM.txt 2> valgrind-out$NUM.txt
  if [ $? -eq 124 ]; then
    echo -e "${RED} SHUFFLE TEST TIMED OUT ${NC}"
  fi
  diff -bBwu out$NUM.txt Model-out$NUM.txt &> diff$NUM.txt >> diff$NUM.txt
  echo "Shuffle Test $NUM: (Press enter to continue...)"
  read verbose
  echo "=========="
  cat diff$NUM.txt
  echo "=========="
  if [ -e diff$NUM.txt ] && [[ ! -s diff$NUM.txt ]]; then
    let lextestspassed+=1
  fi
done

let lextestpoints=${PNTSPERTEST}*lextestspassed

echo "Passed $lextestspassed / $NUMTESTS Shuffle tests"
echo "This gives a total of $lextestpoints / $MAXPTS points"
echo ""
echo ""

echo "Press Enter To Continue with Valgrind Results for Shuffle"
echo "The valgrind report will only show the number of leaks and errors. For a detail report, please negivate to the corresponding output valgrind-out#.txt"
#TODO find a way to automate detecting if leaks and errors are found and how many
read garbage

for NUM in "${INPUTS[@]}"; do
   echo "Shuffle Valgrind Test $NUM:"
   # read verbose
   echo "=========="
   cat valgrind-out$NUM.txt
   echo "=========="
done

echo ""
echo ""
rm -f *.o Shuffle

