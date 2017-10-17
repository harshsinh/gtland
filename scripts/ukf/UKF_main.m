function [ X,K ] = UKF( U,V,R,f,cx,cy,R_actual,QE,RE,dt,start )
%UNTITLED Summary of this function goes here
%   Detailed explanation goes here
d=R_actual*f/R(1,1);
z=d*f/(((U(1,1)-cx)^2+(V(1,1)-cy)^2+f^2)^0.5);

x=[(U(1,1)-cx)*z/f;
    (V(1,1)-cy)*z/f;
    z;0;0;0];

P=0.01*[eye(3) zeros(3)
    zeros(3) eye(3)];
X=[];
for i=1:1:size(U)
    z=[R(i);U(i);V(i)];
    
    [x,P]=UKF_predict(x,P,dt,RE);
    [x,P]=UKF_update(x,P,z,QE,R_actual,f,cx,cy);
    
    X=[X;x'];
    K(i)=start+i;
end

end

