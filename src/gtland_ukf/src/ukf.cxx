/**********************************************************
************* Driver Program for UKF in GTLand ************
**********************************************************/

#include "matlab/UKF_main_c.h"
#include <geometry_msgs/Vector3.h>
#include <ros/ros.h>
#include <cmath>

double const dt = 1/24.5; // Frequency from the SITL

double x[6] = {0};
double z[3]; // u, v, r objects location in pixel frame 
double P[36];
double RE[36];
double QE[9] = {2*1e-5, 0, 0, 0, 1e-5, 0, 0, 0, 1e-5};

// Internal matrix for the camera
// From calib for my webcam
double cx = 339.639683;
double cy = 227.812694;
double f  = (757.324001 +  760.531946)/2;

// radius of the actual ball
const double R_actual = 0.03;

bool xinitialized = false;

ros::Publisher gimbal_asetp, current_pose, current_velo;
ros::Subscriber cam_sub;

// single callback contains all publishers
void cam_cb (const geometry_msgs::Vector3 &camera)
{
	z[1] = camera.x;
	z[2] = camera.y;
	z[0] = camera.z;

	// Initialize x with the with the first values from camera
	if (!xinitialized) {
		double dist = f*R_actual / z[0];
		double camz = dist * f / std::pow (std::pow (z[1] - cx, 2)
										 + std::pow (z[2] - cy, 2)
								  		 + std::pow (f, 2), 0.5);
		x[0] = (z[1] - cx) * camz / f;
		x[1] = (z[2] - cy) * camz / f;
		x[2] = camz;

		xinitialized = true;
	}

	// Call the UKF functions in the callback itself
	UKF_main_c (x, P, z, f, cx, cy, R_actual, QE, RE, dt);
	
	// Publish the current position and Velocity messages
	geometry_msgs::Vector3 position;
	geometry_msgs::Vector3 velocity;

	position.x = x[0];
	position.y = x[1];
	position.z = x[2];

	velocity.x = x[3];
	velocity.y = x[4];
	velocity.z = x[5];

	current_pose.publish (position);
	current_velo.publish (velocity);

	// Publish the desired gimbal angle
	// x -- yaw, y -- pitch, z -- roll
	geometry_msgs::Vector3 desired_angles;
	desired_angles.x = asin (x[0] / std::pow ((x[0]*x[0] + x[2]*x[2]), 0.5));
	desired_angles.y = asin (x[1] / std::pow ((x[1]*x[1] + x[2]*x[2]), 0.5));
	desired_angles.z = 0;

	gimbal_asetp.publish (desired_angles);

	ros::spinOnce ();
}

int main (int argc, char** argv)
{

	ros::init (argc, argv, "QUAD_UKF_node");
	ros::NodeHandle nh;

	gimbal_asetp = nh.advertise<geometry_msgs::Vector3>("/gimbal_asetp", 10);
	current_pose = nh.advertise<geometry_msgs::Vector3>("/current_position", 10);
	current_velo = nh.advertise<geometry_msgs::Vector3>("/current_velocity", 10);

	cam_sub = nh.subscribe ("/camera_pose", 100, cam_cb);

	ros::Rate loop_rate (100);

	// Initialization for RE and P as Identity matrices.
	for (int i = 0; i < 36; i=i+7) {
		RE[i] = 1e-10;
		P[i] = 0.01;
	}

	while (ros::ok()) {
		ros::spinOnce ();
		loop_rate.sleep ();
	}
	return 0;
}
