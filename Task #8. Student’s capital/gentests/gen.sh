#!/bin/bash

g++ -o generator generator.cpp
g++ -o solution ../sols/main.cpp

mkdir -p ../tests

for i in `seq -w 1 30`;
do
    ./generator > ../tests/${i}.in
    ./solution < ../tests/${i}.in > ../tests/${i}.out
    echo "Saved test case $i to tests/${i}.in and tests/${i}.out"
done
