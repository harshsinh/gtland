function [x,P]= UKF_update(x,P,z,QE,R_actual,f,cx,cy)
% correcting the predicted pose using Kalman gain.

% Inputs:
%   x, P -  state and covariance
%   z, R - range-bearing measurements and covariances
%   idf - indecies of each landmark from which measurements have arrived

% Outputs:
%   x, P - updated state and covariance


% <---------------------------- TO DO -------------------------->
%generate sigma points
n=6;
alpha=1e-3;
kappa=0;
beta=2;
lambda=(alpha^2*(n+kappa))-n;
% lambda=1;


% x(3,1)=pi_to_pi(x(3,1));
    P_bar_aug=[];
    S_aug=[];
    z_hat_aug=[];
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
        
        Z=[];
        for i=1:2*n+1
            X_mag=(X(1,i)^2+X(2,i)^2+X(3,i)^2)^0.5;
            r_cap=R_actual*f/X_mag;
            u_cap=f*X(1,i)/X(3,i)+cx;
            v_cap=f*X(2,i)/X(3,i)+cy;
        
            z_cap=[r_cap;u_cap;v_cap];
            Z=[Z z_cap];
        end
        
        z_hat=0;
        for i=1:2*n+1
            z_hat=z_hat+w_m(i)*Z(:,i);
        end
        
        S=0;
        for i=1:2*n+1
            S=S+(w_c(i)*(Z(:,i)-z_hat)*transpose((Z(:,i)-z_hat)));
        end
        S=S+QE;
        
        P_bar=0;
        for i=1:2*n+1
            P_bar=P_bar+w_c(i)*(X(:,i)-x)*transpose((Z(:,i)-z_hat));
        end
        
        K=P_bar*inv(S);
        zn=(z-z_hat);
        x=real(x+K*(zn));
        P=real(P-K*S*transpose(K));
end








