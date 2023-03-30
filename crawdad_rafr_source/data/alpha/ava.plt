set yrange [0 : 1]
set xlabel 'The offline nodes selection parameter alpha'
set ylabel 'Availability'
set key left bottom
# set key right top
# set key right center
set xtics 0.1
set xrange [0.1 : 1.0]


set terminal postscript eps color enhanced "Courier" 25
set colorsequence classic
set output "ava.eps"
plot "avaDEGREE.txt" with linespoints pointtype 4 pointsize 2 title 'Degree',\
"avaBETWEENNESS.txt" with linespoints pointtype 2 pointsize 2 title 'Betweenness',\
"avaCONNECTIVITY.txt" with linespoints pointtype 8 pointsize 2 title 'Connectivity',\
"avaRAFR.txt" with linespoints pointtype 6 pointsize 2 title 'RAFC',\
"avaOPT.txt" with linespoints pointtype 10 pointsize 2 title 'Optimal'
set output
