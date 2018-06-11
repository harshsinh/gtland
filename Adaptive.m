function [ State_mat,State_des,State_veh,Control_mat,State_veh_pred,t ] = Adaptive(gains)

global Thrust_max Thrust_min M_max M_min m initial_state dt time_end noise vehicle_mode gravity wind_mean wind_sd J
State_mat=initial_state;
x1=initial_state(1:3,1);
height=x1(3,1);
x2=initial_state(4:6,1);
x2_dot=[0;0;0];
Euler=initial_state(7:9,1);
omega=initial_state(10:12,1);
kp_outer=gains(1,1);
kv_outer=gains(2,1);
G_outer=gains(3,1);
kp_inner=gains(4,1);
kd_inner=gains(5,1);
ki_inner=gains(6,1);
B_outer=kp_outer+kv_outer;

prev_error_inner=[0;0;0];
error_int_outer=[0;0;0];error_int_inner=[0;0;0];

thetaf_est_outer=[0;0;0];
thetaf_est_dot_outer=[0;0;0];
theta_est_outer_int=[0;0;0];
Wf_outer=[0;0;0];
e2f_outer=[0;0;0];
%%%%%%%%%%%%%%% EKF stuff %%%%%%%%%%%%%%%%%%%%%%%%%%%%%
rel_statex=[0;0;0];rel_statey=[0;0;0];rel_statez=[0;0;0];
rel_statex_cov=100*eye(3,3);rel_statey_cov=100*eye(3,3);rel_statez_cov=100*eye(3,3);
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
iter=1;
for t=0:dt:time_end
    if (vehicle_mode==1)
        w=0.4;
        A=5;
        xv=[10*t;A*sin(w*t);0];
        xv_dot=[10;A*w*cos(w*t);0];
        xv_ddot=[0;-A*w*w*sin(w*t);0];
    end
    if (vehicle_mode==2)
        V=2;
%         w=0.4;
        R=5;
        w=V/R;
        xv=R*[cos(w*t);sin(w*t);0];
        xv_dot=R*w*[-sin(w*t);cos(w*t);0];
        xv_ddot=R*w*w*[-cos(w*t);-sin(w*t);0];
    end
    if (vehicle_mode==3)
        xv=[10*t;0.5*t^2;0];
        xv_dot=[10;1*t;0];
        xv_ddot=[0;1;0];
    end
    if (vehicle_mode==4)
        w=0.4;
        R=5;
        xv=R*[cos(w*t);0.5*sin(2*w*t);0];
        xv_dot=R*w*[-sin(w*t);0.5*2*cos(2*w*t);0];
        xv_ddot=R*w*w*[-cos(w*t);-0.5*2*2*sin(2*w*t);0];
    end
    %%%%%%%%%%%%%% Position %%%%%%%%%%%%%%%%%%%
    theta_est_outer=thetaf_est_dot_outer+B_outer*thetaf_est_outer;
    theta_est_outer_int=theta_est_outer_int+theta_est_outer*dt;
    sensor_data=(xv-x1)+noise*randn(3,1);
    
    [rel_statex,rel_statex_cov]=EKF(rel_statex,rel_statex_cov,sensor_data(1,1),dt);
    [rel_statey,rel_statey_cov]=EKF(rel_statey,rel_statey_cov,sensor_data(2,1),dt);
    [rel_statez,rel_statez_cov]=EKF(rel_statez,rel_statez_cov,sensor_data(3,1),dt);
    
    EKF_pos_des=x1+[rel_statex(1,1);rel_statey(1,1);rel_statez(1,1)];
    EKF_vel_des=x2+[rel_statex(2,1);rel_statey(2,1);rel_statez(2,1)];
    EKF_acc_des=x2_dot+[rel_statex(3,1);rel_statey(3,1);rel_statez(3,1)];
    
        if (abs(rel_statex(1,1))<(abs(rel_statez(1,1))+0.01) && abs(rel_statey(1,1))<(abs(rel_statez(1,1))+0.01))
            if (-rel_statez(1,1)<0.01) height=0;
            else height=height-0.5*dt;
            end
        end
    pos_des(1:2,:)=EKF_pos_des(1:2,:);
    pos_des(3,:)=height;
