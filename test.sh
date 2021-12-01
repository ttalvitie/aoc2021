#!/bin/bash

set -e

for i in {1..1}
do
    echo "Day $i debug"
    ./aoc2021.debug $i < data/input$i.txt | diff - data/output$i.txt
    echo "Day $i release"
    ./aoc2021.release $i < data/input$i.txt | diff - data/output$i.txt
done

echo "Tests passed"
