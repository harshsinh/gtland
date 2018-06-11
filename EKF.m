function [ mu,sigma ] = EKF( mu,sigma,z_t,dt )
%UNTITLED2 Summary of this function goes here
%   Detailed explanation goes here
A=[1 dt 0.5*dt^2;
   0 1  dt;
   0 0  1];
R=0.001*eye(3,3);
mu_bar=A*mu;
sigma_bar=A*sigma*A'+R;

C=[1 0 0];
Q=0.1;
K=sigma_bar*C'*inv(C*sigma_bar*C'+Q);
mu=mu_bar+K*(z_t-C*mu_bar);
sigma=(eye(3,3)-K*C)*sigma_bar;
end

