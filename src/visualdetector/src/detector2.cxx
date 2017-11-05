#include <ros/ros.h>
#include <iostream>
#include <string>
#include <vector>
#include "CVInclude.h"
#include <geometry_msgs/Vector3.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>

cv::VideoCapture cap;

int main (int argc, char** argv)
{
	//ros node stuff
	ros::init (argc, argv, "detector2");
	ros::NodeHandle nh;
	ros::Rate loop_rate (500);
	sensor_msgs::ImagePtr msg;
	image_transport::ImageTransport it(nh);
	image_transport::Publisher threshpub = it.advertise ("thresholded", 1);
	image_transport::Publisher detectpub = it.advertise ("detected", 1);

	// cv stuff
	cv::Mat frame;
	std::vector<std::vector<cv::Point> > contours;
	std::vector<cv::Vec4i> hierarchy;
	int th, sub;
	//camera choice
	int camera = argv [1][0] - 48;

	if (camera < 0)
		ROS_ERROR_STREAM ("Not a valid camera address");

	cap.open (camera);

	cv::namedWindow ("thresholded");
	cv::startWindowThread();
	cv::createTrackbar ("Threshold", "thresholded", &th, 20);
	cv::createTrackbar ("Threshold", "thresholded", &sub, 200);

	if (!cap.isOpened()) {
		std::cout << "Unable to open camera "<< camera << std::endl;
		ROS_ERROR_STREAM ("Unable to open camera");
		return -1;
	}
	
	while (nh.ok()) {

		cap >> frame;
		if (!frame.empty()) {

			//find marker
			cv::Mat thresh, gray, ero, dila, opening, closing;
			cv::cvtColor (frame, gray, CV_BGR2GRAY);
			cv::adaptiveThreshold (gray, thresh, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY, 7, 0);
			cv::erode (thresh, ero, 0, cv::Point(-1, -1), 2);
			cv::dilate (ero, dila, 0, cv::Point(-1, -1), 2);
			cv::morphologyEx (thresh, closing, cv::MORPH_CLOSE, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(2, 2), cv::Point(-1, -1)));
			cv::morphologyEx (closing, opening, cv::MORPH_OPEN, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(2, 2), cv::Point(-1, -1)));

			// show stuff
			cv::imshow ("original", frame);
			cv::imshow ("opened", opening);
			cv::imshow ("dilated-erroded", ero);

			threshpub.publish (cv_bridge::CvImage (std_msgs::Header(), "mono8", opening).toImageMsg());
			detectpub.publish (cv_bridge::CvImage (std_msgs::Header(), "bgr8", frame).toImageMsg());
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
