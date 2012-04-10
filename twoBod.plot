
set multiplot;

set xlabel "2D Position"
set size 1,0.7;
set origin 0,0.3;
plot "twoBod.out" using 2:3 every 2::0 title 'body A' with linespoints, \
	 "twoBod.out" using 2:3 every 2::1 title 'body B' with linespoints

set size 1,0.3;
set origin 0,0;
# KE = 0.5 * m * v^2
# m = $1
# x,y,z = $2,$3,$4
# vx,vy,vz = $5,$6,$7
# v = sqrt(vx*vx+vy*vy+vz*vz)
set xlabel "Step Number"
set ylabel "Kinetic Energy"
plot "twoBod.out" using (0.5*$1*sqrt($5*$5+$6*$6+$7*$7)) title 'Kinetic Energy' with impulses;
unset multiplot;