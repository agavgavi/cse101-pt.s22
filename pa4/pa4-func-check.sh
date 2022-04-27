#!/usr/bin/bash

SRCDIR=https://raw.githubusercontent.com/agavgavi/cse101-pt.s22/master/pa4
NUMTESTS=5
PNTSPERTEST=2
let MAXPTS=$NUMTESTS*$PNTSPERTEST
TIME=5
RED='\033[0;31m'
NC='\033[0m' # No Color
if [ ! -e backup ]; then
  mkdir backup
fi

cp *.c *.h Makefile backup   # copy all files of importance into backup

for NUM in $(seq 1 $NUMTESTS); do
  curl $SRCDIR/infile$NUM.txt > infile$NUM.txt
  curl $SRCDIR/model-outfile$NUM.txt > model-outfile$NUM.txt
  rm -f outfile$NUM.txt
done


rm -f *.o Sparse

gcc -c -Wall -std=c17 -g Sparse.c Matrix.c List.c
gcc -o Sparse Sparse.o Matrix.o List.o


echo ""
echo ""

pathtestspassed=$(expr 0)
echo "Please be warned that the following tests discard all output to stdout/print stderr separately"
echo "Sparse tests: If nothing between '=' signs, then test is passed"
echo "Press enter to continue"
read verbose
for NUM in $(seq 1 $NUMTESTS); do
  rm -f outfile$NUM.txt
  let RUNTIME=$TIME
  if [ $NUM -eq 4 ]; then
    let RUNTIME=3*$TIME
  fi
  timeout $RUNTIME ./Sparse infile$NUM.txt outfile$NUM.txt &> garbage >> garbage #all stdout/stderr thrown away
  if [ $? -eq 124 ]; then
    echo -e "${RED} SPARSE TEST TIMED OUT ${NC}"
  fi
  diff -bBwu outfile$NUM.txt model-outfile$NUM.txt > diff$NUM.txt &>> diff$NUM.txt
  echo "Sparse Test $NUM:"
  echo "=========="
  cat diff$NUM.txt
  echo "=========="
  if [ -e diff$NUM.txt ] && [[ ! -s diff$NUM.txt ]]; then # increment number of tests passed counter
    let pathtestspassed+=1
  fi
done

echo ""
echo ""

let pathtestspoints=$PNTSPERTEST*$pathtestspassed
if [ "$pathtestspoints" -gt "$MAXPTS" ]; then # max 10 points
  let pathtestspoints=$(expr $MAXPTS)
fi
echo "Passed $pathtestspassed Sparse tests for a total of $pathtestspoints / $MAXPTS points"

echo ""
echo ""
timeout $TIME valgrind --leak-check=full -v ./Sparse infile2.txt outfile2.txt > garbage 2> Sparse-mem.txt

cat Sparse-mem.txt

echo ""
echo ""
rm -f *.o Sparse garbage Sparse-mem.txt

