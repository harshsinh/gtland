clear all
close all
clc

start=10;
stop=400;

R_actual=0.0212;
fx=757.32400;
fy=760.531946;
f=(fx+fy)/2;
cx=339.639683;
cy=227.812694;
% cx=300;
% cy=200;

RE=1e-10*eye(6);
QE=1e-5*[2 0 0
        0 1 0
        0 0 1];

fileID = fopen('dataset_spiral.jpg','r');
A= fscanf(fileID, ['(' '%d' ',' '%d' ')'  '%d' '\n'], [3 Inf]);

fps=30;
dt=1/fps;
U=A(1,start:stop)';
V=A(2,start:stop)';
R=A(3,start:stop)';

[X_E,K]=EKF(U,V,R,f,cx,cy,R_actual,QE,RE,dt,start);
[X_U,K]=UKF(U,V,R,f,cx,cy,R_actual,QE,RE,dt,start);
[X_PF,K]=PF(U,V,R,f,cx,cy,R_actual,QE,RE,dt,start);

d_RAW=R_actual*f./R;
Z_RAW=f*d_RAW./(((U-cx).^2+(V-cy).^2+f^2).^0.5);

X_RAW=[(U-cx).*Z_RAW/f      (V-cy).*Z_RAW/f         Z_RAW];

for i=1:size(X_RAW)-1
    V_RAW(i,:)=(X_RAW(i+1,:)-X_RAW(i,:))/dt;
end
V_RAW=[0 0 0 ; V_RAW];

% figure(1);
% plot(X_RAW(:,1),X_RAW(:,2),'g',X_E(:,1),X_E(:,2),'b',X_U(:,1),X_U(:,2),'r');
% axis equal; grid on;
% legend('Raw','EKF','UKF');
% xlabel('x(m)');ylabel('y(m)');
% 
% figure(2);
% plot(K,(X_RAW(:,1).^2+X_RAW(:,2).^2+X_RAW(:,3).^2).^0.5,'g',K,(X_E(:,1).^2+X_E(:,2).^2+X_E(:,3).^2).^0.5,'b',K,(X_U(:,1).^2+X_U(:,2).^2+X_U(:,3).^2).^0.5,'r');
% legend('Raw','EKF','UKF');xlabel('frame');ylabel('distance(m)');grid on;

figure(3);
subplot(3,1,1);
plot(K,X_RAW(:,1),'g',K,X_E(:,1),'b',K,X_U(:,1),'r',K,X_PF(:,1),'k');legend('Raw','EKF','UKF','PF');xlabel('frame');ylabel('x(m)');grid on;
subplot(3,1,2);
plot(K,X_RAW(:,2),'g',K,X_E(:,2),'b',K,X_U(:,2),'r',K,X_PF(:,2),'k');legend('Raw','EKF','UKF','PF');xlabel('frame');ylabel('y(m)');grid on;
subplot(3,1,3);
plot(K,X_RAW(:,3),'g',K,X_E(:,3),'b',K,X_U(:,3),'r',K,X_PF(:,3),'k');legend('Raw','EKF','UKF','PF');xlabel('frame');ylabel('z(m)');grid on;

% figure(4);
% plot(U,V);grid on;
% axis equal;xlabel('u');ylabel('v');
% 
% figure(5);
% plot3(X_RAW(:,1),X_RAW(:,2),X_RAW(:,3),'g',X_E(:,1),X_E(:,2),X_E(:,3),'b',X_U(:,1),X_U(:,2),X_U(:,3),'r');
% legend('Raw','EKF','UKF');xlabel('x(m)');ylabel('y(m)');zlabel('z(m)');grid on;
% % axis equal
% 
% figure(6);
% subplot(3,1,1);
% plot(K,V_RAW(:,1),'g',K,X_E(:,4),'b',K,X_U(:,4),'r');legend('Raw','EKF','UKF');xlabel('frame');ylabel('v_x(m/s)');grid on;
% subplot(3,1,2);
% plot(K,V_RAW(:,2),'g',K,X_E(:,5),'b',K,X_U(:,5),'r');legend('Raw','EKF','UKF');xlabel('frame');ylabel('v_y(m/s)');grid on;
% subplot(3,1,3);
% plot(K,V_RAW(:,3),'g',K,X_E(:,6),'b',K,X_U(:,6),'r');legend('Raw','EKF','UKF');xlabel('frame');ylabel('v_z(m/s)');grid on;