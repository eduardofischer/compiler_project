#!/bin/bash

while IFS=, read -r fname code
do
  ./etapa4 < tests/E4/$fname > tests/E4/output/$fname
  exit_code=$?

  if [[ $exit_code == $code ]]
  then
    echo "$fname: ✔ PASSED"
  else
    echo "$fname: ❌ FAILED (Exit code: $exit_code Expected: $code)"
  fi
done < tests/E4/e4_expected_code.csv