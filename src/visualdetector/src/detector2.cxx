#include <ros/ros.h>
#include <iostream>
#include "CVInclude.h"
#include <geometry_msgs/Vector3.h>

cv::VideoCapture cap;

int main (int argc, char** argv)
{

	int camera = argv [1][0] - 48;

	if (camera < 0) {
		ROS_ERROR_STREAM ("Not a valid camera address")
	}

	if (!cap.isOpened()) {
		std::cout << "Unable to open camera 0" << std::endl;
		ROS_ERROR_STREAM ("Unable to open camera 0");
		return -1;
	}

	ros::init (argc, argv, "detector2");
	ros::NodeHandle nh;
	image_transport::ImageTransport it (nh);

	image_transport::Publisher pub = it.advertise ("detected_image", 1);

	ros::Subscriber sub = nh.subscribe ("", 1000, msgCallback);

	return 0;
}
