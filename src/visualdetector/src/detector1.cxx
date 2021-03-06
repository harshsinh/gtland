#include <ros/ros.h>
#include <iostream>
#include "CVInclude.h"
#include <vector>
#include <string>
#include <geometry_msgs/Vector3.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>

cv::VideoCapture cap;
cv::Mat frame;

// Frame from topic subscription
void image_cb (const sensor_msgs::ImageConstPtr& msg)
{
  frame = cv_bridge::toCvShare (msg) -> image;
  return;
}

int main (int argc, char ** argv)
{

    //ros node stuff
	ros::init (argc, argv, "detector1");
    ros::NodeHandle nh;
	image_transport::ImageTransport it(nh);
	image_transport::Publisher threshpub = it.advertise ("thresholded", 1);
	image_transport::Publisher detectpub = it.advertise ("detected", 1);
    ros::Publisher pub = nh.advertise<geometry_msgs::Vector3>("/camera_pose", 100);
    image_transport::Subscriber sub = it.subscribe ("/image", 100, image_cb);
	ros::Rate loop_rate (500);
    geometry_msgs::Vector3 pixelCord;

    // cv stuff
    std::vector<std::vector<cv::Point> > contours;
    std::vector<cv::Vec4i> hierarchy;
    std::vector<double> area;

    // scaling factors
    double imScale = 1.00;

	cv::namedWindow ("color");
	cv::startWindowThread();
	int rl, bl, gl, rh, bh, gh;

	cv::createTrackbar ("Red Low", "color", &rl, 255);
	cv::createTrackbar ("Blue Low", "color", &bl, 255);
	cv::createTrackbar ("Green Low", "color", &gl, 255);
	cv::createTrackbar ("Red High", "color", &rh, 255);
	cv::createTrackbar ("Blue High", "color", &bh, 255);
	cv::createTrackbar ("Green High", "color", &gh, 255);



	//camera choice
	int camera = argv [1][0] - 48;

	if (camera >= 0)
    {
	    cap.open (camera);

	    if (!cap.isOpened()) {
	    	std::cout << "Unable to open camera "<< camera << std::endl;
	    	ROS_ERROR_STREAM ("Unable to open camera");
	    	return -1;
        }
    }

    while (nh.ok()) {

        if (cap.isOpened())
            cap >> frame;

        if (!frame.empty()) {

            cv::Scalar colorLow (bl, gl, rl);
            cv::Scalar colorHigh (bh, gh, rh);
            //find marker
            cv::Mat resized, blurred, gray, hsv, thresh, ero, dila;
            cv::resize (frame, resized, cv::Size(), imScale, imScale, CV_INTER_LINEAR);
            cv::GaussianBlur (resized, blurred,  cv::Size(11, 11), 0, 0);
            cv::cvtColor (blurred, hsv, CV_BGR2HSV);
            cv::inRange (hsv, colorLow, colorHigh, thresh);
            cv::erode (thresh, ero, 0, cv::Point(-1, -1), 2);
            cv::dilate (ero, dila, 0, cv::Point(-1, -1), 2);

            // Find contours
            findContours( dila, contours, hierarchy, CV_RETR_TREE, 
                        CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0) );

            if (contours.size() > 0) {
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
                // center = cv::Point2f( M.m10/M.m00 , M.m01/M.m00 );
                center = cv::Point2f(2, 2);

                std::cout << "radius : " << int(radius) << std::endl;

                if (radius > 2.0 && radius < 225) {
                    // cv::putText (frame, std::to_string(radius), circenter/imScale, cv::FONT_HERSHEY_SIMPLEX, imScale, 0, 2);
                    // cv::circle (frame, circenter/imScale, int(radius/imScale), cv::Scalar(0, 255, 255), 2, 8, 0);
                    // pixelCord.x = int(circenter.x/imScale);
                    // pixelCord.y = int(circenter.y/imScale);
                    // pixelCord.z = int(radius/imScale);
                    // pub.publish (pixelCord);
                }
                area.clear();
            }

            // show stuff
            cv::imshow ("original", frame);
            cv::imshow ("hsv", hsv);
            cv::imshow ("color", thresh);
			// publish images
			threshpub.publish (cv_bridge::CvImage (std_msgs::Header(), "mono8", thresh).toImageMsg());
			detectpub.publish (cv_bridge::CvImage (std_msgs::Header(), "bgr8", frame).toImageMsg());
            // stop if "q" is pressed
            if (cv::waitKey(1) == 113)
            break;
        }

        ros::spinOnce();
        loop_rate.sleep();

    }
    return 0;
}
