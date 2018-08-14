#!/bin/bash

name="$1"
args="$2"
compileCount="$3"

mkdir -p bin

gcc "programs/$name.c" new/jit/.libs/libjit.a -Inew/include -lm -lpthread -o "bin/${name}New" -DFUNCTION_ENABLE_DUMP
gcc "programs/$name.c" old/jit/.libs/libjit.a -Iold/include -lm -lpthread -o "bin/${name}Old" -DFUNCTION_ENABLE_DUMP

function multiRun()
{
    startTime=$(date +"%s.%N")
    for i in $(seq 1 "$compileCount"); do
        "$1" $2 > /dev/null
    done
    endTime=$(date +"%s.%N")

    echo "$endTime - $startTime" | bc -l
}

compileTimeNew=$(multiRun "bin/${name}New" "$args")
compileTimeOld=$(multiRun "bin/${name}Old" "$args")

x=$(wc -l plot.dat | cut -d ' ' -f 1)
printf "$x $compileTimeOld $x.5 $compileTimeNew $compileCount $args\n" >> plot.dat
