% Clear all variables
clear variables; clc;
num = [0 0.7707];
dnum = [6.5e-6 0.0121 1.136 0];
% Open loop transfer function
G_hor = tf(num, dnum);
%Closed loop transfer function
Gcl_hor = feedback(G_hor, 1, -1);
%PID parameters
Kp = 1646.7;
Ki = 2.2e5;
Kd = 3.09;
% pid_hor = pid(Kp, Ki, Kd);
% Transfer function with PID
% Gpid_hor = feedback(pid_hor*G_hor, 1, -1);
% step(Gpid_hor); grid on;
% pidTuner(G_hor, pid_hor);
% clear;clc
% %open loop tf
% numt = [0.7707];
% dent = [1.0404*10^-5 0.0185 0.722 0];
% Gt = tf(numt,dent);
% Tt = feedback(Gt,1);
% %PID
% Kp = 999.481;
% Ki = 8.3814*10^4;
% Kd = 2.9794;
% %Kp = 200;
% %Ki = 8.3814*10^4;
% %Kd = 50;
% PIDCont = pid(Kp,Ki,Kd);
% % open loop tf with PID controller
% Gtc = PIDCont*Gt;
% %closed loop tf
% Ttc = feedback(Gtc,1);
% %step response
% step(Ttc),grid on;