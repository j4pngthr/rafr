set yrange [0 : 1]
set xlabel 'The percentage of offline nodes'
set ylabel 'Delivery rate'
set key right center
set xtics 1
set xrange [1 : 10.3]


set terminal postscript eps color enhanced "Courier" 25
set colorsequence classic
set output "del_rat.eps"
plot "del_ratDEGREE.txt" with linespoints pointtype 4 pointsize 2 title 'Degree',\
"del_ratBETWEENNESS.txt" with linespoints pointtype 2 pointsize 2 title 'Betweenness',\
"del_ratCONNECTIVITY.txt" with linespoints pointtype 8 pointsize 2 title 'Connectivity',\
"del_ratRAFR.txt" with linespoints pointtype 6 pointsize 2 title 'RAFC',\
"del_ratOPT.txt" with linespoints pointtype 10 pointsize 2 title 'Optimal'
set output
