#include <ros/ros.h>
#include <iostream>
// #include "CVInclude.h"
#include <geometry_msgs/Vector3.h>
#include <cv_bridge/cv_bridge.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <image_transport/image_transport.h>

cv::VideoCapture cap;

int main (int argc, char** argv)
{
	//ros node stuff
	ros::init (argc, argv, "detector2");
	ros::NodeHandle nh;
	ros::Rate loop_rate (500);
	sensor_msgs::ImagePtr msg;
	image_transport::ImageTransport it (nh);
	image_transport::Publisher pub = it.advertise ("detected_image", 1);

	// cv stuff
	cv::Mat frame;
	
	//camera choice
	int camera = argv [1][0] - 48;

	if (camera < 0)
		ROS_ERROR_STREAM ("Not a valid camera address");

	cap.open (camera);

	cv::namedWindow ("thresholded");
	cv::startWindowThread();
	int th;
	cv::createTrackbar ("Threshold", "thresholded", &th, 255);

	if (!cap.isOpened()) {
		std::cout << "Unable to open camera "<< camera << std::endl;
		ROS_ERROR_STREAM ("Unable to open camera");
		return -1;
	}
	
	while (nh.ok()) {

		cap >> frame;
		if (!frame.empty()) {

			//find marker
			cv::Mat resized, blurred, thresholded, gray, dilated;
			cv::cvtColor (frame, gray, CV_BGR2GRAY);
			cv::resize (gray, resized, cv::Size(), 0.50, 0.50, CV_INTER_LINEAR);
			cv::GaussianBlur (resized, blurred,  cv::Size(11, 11), 0, 0);
			cv::threshold (blurred, thresholded, th, 255, CV_THRESH_BINARY);

			// show stuff
			cv::imshow ("original", frame);
			cv::imshow ("resized", resized);
			cv::imshow ("blurred", blurred);
			cv::imshow ("thresholded", thresholded);
			// stop if "q" is pressed
			if (cv::waitKey(1) == 113)
				break;
		}

		else {
			ROS_ERROR_STREAM ("No Image found!");
			return -1;
		}

		ros::spinOnce();
		loop_rate.sleep();

	}
	return 0;
}
