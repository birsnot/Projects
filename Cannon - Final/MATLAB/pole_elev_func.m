clear variables; clc;
num = [0 0.7707];
dnum = [6.37e-6 0.0119 1.136 0];
G_elev = tf(num, dnum);
% pole_elev = pole(G_elev);
Gcl_elev = feedback(G_elev, 1, -1);
% pole_cl_elev = pole(Gcl_elev);
step(Gcl_elev); grid on;