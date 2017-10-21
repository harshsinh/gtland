#include <ros.h>
#include <geometry_msgs/Vector3.h>
geometry_msgs::Vector3 serial_read_imu()
{

	geometry_msgs::Vector3 gimbal_ang_msg;

	char inchar = Serial.read();

	while (inchar != '=') {
		inchar = Serial.read();
	}

	String str = "";
	inchar = Serial.read();

	while (inchar != ',') {
		if (inchar != -1 && inchar != '#')
		str = str + inchar;
		inchar = Serial.read();
	}

	//if (str.length() <= yaw_max_len && str.length() >= yaw_min_len) {
        g.yaw_imu = str.toFloat();
	//}
	str = "";
	inchar = Serial.read();

	while (inchar != ',') {
		if (inchar != -1 && inchar != '#') str = str + inchar;
		inchar = Serial.read();
	}

	g.pitch_imu = str.toFloat();
	str = "";

	inchar = Serial.read();

	while (inchar != ',') {
		if (inchar != -1 && inchar != '#') str = str + inchar;
		inchar = Serial.read();
	}

	g.roll_imu = str.toFloat();
	//for inverted
	//  if (roll_IMU>0) roll_IMU=roll_IMU-180;
	//  else if (roll_IMU<0) roll_IMU=roll_IMU+180;
	//  pitch_IMU=-pitch_IMU;

	gimbal_ang_msg.x = g.roll_imu;
	gimbal_ang_msg.y = g.pitch_imu;
	gimbal_ang_msg.z = g.yaw_imu;

	return gimbal_ang_msg;
}
