#!/bin/bash
names=$(grep -E "still" last.fake | awk '{print $1}')

for name in $names; do
  eval=$(awk '{print $1}' ps.fake | grep -E "^$name" | wc -l)
  echo $name $eval
done

# NOT OK idk why
