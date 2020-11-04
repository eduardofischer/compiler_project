#!/bin/bash

while IFS=, read -r fname code
do
  ./etapa4 < tests/E4/$fname > tests/E4/output/$fname
  exit_code=$?

  

  if [[ $exit_code == $code ]]
  then
    printf "%s: ✔ PASSED" $fname
  else
    printf "%s: ❌ FAILED (Exit code: %d Expected: %d)" $fname $exit_code $code
  fi

  if [[ $code == 0 ]]
  then
    valgrind ./etapa4 < tests/E4/$fname > grep -q "no leaks are possible" 2>&1
    if [[ $? == 0 ]]
    then
      printf " ✔ VALGRIND"
    else
      printf " ❌ VALGRIND"
    fi
  fi

  printf "\n"
done < tests/E4/e4_expected_code.csv