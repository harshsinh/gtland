#include <ros/ros.h>
#include <iostream>
#include <string>
#include <vector>
#include "CVInclude.h"
#include <geometry_msgs/Vector3.h>

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
	std::vector<std::vector<cv::Point> > contours;
	std::vector<cv::Vec4i> hierarchy;
	
	//camera choice
	int camera = argv [1][0] - 48;

	if (camera < 0)
		ROS_ERROR_STREAM ("Not a valid camera address");

	cap.open (camera);

	cv::namedWindow ("thresholded");
	cv::startWindowThread();
	int th, rl, bl, gl, rh, bh, gh;
	cv::createTrackbar ("Threshold", "thresholded", &th, 255);

	cv::createTrackbar ("Red Low", "color", &rl, 255);
	cv::createTrackbar ("Blue Low", "color", &bl, 255);
	cv::createTrackbar ("Green Low", "color", &gl, 255);
	cv::createTrackbar ("Red High", "color", &rh, 255);
	cv::createTrackbar ("Blue High", "color", &bh, 255);
	cv::createTrackbar ("Green High", "color", &gh, 255);

	cv::Scalar colorLow (bl, gl, rl);
	cv::Scalar colorHigh (bh, gh, rh);

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
