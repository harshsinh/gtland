#include <ros/ros.h>
#include "matlab/Cam_to_Global.h"
#include <geometry_msgs/Vector3.h>

geometry_msgs::Vector3 current_position_global;
geometry_msgs::Vector3 current_velocity_global;

double current_pos_cam[3];
double current_vel_cam[3];
double current_pos_global[3];
double current_vel_global[3];
double y,p,r;

void current_pos_callback(const geometry_msgs::Vector3 & msg)
{
	current_pos_cam[0]=msg.x;
	current_pos_cam[1]=msg.y;
	current_pos_cam[2]=msg.z;
}

void current_vel_callback(const geometry_msgs::Vector3 & msg)
{
        current_vel_cam[0]=msg.x;
        current_vel_cam[1]=msg.y;
        current_vel_cam[2]=msg.z;
}


void cam_imu(const geometry_msgs::Vector3 & msg)
{
        y=msg.x;
        p=msg.y;
        r=msg.z;
}

int main(int argc, char **argv)
{
	ros::init(argc,argv,"cam_to_global");
	ros::NodeHandle nh;

        ros::Publisher pos = nh.advertise<geometry_msgs::Vector3>("current_position_global", 100);
        ros::Publisher velo = nh.advertise<geometry_msgs::Vector3>("current_velocity_global", 100);
        ros::Subscriber sub_pos = nh.subscribe("current_position", 1000, current_pos_callback);
        ros::Subscriber sub_vel = nh.subscribe("current_velocity", 1000, current_vel_callback);
        ros::Subscriber sub_imu = nh.subscribe("cam_imu", 1000, cam_imu);

	ros::Rate loop_rate(100);
   	ros::spinOnce();
	
	int count = 1;

 	while (ros::ok()){

            Cam_to_Global(current_pos_cam, current_vel_cam, y, p, r, current_pos_global, current_vel_global);

            current_position_global.x = current_pos_global[0];
            current_position_global.y = current_pos_global[1];
            current_position_global.z = current_pos_global[2];

            current_velocity_global.x = current_vel_global[0];
            current_velocity_global.y = current_vel_global[1];
            current_velocity_global.z = current_vel_global[2];

            pos.publish(current_position_global);
            pos.publish(current_velocity_global);

            ros::spinOnce();
            count++;
            loop_rate.sleep();
   	}

	return 0;
}

