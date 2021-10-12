set yrange [0 : 1]
set xlabel 'The percentage of candidate nodes of RAFR'
set ylabel 'Delivery rate'
set key right center
set xtics 10
# set xrange [20 : 80]


set terminal postscript eps color enhanced "Courier" 25
set colorsequence classic
set output "del_rat.eps"
plot "del_ratDEGREE.txt" with linespoints pointtype 4 pointsize 2 title 'Degree',\
"del_ratBETWEENNESS.txt" with linespoints pointtype 2 pointsize 2 title 'Betweenness',\
"del_ratRAFR.txt" with linespoints pointtype 6 pointsize 2 title 'RAFR'
set output
