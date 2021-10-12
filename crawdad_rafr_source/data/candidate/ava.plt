set yrange [0 : 1]
set xlabel 'The percentage of candidate nodes of RAFR'
set ylabel 'Availability'
set key right bottom
# set key right top
# set key right center
set xtics 10
# set xrange [20 : 80]


set terminal postscript eps color enhanced "Courier" 25
set colorsequence classic
set output "ava.eps"
plot "avaDEGREE.txt" with linespoints pointtype 4 pointsize 2 title 'Degree',\
"avaBETWEENNESS.txt" with linespoints pointtype 2 pointsize 2 title 'Betweenness',\
"avaRAFR.txt" with linespoints pointtype 6 pointsize 2 title 'RAFR'
set output
