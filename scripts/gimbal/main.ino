
#include <ros.h>
#include <Servo.h>
#include "gimbal.h"
#include <std_msgs/UInt32.h>
#include <geometry_msgs/Vector3.h>

Gimbal g;
Servo pitch_servo;
Servo roll_servo;

void cb (const geometry_msgs::Vector3&);

ros::NodeHandle nh;
ros::Subscriber<geometric_msgs::Vector3> sub("/yprdesired", &cb);
ros::Publisher pub = nh.advertise<geometry_msgs::Vector3>("/yprgimbal", 100);

geometry_msgs::Vector3 gimbal_ang_msg;

void setup ()
{

	pitch_servo.attach (g.pitch_servo_pin);
	roll_servo.attach (g.roll_servo_pin);

	Serial.begin(57600);

	pitch_servo.writeMicroseconds (g.pitch_pwm);
	roll_servo.writeMicroseconds (g.roll_pwm);

	nh.initNode();
	nh.subscribe (sub);

}

void loop ()
{

	if (Serial.available()) {

		SerialReadIMU();
		control_gimbal (millis()/1000.0);

		pitch_servo.writeMicroseconds (g.pitch_pwm);
		roll_servo.writeMicroseconds (g.roll_pwm);

		yprpub.publish (gimbal_ang_msg);
	}

	else Serial.println("#o1");
	delay (1);
}
