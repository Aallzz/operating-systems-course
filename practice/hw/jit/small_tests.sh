#!/bin/bash
make all
touch file.txt

GREEN='\033[0;31m'
RED='\033[0;30m'
NC='\033[0m'

declare -a INS=(  "2.1 3 +"  "x x *"    "2.1 3 x + -"   "2 x x x * + * 4 x * + 2 x * / 3 x * -")
declare -a X=(    "1.4"      "1.5"      "1.2"           "3.74")
declare -a OUTS=( "5.10"     "2.25"     "-2.10"         "-4.48")

arraylength=${#INS[@]}

for (( i=1; i<${arraylength}+1; i++ )); do
  echo "${INS[$i-1]}" > file.txt
  echo "test: " "${INS[$i-1]}"
  ans=$(./my_jit file.txt ${X[$i-1]})
  if [ $ans = ${OUTS[$i-1]} ]; then
    echo -e "--verdict: ${GREEN}OK${NC}"
  else
    echo -e "--verdict: ${RED}WA${NC}"
    echo "expected: ${OUTS[$i-1]}"
    echo "found: $ans"
  fi
done


rm file.txt
