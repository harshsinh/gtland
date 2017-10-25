#include <ros/ros.h>
#include <iostream>
#include <cv_bridge/cv_bridge.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <image_transport/image_transport.h>

#include <geometry_msgs/Vector3.h>

int main (int argc, char** argv)
{

	VideoCapture cap(0);

	if (!cap.isOpened()) {
		std::cout << "Unable to open camera 0" << std::endl;
		return -1;
	}


	return 0;
}
