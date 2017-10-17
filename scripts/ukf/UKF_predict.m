function [x,P]= UKF_predict(x,P,dt,RE)
% Inputs:
%   x, P - state and covariance
%   v, g - control inputs: velocity and gamma (steer angle)
%   Q - covariance matrix for velocity and gamma
%   dt - timestep
%
% Outputs:
%   x, P - predicted state and covariance

% <------------------------- TO DO -------------------------->

%generate sigma points
n=6;
alpha=1e-3;
kappa=0;
beta=2;
lambda=(alpha^2*(n+kappa))-n;
% lambda=0.1;

% x_aug=[x;0;0];
% X=x_aug;
% P_aug=[P,zeros(3,2);
%     zeros(2,3),Q];
X=x;

P_sqrt=real(sqrtm(((n+lambda).*P)));

for i=1:n
    X=[X x+P_sqrt(:,i)];
end
for i=n+1:2*n
    X=[X x-P_sqrt(:,i-n)];
end

%calculate weights
w_m(1)=lambda/(n+lambda);
w_c(1)=w_m(1)+(1-alpha^2+beta);
for i=1:2*n
    w_m(i+1)=1/(2*(n+lambda));
    w_c(i+1)=1/(2*(n+lambda));
end

A=[1 0 0 dt 0 0;
   0 1 0 0 dt 0;
   0 0 1 0 0 dt;
   0 0 0 1 0 0;
   0 0 0 0 1 0;
   0 0 0 0 0 1];

x_star=[];
for i=1:2*n+1
    x_star=[x_star A*X(:,i)];
end

x_bar=0;
P_bar=0;
for i=1:2*n+1
    x_bar=x_bar+(w_m(i)*x_star(:,i));
end
% x=real(x_bar);
% x(3,1)=pi_to_pi(x(3,1));

for i=1:2*n+1
    P_bar=P_bar+(w_c(i)*(x_star(:,i)-x_bar)*transpose(x_star(:,i)-x_bar));
end

P=real(P_bar)+RE;

end






