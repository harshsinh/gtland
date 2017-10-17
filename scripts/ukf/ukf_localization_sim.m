function output= ukf_localization_sim(lm, wp)
  
% INPUTS: 
%   lm - set of landmarks
%   wp - set of waypoints
%
% OUTPUTS:
%   data - a data structure containing:
%          data.true: the vehicle 'true'-path (ie, where the vehicle *actually* went)
%          data.path: the vehicle path estimate (ie, estimates traversed vehicle path)
%          data.state(k).x: the state vector at time k
%          data.state(k).P: the diagonals of the covariance matrix at time k
 
format compact
configfile; % ** USE THIS FILE TO CONFIGURE THE EKF-Localization**


% Some boring stuff to setup plots :(but helps in getting good visuualization)

fig1=figure;
plot(lm(1,:),lm(2,:),'b*')
hold on, axis([-180 150 -120 150])
plot(wp(1,:),wp(2,:), 'g', wp(1,:),wp(2,:),'g.')
xlabel('metres'), ylabel('metres')
title('Simulated Environment')

fig=figure;
plot(lm(1,:),lm(2,:),'b*')
hold on,  axis([-180 150 -120 150])
xlabel('metres'), ylabel('metres')
title('UKF-Localization')

h= setup_animations;
veh= [0 -WHEELBASE -WHEELBASE; 0 -2 2]; % vehicle animation
plines=[]; % for laser line animation
pcount=0;

% initialise states
xtrue= zeros(3,1);
x= zeros(3,1);
P= zeros(3);
% P=4*eye(3);

% Initialise other variables and constants

dt= DT_CONTROLS;    % Time interval between control signals in 'seconds'
dtsum= 0;           % change in time since last observation
ftag= 1:size(lm,2); % identifier for each landmark
 
iwp = 1; % index to first waypoint 
G = 0; % initial steer angle
output = initialise_store(x,P,xtrue); % stored data for off-line
QE= Q;
RE= R;  
l=2;
E=0;
Pd=zeros(3,5590);
% main loop 
while iwp ~= 0
    
    % compute true data
    [G,iwp]= compute_steering(xtrue, wp, iwp, AT_WAYPOINT, G, RATEG, MAXG, dt);
    %if iwp==0 & NUMBER_LOOPS > 1, iwp=1; NUMBER_LOOPS= NUMBER_LOOPS-1; end % perform loops: if final waypoint reached, go back to first
    xtrue= vehicle_model(xtrue, V,G,dt);
    
    % Adding noise to controls.
    [Vn,Gn]= add_control_noise(V,G,Q, SWITCH_CONTROL_NOISE);
    
    % EKF predict step ----------------------------> TO DO
    [x,P]= UKF_predict(x,P, Vn,Gn,QE,dt);
    dtsum= dtsum + dt;
     
    if dtsum >= DT_OBSERVE
    dtsum = 0;
    % generating observations
    [z,ftag_visible]= get_observations(xtrue, lm, ftag, MAX_RANGE);
    z= add_observation_noise(z,R, SWITCH_SENSOR_NOISE);
    
    
    idf = ftag_visible; % known data association. i.e this idf has indices of the landmarks from which observations are generated.  
    
    
    % EKF update step ------------------------------> TO DO
    [x,P]= UKF_update(x,P,z,RE,idf,lm); 
    
       
    end
    E=E+(xtrue-x).^2; %error in estimated pose
    % offline data store
    output= store_data(output, x, P, xtrue);
    Pd(:,1)=[0;0;0];
    det_P(1)=0;
    Pd(:,l)=diag(P);
    det_P(l)=det(P);
    l=l+1;
%  <-----------------------------------------------VISULAIZATION--------------------------------------------->
    
    % plots to get visualization
    xt= TransformToGlobal(veh,xtrue);
    xv= TransformToGlobal(veh,x);
    set(h.xt, 'xdata', xt(1,:), 'ydata', xt(2,:))
    set(h.xv, 'xdata', xv(1,:), 'ydata', xv(2,:))
    
    ptmp= make_covariance_ellipses(x,P);
    pcov(:,1:size(ptmp,2))= ptmp;
    %output.i
    if dtsum==0
        set(h.cov, 'xdata', pcov(1,:), 'ydata', pcov(2,:)) 
        pcount= pcount+1;
        if pcount == 15
            set(h.ptht, 'xdata', output.true(1,1:output.i), 'ydata', output.true(2,1:output.i))
            set(h.pth, 'xdata', output.path(1,1:output.i), 'ydata', output.path(2,1:output.i))
            pcount=0;
        end
        if ~isempty(z)
            plines= make_laser_lines (lm,x,idf);
            set(h.obs, 'xdata', plines(1,:), 'ydata', plines(2,:))
        end
    end
    drawnow
