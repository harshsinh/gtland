#include <ros/ros.h>
#include <iostream>
#include "CVInclude.h"
#include <vector>
#include <string>
#include <geometry_msgs/Vector3.h>

cv::VideoCapture cap;

int main (int argc, char ** argv)
{

    //ros node stuff
	ros::init (argc, argv, "detector1");
    ros::NodeHandle nh;
    ros::Publisher pub = nh.advertise<geometry_msgs::Vector3>("/camera_pose", 100);
	ros::Rate loop_rate (500);
    geometry_msgs::Vector3 pixelCord;

    // cv stuff
    cv::Mat frame;
    std::vector<std::vector<cv::Point> > contours;
    std::vector<cv::Vec4i> hierarchy;
    std::vector<double> area;
	
	//camera choice
	int camera = argv [1][0] - 48;

	if (camera < 0)
		ROS_ERROR_STREAM ("Not a valid camera address");

	cap.open (camera);

	cv::namedWindow ("color");
	cv::startWindowThread();
	int rl, bl, gl, rh, bh, gh;

	cv::createTrackbar ("Red Low", "color", &rl, 255);
	cv::createTrackbar ("Blue Low", "color", &bl, 255);
	cv::createTrackbar ("Green Low", "color", &gl, 255);
	cv::createTrackbar ("Red High", "color", &rh, 255);
	cv::createTrackbar ("Blue High", "color", &bh, 255);
	cv::createTrackbar ("Green High", "color", &gh, 255);
    
	if (!cap.isOpened()) {
		std::cout << "Unable to open camera "<< camera << std::endl;
		ROS_ERROR_STREAM ("Unable to open camera");
		return -1;
    }
    
    while (nh.ok()) {
        
        cap >> frame;
        if (!frame.empty()) {
        
            cv::Scalar colorLow (bl, gl, rl);
            cv::Scalar colorHigh (bh, gh, rh);
            //find marker
            cv::Mat resized, blurred, gray, hsv, thresh, ero, dila;
            cv::resize (frame, resized, cv::Size(), 0.50, 0.50, CV_INTER_LINEAR);
            cv::GaussianBlur (resized, blurred,  cv::Size(11, 11), 0, 0);
            cv::cvtColor (blurred, hsv, CV_BGR2HSV);
            cv::inRange (hsv, colorLow, colorHigh, thresh);
            cv::erode (thresh, ero, 0, cv::Point(-1, -1), 2);
            cv::dilate (ero, dila, 0, cv::Point(-1, -1), 2);

            // Find contours
            findContours( dila, contours, hierarchy, CV_RETR_TREE, 
                        CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0) );
            std::cout << "contours size : " << contours.size() << std::endl;
            if (contours.size() > 0) {
                std::cout << __PRETTY_FUNCTION__ << std::endl;
                for (int i = 0; i < contours.size(); ++i) {
                    double ar;
                    ar = cv::contourArea (contours[i]);
                    area.push_back (ar);
                }

                std::vector<double>::iterator max_area = std::max_element (area.begin(), area.end());
                int idx = std::distance (area.begin(), max_area);
    
                cv::Moments M = cv::moments (contours[idx]);
                cv::Point2f center, circenter;
                float radius;
                float& refrad = radius;
                cv::minEnclosingCircle ((cv::Mat)contours[idx], circenter, refrad);
                center = cv::Point2f( M.m10/M.m00 , M.m01/M.m00 );
    
                if (radius > 5.0) {
                    double dist = 3500.0/(int(radius));
                    cv::putText (frame, std::to_string(dist), circenter, cv::FONT_HERSHEY_SIMPLEX, 0.5, 0, 2);
                    cv::circle (frame, circenter, int(radius), cv::Scalar(0, 255, 255), 2, 8, 0);
                    pixelCord.x = int(circenter.x);
                    pixelCord.y = int(circenter.y);
                    pixelCord.z = int(dist);
                    pub.publish (pixelCord);
                }
                area.clear();
            }

            // show stuff
            cv::imshow ("original", frame);
            cv::imshow ("blurred", blurred);
            cv::imshow ("hsv", hsv);
            cv::imshow ("color", thresh);
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