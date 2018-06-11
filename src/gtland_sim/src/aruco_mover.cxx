#include <ros/ros.h>
#include <gazebo_msgs/ModelState.h>

double SQUARE_SIZE = 10;

int main (int argc, char** argv)
{

    ros::init(argc, argv, "aruco_mover");
    ros::NodeHandle nh;

    ros::Publisher pub = nh.advertise<gazebo_msgs::ModelState>("/gazebo/set_model_state", 10);
    double freq = 100;
    ros::Rate rate(freq);

    gazebo_msgs::ModelState msg;
    /* speed = 1 m/s */
    double time = 0;
    double dist = 0;
    double dt = 1/freq;
    double dx = 0.001;

    while(nh.ok()) {

        /* move marker in a square of dimension stated above */
        msg.model_name = "ArucoMarker";
        msg.pose.position.x = (dist/SQUARE_SIZE) + (!(dist/SQUARE_SIZE))*SQUARE_SIZE;
        msg.pose.position.y = (dist/SQUARE_SIZE);
        msg.pose.position.z = 1;
        msg.pose.orientation.y = -0.707;
        msg.pose.orientation.w = 0.707;
        pub.publish(msg);
        time += dt;
        dist += dx;

        rate.sleep();
    }
    return 0;
}