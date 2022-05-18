#!/usr/bin/bash
RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m' # No Color

if [ ! -e backup ]; then
  mkdir backup
fi

cp *.cpp *.h Makefile backup   # copy all files of importance into backup


echo ""
echo ""

rm -f *.o Arithmetic

make

if [ ! -e Arithmetic ] || [ ! -x Arithmetic ]; then # exist and executable
  echo ""
  echo -e "${RED}Makefile probably doesn't correctly create Executable!!!${NC}"
  echo ""
else
  echo ""
  echo -e "${GREEN}Makefile probably correctly creates Executable!${NC}"
  echo ""
fi

echo ""
echo ""

make clean

if [ -e Arithmetic ] || [ -e *.o ]; then
    echo -e "${RED}WARNING: Makefile didn't successfully clean all files${NC}"
fi

case `grep -Ei '(power[[:blank:]]+9|p[[:blank:]]+9|power[[:blank:]]*=[[:blank:]]*9|p[[:blank:]]*=[[:blank:]]*9)' BigInteger.* > /dev/null; echo $?` in
  0)
    ;;
  1)
    echo -e "${RED}WARNING: POWER/P is not set to 9 within BigInteger.h/.cpp.\n-10 points${NC}"
    ;;
  *)
    ;;
esac

rm -f *.o Arithmetic garbage
