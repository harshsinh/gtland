#include <ros/ros.h>
#include <iostream>
#include "CVInclude.h"
#include <geometry_msgs/Vector3.h>

cv::VideoCapture cap;

int main (int argc, char** argv)
{
	//ros node stuff
	ros::init (argc, argv, "detector2");
	ros::NodeHandle nh;
	ros::Rate loop_rate (50);

	image_transport::ImageTransport it (nh);
	image_transport::Publisher pub = it.advertise ("detected_image", 1);

	//camera choice
	int camera = argv [1][0] - 48;

	if (camera < 0)
		ROS_ERROR_STREAM ("Not a valid camera address")

	cap.open (camera);

	cv::namedWindow ("original");
	cv::startWindowThread();

	if (!cap.isOpened()) {
		std::cout << "Unable to open camera "<< camera << std::endl;
		ROS_ERROR_STREAM ("Unable to open camera");
		return -1;
	}

	cv::Mat frame;
	sensor_msgs::ImagePtr msg;

	return 0;
}
