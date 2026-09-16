#!/bin/bash

for threads in 1 2 4 8 16 32 64 128 256 512 1024
do
  echo -n "$threads " >> res.txt
  ./a.out $threads | grep "total:" >> res.txt
done
