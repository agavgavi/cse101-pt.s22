#!/usr/bin/bash
TIME=8
RED='\033[0;31m'
NC='\033[0m' # No Color
SRCDIR=https://raw.githubusercontent.com/agavgavi/s22/master/pa4

if [ ! -e backup ]; then
  mkdir backup
fi

cp *.c *.h Makefile backup   # copy all files of importance into backup

curl $SRCDIR/ModelMatrixTest.c > ModelMatrixTest.c

echo ""
echo ""

rm -f *.o Sparse ModelMatrixTest

echo "Press Enter To Continue with MatrixTest Results"
read verbose

echo ""
echo ""

gcc -c -std=c17 -Wall -g ModelMatrixTest.c Matrix.c List.c
gcc -o ModelMatrixTest ModelMatrixTest.o Matrix.o List.o

timeout 20 /usr/bin/time -po time.txt valgrind --leak-check=full -v ./ModelMatrixTest -v > MatrixTest-out.txt 2> MatrixTest-mem.txt
t=$?
userTime=`perl -ane 'print $F[1] if $F[0] eq "user"' time.txt`
tooSlow=$(echo "$userTime > $TIME" |bc -l)
cat MatrixTest-mem.txt
cat MatrixTest-out.txt
if [ $t -eq 124 ] || [ $tooSlow -eq 1 ]; then
  echo -e "${RED} MATRIX MODEL TEST TIMED OUT ${NC}"
fi

rm -f *.o ModelMatrixTest* garbage MatrixTest-mem.txt MatrixTest-out.txt

