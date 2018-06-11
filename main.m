clear all
close all
clc
global Thrust_max Thrust_min M_max M_min m initial_state dt time_end noise vehicle_mode gravity wind_mean wind_sd J
initial_state=[0;0;5;0;0;0;0;0;0;0;0;0];
dt=0.01
time_end=30;
noise=0.1;
m=1.2;
Thrust_max=30;
Thrust_min=-Thrust_max;
M_max=5*[1;1;1];
M_min=-M_max;
vehicle_mode=2;
gravity=[0;0;9.81];
wind_mean=[-0;-0;0];
wind_sd=0;
J=[ 0.025,   0,         0;
    0,      0.025,      0;
    0,      0,       0.05];

kp_PID_outer=0.5;kd_PID_outer=0.3;ki_PID_outer=0.01;
kp_PID_inner=0.5;kd_PID_inner=0.3;ki_PID_inner=0.01;
gains_PID=[kp_PID_outer;kd_PID_outer;ki_PID_outer;kp_PID_inner;kd_PID_inner;ki_PID_inner];
[ State_PID,State_des_PID,State_veh,Control_PID ]=PID(gains_PID);

kp_outer=1.2;
kv_outer=1.2;
G_outer=0.8;
gains_Ad=[kp_outer;kv_outer;G_outer;kp_PID_inner;kd_PID_inner;ki_PID_inner];
[ State_Ad,State_des_Ad,State_veh,Control_Ad,State_veh_pred ]=Adaptive(gains_Ad);


time=0:dt:time_end;
figure(1);
subplot(3,1,1);
plot(time,State_PID(1,:),'b',time,State_Ad(1,:),'r',time,State_veh(1,:),'--k');title('Position Plots');
%axis([0,2.5,-0.01,0.01]);
ylabel('x(m)');xlabel('time(s)');legend('quad x PID','quad x Adaptive','Vehicle x');grid on;
subplot(3,1,2);
plot(time,State_PID(2,:),'b',time,State_Ad(2,:),'r',time,State_veh(2,:),'--k');
ylabel('y(m)');xlabel('time(s)');legend('quad y PID','quad y Adaptive','Vehicle y');grid on;
subplot(3,1,3);
plot(time,State_PID(3,:),'b',time,State_Ad(3,:),'r',time,State_veh(3,:),'--k');
ylabel('z(m)');xlabel('time(s)');legend('quad z PID','quad z Adaptive','Vehicle z');grid on;
hold on;

figure(2);
subplot(3,1,1);
plot(time,State_des_PID(7,:),'--k',time,State_PID(7,:),'r');title('Euler Plots');
%axis([0,2.5,-0.01,0.01]);
ylabel('\phi');xlabel('time(s)');legend('\phi PID');grid on;
subplot(3,1,2);
plot(time,State_des_PID(8,:),'--k',time,State_PID(8,:),'r');
ylabel('\theta');xlabel('time(s)');legend('\theta PID');grid on;
subplot(3,1,3);
plot(time,State_des_PID(9,:),'--k',time,State_PID(9,:),'r');
ylabel('\psi');xlabel('time(s)');legend('\psi PID');grid on;
hold on;

figure(3);
subplot(3,1,1);
plot(time,State_des_Ad(7,:),'--k',time,State_Ad(7,:),'r');title('Euler Plots');
%axis([0,2.5,-0.01,0.01]);
ylabel('\phi');xlabel('time(s)');legend('\phi Adaptive');grid on;
subplot(3,1,2);
plot(time,State_des_Ad(8,:),'--k',time,State_Ad(8,:),'r');
ylabel('\theta');xlabel('time(s)');legend('\theta Adaptive');grid on;
subplot(3,1,3);
plot(time,State_des_Ad(9,:),'--k',time,State_Ad(9,:),'r');
ylabel('\psi');xlabel('time(s)');legend('\psi Adaptive');grid on;
hold on;

