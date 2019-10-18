#!/bin/bash

make
for i in `ls ../tests`
do
    ./demo ../tests/$i > ../output/$i.out
done
