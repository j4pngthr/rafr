set xlabel 'The percentage of candidate nodes of RAFC'
set ylabel 'Average delay'
set key right bottom
set xtics 10
# set xrange [20 : 80]


set terminal postscript eps color enhanced "Courier" 25
set colorsequence classic
set output "ave_del.eps"
plot "ave_delDEGREE.txt" with linespoints pointtype 2 pointsize 2 title 'Degree',\
"ave_delBETWEENNESS.txt" with linespoints pointtype 4 pointsize 2 title 'Betweenness',\
"ave_delCONNECTIVITY.txt" with linespoints pointtype 6 pointsize 2 title 'Connectivity',\
"ave_delRAFR.txt" with linespoints pointtype 8 pointsize 2 title 'RAFC',\
"ave_delOPT.txt" with linespoints pointtype 10 pointsize 2 title 'Optimal'
set output
