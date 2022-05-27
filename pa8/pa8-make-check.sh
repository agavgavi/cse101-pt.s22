#!/usr/bin/bash
RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m' # No Color

echo ""
echo ""
echo "press Enter for make tests"
read garbage

make

if [ ! -e Order ] || [ ! -x Order ]; then # exist and executable
  echo ""
  echo -e "${RED}Makefile probably doesn't correctly create Executable!!!${NC}"
  echo ""
else
  echo ""
  echo -e "${GREEN}Makefile probably correctly creates Executable!${NC}"
  echo ""
fi

make clean

echo ""
echo ""

if [ -e Order ] || [ -e *.o ]; then
   echo -e "${RED}WARNING: Makefile didn't successfully clean all files${NC}"
fi