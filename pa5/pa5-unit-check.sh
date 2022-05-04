#!/usr/bin/bash

SRCDIR=https://raw.githubusercontent.com/agavgavi/cse101-pt.s22/master/pa5

if [ ! -e backup ]; then
   echo "WARNING: a backup has been created for you in the \"backup\" folder"
   mkdir backup
fi


cp *.cpp *.h Makefile README* backup   # copy all files of importance into backup

curl $SRCDIR/ModelListTest.cpp > ModelListTest.cpp

echo ""
echo ""

echo "Press Enter To Continue with List Unit Test"
read verbose

echo ""
echo ""

g++ -std=c++17 -Wall -c -g ModelListTest.cpp List.cpp
g++ -std=c++17 -Wall -o ModelListTest ModelListTest.o List.o

timeout 5 valgrind --leak-check=full -v ./ModelListTest -v > ListTest-out.txt 2> ListTest-mem.txt
if [ $? -eq 124 ]; then
   echo -e "${RED} MODEL LIST TEST TIMED OUT ${NC}"
fi

cat ListTest-out.txt
echo
echo "Press Enter for Memory Leaks Report:"
read verbose
cat ListTest-mem.txt

rm -f *.o ModelListTest ListTest-out.txt ModelListTest.cpp

