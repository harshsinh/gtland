/**********************************************************
************** Arduino Code for Gimbal Control ************
**********************************************************/

#include <ros.h>
#include <Servo.h>
#include <std_msgs/UInt32.h>
#include <geometry_msgs/Vector3.h>

/*** Constants for the Gimbal ***/
# define yaw_max_len 7
# define yaw_min_len 5

# define max_pitch_PWM 2000
# define min_pitch_PWM 700
# define max_roll_PWM 1900
# define min_roll_PWM 1000

/*** PID max-min ***/
#define U_max 180v // deg/s
#define U_min -180 // deg/s

#define alpha 40 // deg/s to delta_pwm/s

/*** PID constants ***/
#define Kp_pitch 0.9
#define Ki_pitch 0.01
#define Kd_pitch 0.01

#define Kp_roll 0.9
#define Ki_roll 0.01
#define Kd_roll 0.01

#define Pitch_Servo_pin 8
#define Roll_Servo_pin 9

float yaw_IMU = 0;
float pitch_IMU = 0;
float roll_IMU = 0;

float roll_PWM = 1450;
float pitch_PWM = 1100;

float roll_des = 0;
float pitch_des = -0;

float error_pitch_prev = 0;
float error_roll_prev = 0;
float U_pitch_i_prev=0;
float U_roll_i_prev=0;
float prev_time = 0;

Servo Pitch_Servo;
Servo Roll_Servo;

geometry_msgs::Vector3 gimbal_ang_msg;

// Declaration for cb needs to be before that of subscriber
void cb (const std_msgs::UInt32&);

/*** ROS things ***/
ros::NodeHandle nh;
ros::Subscriber<geometric_msgs::Vector3> sub("/yprdesired", &cb);
ros::Publisher yprpub = nh.advertise<geometric_msgs::Vector3>("/ypr_gimbal", 100);


void cb (const std_msgs::UInt32& ypr) {

	float yawdesired = ypr.yaw;
	roll_des = ypr.roll;
	pitch_des = ypr.pitch;
}

void pitch_PID(float dt) {

	float error_pitch = pitch_des - pitch_IMU;
	float U_pitch_p = Kp_pitch * error_pitch;
	float U_pitch_i = U_pitch_i_prev + 0.5 * Ki_pitch * ( error_pitch + error_pitch_prev ) * dt;
	float U_pitch_d = Kd_pitch * ( error_pitch - error_pitch_prev ) / dt;
	if (U_pitch_i > U_max) U_pitch_i = U_max;
	else if (U_pitch_i < U_min) U_pitch_i = U_min;
	float U_pitch = U_pitch_p + U_pitch_i + U_pitch_d;
	if (U_pitch > U_max) U_pitch = U_max;
	else if (U_pitch < U_min) U_pitch = U_min;
	//Serial.println(U_pitch);
	U_pitch_i_prev = U_pitch_i;
	error_pitch_prev = error_pitch;
	if (abs(U_pitch)>4) pitch_PWM = pitch_PWM + alpha*U_pitch*dt;
}

void roll_PID(float dt) {

	float error_roll = roll_des - roll_IMU;
	float U_roll_p = Kp_roll * error_roll;
	float U_roll_i = U_roll_i_prev + 0.5 * Ki_roll * ( error_roll + error_roll_prev ) * dt;
	float U_roll_d = Kd_roll * ( error_roll - error_roll_prev ) / dt;
	if (U_roll_i > U_max) U_roll_i = U_max;
	else if (U_roll_i < U_min) U_roll_i = U_min;
	float U_roll = U_roll_p + U_roll_i + U_roll_d;
	if (U_roll > U_max) U_roll = U_max;
	else if (U_roll < U_min) U_roll = U_min;
	U_roll_i_prev = U_roll_i;
	error_roll_prev = error_roll;
	if (abs(U_roll)>4) roll_PWM = roll_PWM + alpha*U_roll*dt;
}

void controlGimbal() {

	float current_time=millis()/1000.0;
	float dt = current_time - prev_time;
	prev_time = current_time;
	pitch_PID(dt);
	roll_PID(dt);
	if (pitch_PWM > max_pitch_PWM) pitch_PWM = max_pitch_PWM;
	if (pitch_PWM < min_pitch_PWM) pitch_PWM = min_pitch_PWM;
	if (roll_PWM > max_roll_PWM) roll_PWM = max_roll_PWM;
	if (roll_PWM < min_roll_PWM) roll_PWM = min_roll_PWM;
	Pitch_Servo.writeMicroseconds(pitch_PWM);
	Roll_Servo.writeMicroseconds(roll_PWM);

}

SerialReadIMU() {

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
	yaw_IMU = str.toFloat();
	//}
	str = "";
	inchar = Serial.read();

	while (inchar != ',') {
		if (inchar != -1 && inchar != '#') str = str + inchar;
		inchar = Serial.read();
	}

	pitch_IMU = str.toFloat();
	str = "";

	inchar = Serial.read();

	while (inchar != ',') {
		if (inchar != -1 && inchar != '#') str = str + inchar;
		inchar = Serial.read();
	}

	roll_IMU = str.toFloat();
	//for inverted
	//  if (roll_IMU>0) roll_IMU=roll_IMU-180;
	//  else if (roll_IMU<0) roll_IMU=roll_IMU+180;
	//  pitch_IMU=-pitch_IMU;

	gimbal_ang_msg.x = roll_IMU;
	gimbal_ang_msg.y = pitch_IMU;
	gimbal_ang_msg.z = yaw_IMU;

	Serial.print(pitch_IMU);Serial.print('\t');
	Serial.print(roll_IMU);Serial.print('\n');
}

void setup() {


	Pitch_Servo.attach(Pitch_Servo_pin);
	Roll_Servo.attach(Roll_Servo_pin);
	Serial.begin(57600);
	Pitch_Servo.writeMicroseconds(pitch_PWM);
	Roll_Servo.writeMicroseconds(roll_PWM);
	nh.initNode();
	nh.subscribe (sub);
}

void loop() {

	if (Serial.available()) {
		SerialReadIMU();
		controlGimbal();
		yprpub.publish (gimbal_ang_msg);
	}

	else Serial.println("#o1");
	delay (1);
}
