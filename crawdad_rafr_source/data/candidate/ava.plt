set yrange [0 : 1]
set xlabel 'The percentage of candidate nodes of RAFC'
set ylabel 'Availability'
set key right bottom
# set key right top
# set key right center
set xtics 10
# set xrange [20 : 80]


set terminal postscript eps color enhanced "Courier" 25
set colorsequence classic
set output "ava.eps"
plot "avaDEGREE.txt" with linespoints pointtype 2 pointsize 2 title 'Degree',\
  "avaBETWEENNESS.txt" with linespoints pointtype 4 pointsize 2 title 'Betweenness',\
  "avaCONNECTIVITY.txt" with linespoints pointtype 6 pointsize 2 title 'Connectivity',\
  "avaRAFR.txt" with linespoints pointtype 8 pointsize 2 title 'RAFC',\
  "avaOPT.txt" with linespoints pointtype 10 pointsize 2 title 'Optimal'
set output
