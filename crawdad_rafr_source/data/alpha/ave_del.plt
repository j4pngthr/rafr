set xlabel 'The offline nodes selection parameter alpha'
set ylabel 'Average delay'
set key left top
set xtics 0.1
set xrange [0.1 : 1.0]


set terminal postscript eps color enhanced "Courier" 25
set colorsequence classic
set output "ave_del.eps"
plot "ave_delDEGREE.txt" with linespoints pointtype 4 pointsize 2 title 'Degree',\
"ave_delBETWEENNESS.txt" with linespoints pointtype 2 pointsize 2 title 'Betweenness',\
"ave_delCONNECTIVITY.txt" with linespoints pointtype 8 pointsize 2 title 'Connectivity',\
"ave_delRAFR.txt" with linespoints pointtype 6 pointsize 2 title 'RAFC',\
"ave_delOPT.txt" with linespoints pointtype 10 title 'Optimal'
set output
