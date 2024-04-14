#!/bin/bash

if [ -z $1 ]; then
  echo "Надо указать название проверяющей программы"
  echo "Например:"
  echo "      ./compilecheker icmp"
  exit
fi

cd checker
mv $1.cpp checker_$1.cpp
g++ -O2 -std=c++17 -ocheck -static checker_$1.cpp
strip check
cd ..
