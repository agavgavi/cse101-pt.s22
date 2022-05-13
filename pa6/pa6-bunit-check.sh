#!/usr/bin/bash

SRCDIR=https://raw.githubusercontent.com/agavgavi/cse101-pt.s22/master/pa6
TIME=8

if [ ! -e backup ]; then
   echo "WARNING: a backup has been created for you in the \"backup\" folder"
   mkdir backup
fi


cp *.cpp *.h Makefile backup   # copy all files of importance into backup

curl $SRCDIR/ModelBigIntegerTest.cpp > ModelBigIntegerTest.cpp

echo ""
echo ""

echo "Press Enter To Continue with ModelBigIntegerTest Results"
read verbose

echo ""
echo ""

g++ -std=c++17 -Wall -c -g ModelBigIntegerTest.cpp BigInteger.cpp List.cpp -lm
g++ -std=c++17 -Wall -o ModelBigIntegerTest ModelBigIntegerTest.o BigInteger.o List.o -lm

timeout $TIME valgrind --leak-check=full -v ./ModelBigIntegerTest -v > BITest-out.txt

cat BITest-out.txt

rm -f *.o BITest-out.txt ModelBigIntegerTest ModelBigIntegerTest.cpp
