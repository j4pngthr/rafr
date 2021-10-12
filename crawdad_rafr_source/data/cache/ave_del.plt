set xlabel 'The percentage of caching nodes'
set ylabel 'Average delay'
set key right top
set xtics 1
set xrange [1 : 10]


set terminal postscript eps color enhanced "Courier" 25
set colorsequence classic
set output "ave_del.eps"
plot "ave_delDEGREE.txt" with linespoints pointtype 4 pointsize 2 title 'Degree',\
"ave_delBETWEENNESS.txt" with linespoints pointtype 2 pointsize 2 title 'Betweenness',\
"ave_delRAFR.txt" with linespoints pointtype 6 pointsize 2 title 'RAFR'
set output