figure(4);
subplot(3,1,1);
plot(time,State_veh(1,:)-State_PID(1,:),'b',time,State_veh(1,:)-State_Ad(1,:),'r');title('Position Error Plots');
%axis([0,2.5,-0.01,0.01]);
ylabel('x(m)');xlabel('time(s)');legend('\Delta x PID','\Delta x Adaptive');grid on;
subplot(3,1,2);
plot(time,State_veh(2,:)-State_PID(2,:),'b',time,State_veh(2,:)-State_Ad(2,:),'r');
ylabel('y(m)');xlabel('time(s)');legend('\Delta y PID','\Delta y Adaptive');grid on;
subplot(3,1,3);
plot(time,State_veh(3,:)-State_PID(3,:),'b',time,State_veh(3,:)-State_Ad(3,:),'r');
ylabel('z(m)');xlabel('time(s)');legend('\Delta z PID','\Delta z Adaptive');grid on;
hold on;

figure(5);
subplot(3,1,1);
plot(time,State_veh(7,:),'--k',time,State_veh_pred(7,:),'r');title('Vehicle Acceleration Prediction');
%axis([0,2.5,-0.01,0.01]);
ylabel('acc_x(m/s^2)');xlabel('time(s)');legend('Actual Acc x','Predicted Acc x');grid on;
subplot(3,1,2);
plot(time,State_veh(8,:),'--k',time,State_veh_pred(8,:),'r');
ylabel('acc_y(m/s^2)');xlabel('time(s)');legend('Actual Acc y','Predicted Acc y');grid on;
subplot(3,1,3);
plot(time,State_veh(9,:),'--k',time,State_veh_pred(9,:),'r');
ylabel('acc_z(m/s^2)');xlabel('time(s)');legend('Actual Acc z','Predicted Acc z');grid on;
hold on;

figure(6);
subplot(3,1,1);
plot(time,State_veh(4,:),'--k',time,State_veh_pred(4,:),'r');title('Vehicle Velocity Prediction');
%axis([0,2.5,-0.01,0.01]);
ylabel('Vel_x(m/s^2)');xlabel('time(s)');legend('Actual Vel x','Predicted Vel x');grid on;
subplot(3,1,2);
plot(time,State_veh(5,:),'--k',time,State_veh_pred(5,:),'r');
ylabel('Vel_y(m/s^2)');xlabel('time(s)');legend('Actual Vel y','Predicted Vel y');grid on;
subplot(3,1,3);
plot(time,State_veh(6,:),'--k',time,State_veh_pred(6,:),'r');
ylabel('Vel_z(m/s^2)');xlabel('time(s)');legend('Actual Vel z','Predicted Vel z');grid on;
hold on;

figure(7)
plot3(State_veh(1,:),State_veh(2,:),State_veh(3,:),'--k');hold on;
plot3(State_PID(1,:),State_PID(2,:),State_PID(3,:),'b');
plot3(State_Ad(1,:),State_Ad(2,:),State_Ad(3,:),'r');
title('3D position plots')
xlabel('x(m)');ylabel('y(m)');zlabel('z(m)');legend('Vehicle Trajectory','Quad PID trajectory','Quad Adaptive Trajectory');grid on;
axis equal;

figure(8);
subplot(2,2,1);
plot(time,Control_PID(1,:),'b',time,Control_Ad(1,:),'r');title('Control plots');
ylabel('Thrust(N)');xlabel('time(s)');legend('Thrust PID','Thrust Adaptive');
subplot(2,2,2);
plot(time,Control_PID(2,:),'b',time,Control_Ad(2,:),'r');
ylabel('l(N-m)');xlabel('time(s)');legend('l PID','l Adaptive');
subplot(2,2,3);
plot(time,Control_PID(3,:),'b',time,Control_Ad(3,:),'r');
%axis([0,3,-0.01,0.01]);
ylabel('m(N-m)');xlabel('time(s)');legend('m PID','m Adaptive');
subplot(2,2,4);
plot(time,Control_PID(4,:),'b',time,Control_Ad(4,:),'r');
%axis([0,3,-0.01,0.01]);
ylabel('n(N-m)');xlabel('time(s)');legend('n PID','n Adaptive');
hold on;