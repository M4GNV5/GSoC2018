set term png
set output "output.png"
set boxwidth 0.5
set xtics ("10^7" 0.25, "10^8" 1.75, "10^9" 3.25)

set style fill solid
set key left top
set xlabel "Iterations"
set yrange [0:]
set ylabel "Execution time in seconds"
set style line 1 lc rgb "blue"
set style line 2 lc rgb "red"

plot 'plot.dat' using 1:2 with boxes ls 1 t "old register allocator",\
     'plot.dat' using 3:4 with boxes ls 2 t "new register allocator"
