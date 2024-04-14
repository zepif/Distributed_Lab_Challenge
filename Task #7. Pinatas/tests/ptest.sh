#!/bin/bash

N=$(find . -maxdepth 1 -type f -name "*.in" | wc -l)

for i in $(seq -w 1 $N); do
    echo "File: $i.in"
    cat "$i.in"
    echo ""

    echo "File: $i.out"
    cat "$i.out"
    echo ""

    echo "=================================================="
done
