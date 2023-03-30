set xlabel 'Node ID'
set ylabel 'Node score'
set key left top

set terminal postscript eps color enhanced "Courier" 25
set colorsequence classic
set output "metric_value.eps"
plot "metric_value.txt" with boxes title "WC Score"
set output