%     pos_des=EKF_pos_des;
%     theta_est_outer(3,1)=0;
%     theta_est_outer_int(3,1)=0;
    acc_des=-theta_est_outer;
    vel_des=-theta_est_outer_int;
    
    e1_outer=x1-pos_des;
    e2_outer=x2-vel_des;
    e1_dot_outer=e2_outer;
    %     e2_dot_outer=x2_dot-acc_des;
    %calculating derivates of some state variables
    W_outer=gravity;
    Wf_dot_outer=-B_outer*Wf_outer+W_outer;
    e2f_dot_outer=-B_outer*e2f_outer+e2_outer;
    
    thetaf_est_dot_outer=G_outer*((e2f_outer+e1_outer));
    uf_outer=m*(Wf_outer-kp_outer*e1_outer-kv_outer*e2f_outer-thetaf_est_outer);
    uf_dot_outer=m*(Wf_dot_outer-kp_outer*e1_dot_outer-kv_outer*e2f_dot_outer-thetaf_est_dot_outer);
    u_outer=uf_dot_outer+B_outer*uf_outer;
    Thrust=((u_outer(1,1)^2)+(u_outer(2,1)^2)+(u_outer(3,1)^2))^0.5;
    psi_des=atan2(-e1_outer(2,1),-e1_outer(1,1));
    phi_des=asin((u_outer(1,1)*sin(psi_des)-u_outer(2,1)*cos(psi_des))/(Thrust));
    theta_des=asin((u_outer(1,1)*cos(psi_des)+u_outer(2,1)*sin(psi_des))/((Thrust)*cos(phi_des)));
    Euler_des=[phi_des;theta_des;psi_des];
    if (x1(3,1)<=0)
        Thrust=m*gravity(3,1);
        x1=xv;x2=xv_dot;
        Euler_des=[0;0;atan2(xv_dot(2,1),xv_dot(1,1))];
    end
    %%%%%%%%%%%%%%%%%%%%%%%%%%%
    %%%%%%%%%%%%%% Attitude %%%%%%%%%%%%%%%%%%%
    error_inner=Euler_des-Euler;
    error_dot_inner=(error_inner-prev_error_inner)/dt;
    error_int_inner=error_int_inner+error_inner*dt;
    M=kp_inner*(error_inner)+kd_inner*(error_dot_inner)+ki_inner*(error_int_inner);
    prev_error_inner=error_inner;
    if (Thrust>Thrust_max) Thrust=Thrust_max; end
    if (Thrust<Thrust_min) Thrust=Thrust_min; end
    if (M(1,1)>M_max(1,1)) M(1,1)=M_max(1,1); end
    if (M(2,1)>M_max(2,1)) M(2,1)=M_max(2,1); end
    if (M(3,1)>M_max(3,1)) M(3,1)=M_max(3,1); end
    if (M(1,1)<M_min(1,1)) M(1,1)=M_min(1,1); end
    if (M(2,1)<M_min(2,1)) M(2,1)=M_min(2,1); end
    if (M(3,1)<M_min(3,1)) M(3,1)=M_min(3,1); end
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    wind_vel=wind_mean+wind_sd*randn(3,1);
    R=eul2rotm([Euler(3,1) Euler(2,1) Euler(1,1)]);
    x2_dot=-gravity+(1/m)*R*[0;0;Thrust]+(1/m)*0.5*1.225*(sign(wind_vel).*(wind_vel.^2))*(2*0.5*0.1);
    x1_dot=x2;
    x1=x1+x1_dot*dt;
    x2=x2+x2_dot*dt;
    omega_hat=[ 0 -omega(3,1) omega(2,1);
        omega(3,1) 0 -omega(1,1);
        -omega(2,1) omega(1,1) 0];
    omega_dot=inv(J)*(-omega_hat*(J*omega)+M);
    omega=omega+omega_dot*dt;
    A=[1 sin(Euler(1,1))*tan(Euler(2,1)) cos(Euler(1,1))*tan(Euler(2,1));
        0 cos(Euler(1,1)) -sin(Euler(1,1));
        0 sin(Euler(1,1))/cos(Euler(2,1)) cos(Euler(1,1))/cos(Euler(2,1))];
    Euler_dot=A*omega;
    Euler=Euler+Euler_dot*dt;
    State_mat(:,iter)=[x1;x2;Euler;omega];
    State_des(:,iter)=[EKF_pos_des;EKF_vel_des;Euler_des;[0;0;0]];
    State_veh(:,iter)=[xv;xv_dot;xv_ddot];
    Control_mat(:,iter)=[Thrust;M];
    State_veh_pred(:,iter)=[xv;-theta_est_outer_int;-theta_est_outer];
    iter=iter+1;
end
end