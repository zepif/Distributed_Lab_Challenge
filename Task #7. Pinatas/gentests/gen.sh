#!/bin/bash

g++ -o generator generator.cpp
g++ -o solution ../sols/tl.cpp

mkdir -p ../tests

for i in `seq -w 1 50`;
do
    ./generator > ../tests/${i}.in
    ./solution < ../tests/${i}.in > ../tests/${i}.out
    echo "Saved test case $i to tests/${i}_in.txt and tests/${i}_out.txt"
done
