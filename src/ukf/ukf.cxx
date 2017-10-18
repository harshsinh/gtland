/**********************************************************
************* Driver Program for UKF in GTLand ************
**********************************************************/

#include <update/UKF_update.h>
#include <predict/UKF_predict.h>
#include <geometry_msgs/Vector3.h>
#include <ros.h>
#include <ypr.h>

double const dt = 1/24.5; // Frequency from the SITL

double x[6] = {0};
double z[3]; // u, v, r objects location in pixel frame 
double P[36];
const double RE[36];
const double QE[9] = [2*1e-5, 0, 0, 0, 1e-5, 0, 0, 0, 1e-5];

// Internal matrix for the camera
// From calib for my webcam
double cx = 339.639683;
double cy = 227.812694;
double f  = (757.324001 +  760.531946)/2;

// TODO: Get Idea what R_actual is
const double R_actual = 0.0212;

bool xinitialized = false;

ros::NodeHandle nh;
ros::Publisher gimbal_ang, current_pose, current_velo;
ros::Subscriber cam_sub;

// Main callback function, all the publishers work here.
void cam_cb (geometry_msgs::Twist &camera)
{
	z[1] = camera.linear.x;
	z[2] = camera.linear.y;
	z[0] = camera.linear.z;

	// Initialize x with the with the first values from camera
	if (!xinitialized) {
		double dist = f*R_actual / z[0];
		double camz = dist * f / ((z[1] - cx)^2 + (z[2] - cy)^2 + f*f)^0.5;
		x[0] = (z[1] - cx) * camz / f;
		x[1] = (z[2] - cy) * camz / f;
		x[2] = camz;

		xinitialized = true;
	}

	// Call the UKF functions in the callback itself
	UKF_predict (x, P, dt, RE);
	UKF_update (x, P, z, QE, R_actual, f, cx, cy);

	// Publish the current position and Velocity messages
	geometry_msgs::Vector3 position;
	geometry_msgs::Vector3 velocity;

	position.x = x[1];
	position.y = x[2];
	position.z = x[3];

	velocity.x = x[4];
	velocity.y = x[5];
	velocity.z = x[6];

	current_pose.publish (position);
	current_velo.publish (velocity);

	// Publish the desired gimbal angles
	// TODO: Correct the calculations for Yaw Pitch and Roll desired
	yaw_desired = 0;
	pitch_desired = 0;
	roll_desired = 0;

	ypr desired_angles;
	desired_angles.yaw = yaw_desired;
	desired_angles.pitch = pitch_desired;
	desired_angles.roll = roll_desired;
}

int main (int argc, char** argv)
{

	ros::init (argc, argv, "QUAD_UKF_node");

	gimbal_asetp = nh.advertise<gtland::ypr>("/gimbal_asetp", 10);
	current_pose = nh.advertise<geometry_msgs::Vector3>("/current_position", 10);
	current_velo = nh.advertise<geometry_msgs::Vector3>("/current_velocity", 10);

	cam_sub = nh.Subscribe ("/camera_pose", 100, cam_cb);

	ros::Rate loop_rate (100);

	// Initialization for RE and P as Identity matrices.
	for (int i = 0; i < 36; i=i+7) {
		RE[i] = 1e-10;
		P[i] = 0.01;
	}

	while (ros::ok()) {
		ros::SpinOnce ();
		loop_rate.sleep ();
	}
	return 0;
}