end

output= finalise_data(output);
set(h.ptht, 'xdata', output.true(1,:), 'ydata', output.true(2,:))   
set(h.pth, 'xdata', output.path(1,:), 'ydata', output.path(2,:))
save('output.mat', 'output');
figure;
plot(output.true(1,:),'-b')
hold on 
plot(output.path(1,:),'-r')
hold on 
plot(output.path(1,:)+3*sqrt(Pd(1,:)),'-g')
hold on 
plot(output.path(1,:)-3*sqrt(Pd(1,:)),'-y')
title('X-coordinates of pose with time')
legend('True pose_x','Estimated pose_x','mean+3sigma','mean-3sigma')
figure;
plot(output.true(2,:),'-b')
hold on 
plot(output.path(2,:),'-r')
hold on 
plot(output.path(2,:)+3*sqrt(Pd(2,:)),'-g')
hold on 
plot(output.path(2,:)-3*sqrt(Pd(2,:)),'-y')
title('Y-coordinates of pose with time')
legend('True pose_y','Estimated pose_y','mean+3sigma','mean-3sigma')
figure;
plot(output.true(3,:),'-b')
hold on 
plot(output.path(3,:),'-r')
hold on 
plot(output.path(3,:)+3*sqrt(Pd(3,:)),'-g')
hold on 
plot(output.path(3,:)-3*sqrt(Pd(3,:)),'-y')
title('theta-coordinates of pose with time')
legend('True pose_y','Estimated pose_y','mean+3sigma','mean-3sigma')
figure;
plot(det_P,'-b')
title('Determinant of covariance with time')
error=sqrt(E)
% 

%

%          <-----------------------------------------------HELPFUL FUNCTIONS ---------------------->

function h= setup_animations()
h.xt= patch(0,0,'b','erasemode','xor'); % vehicle true
h.ptht= plot(0,0,'g-','markersize',2); % vehicle path estimate

h.xv= patch(0,0,'r','erasemode','xor'); % vehicle estimate
h.pth= plot(0,0,'k-','markersize',2); % vehicle path estimate

h.obs= plot(0,0,'r','erasemode','xor'); % observations
h.cov= plot(0,0,'r','erasemode','xor'); % covariance ellipses

%
%

function p = make_laser_lines(lm,xv,idf)
% compute set of line segments for laser range-bearing measurements
if isempty(idf), p=[]; return, end
len= length(idf);
lnes(1,:)= zeros(1,len)+ xv(1);
lnes(2,:)= zeros(1,len)+ xv(2);
lnes(3:4,:)= lm(:,idf);
p= line_plot_conversion (lnes);

%
%

function p= make_covariance_ellipses(x,P)
% compute ellipses for plotting state covariances
N= 10;
inc= 2*pi/N;
phi= 0:inc:2*pi;

lenx= length(x);
lenf= (lenx-3)/2;
p= zeros (2,(lenf+1)*(N+2));

ii=1:N+2;
p(:,ii)= make_ellipse(x(1:2), P(1:2,1:2), 2, phi);

 
%
%

function p= make_ellipse(x,P,s, phi)
% make a single 2-D ellipse of s-sigmas over phi angle intervals 
r= sqrtm(P);
a= s*r*[cos(phi); sin(phi)];
p(2,:)= [a(2,:)+x(2) NaN];
p(1,:)= [a(1,:)+x(1) NaN];

%
%

function data= initialise_store(x,P, xtrue)
% offline storage initialisation
data.i=1;
data.path= x;
data.true= xtrue;
data.state(1).x= x;
data.state(1).P= diag(P);

%
%

function data= store_data(data, x, P, xtrue)
% add current data to offline storage
CHUNK= 5000;
if data.i == size(data.path,2) % grow array in chunks to amortise reallocation
    data.path= [data.path zeros(3,CHUNK)];
    data.true= [data.true zeros(3,CHUNK)];
end
i= data.i + 1;
data.i= i;
data.path(:,i)= x(1:3);
data.true(:,i)= xtrue;
%data.state(i).x= x;
data.state(i).P= diag(P);

%
%

function data= finalise_data(data)
% offline storage finalisation
data.path= data.path(:,1:data.i);
data.true= data.true(:,1:data.i);
