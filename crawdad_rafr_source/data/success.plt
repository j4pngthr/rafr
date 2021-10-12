set yrange [0 : 1]
set xlabel 'Delay'
set ylabel 'Delivery rate'
set key right bottom
set logscale x
# set format x "10^{%T}"
# set format x "%.0t"
# set xrange [1 : 10]
set format x "%.0t*10^{%T}"
# set xtics 10000

set terminal postscript eps color enhanced "Courier" 25
set colorsequence classic
set output "success.eps"
plot "success_rateDEGREE.txt" with linespoints pointtype 4 pointsize 2 title 'Degree',\
"success_rateBETWEENNESS.txt" with linespoints pointtype 2 pointsize 2 title 'Betweenness',\
"success_rateRAFR.txt" with linespoints pointtype 6 pointsize 2 title 'RAFR'
set output

