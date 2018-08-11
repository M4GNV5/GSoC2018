#!/bin/bash

name="$1"
singleArgs="$2"
multiArgs="$3"
multiCount="$4"

mkdir -p bin

gcc "programs/$name.c" new/jit/.libs/libjit.a -Inew/include -lm -lpthread -o "bin/${name}New"
gcc "programs/$name.c" old/jit/.libs/libjit.a -Iold/include -lm -lpthread -o "bin/${name}Old"

singleTimeOld=$(/usr/bin/time -f '%e' "bin/${name}Old" $singleArgs 2>&1 > /dev/null)
singleTimeNew=$(/usr/bin/time -f '%e' "bin/${name}New" $singleArgs 2>&1 > /dev/null)

printf "0 $singleTimeOld\n0.5 $singleTimeNew\n" > plot.dat

function multiRun()
{
    startTime=$(date +"%s.%N")
    for i in $(seq 1 "$multiCount"); do
        "$1" $2 > /dev/null
    done
    endTime=$(date +"%s.%N")

    echo "$endTime - $startTime" | bc -l
}

multiTimeOld=$(multiRun "bin/${name}Old" "$multiArgs")
multiTimeNew=$(multiRun "bin/${name}New" "$multiArgs")

printf "1.5 $multiTimeOld\n2 $multiTimeNew\n" >> plot.dat

gnuplot << EOF
set term png
set output "output.png"
set boxwidth 0.5
set xtics ("Single Run" 0.25, "Multiple Runs" 1.75,)

set style fill solid
set key center top
set yrange [0:]
set ylabel "Execution time in seconds"
set style line 1 lc rgb "blue"
set style line 2 lc rgb "red"

plot 'plot.dat' every 2    using 1:2 with boxes ls 1 t "old register allocator",\
     'plot.dat' every 2::1 using 1:2 with boxes ls 2 t "new register allocator"
EOF

mkdir -p img
mv output.png "img/$name.png"

#rm plot.dat
