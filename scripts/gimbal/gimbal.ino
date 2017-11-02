/**********************************************************
************** Arduino Code for Gimbal Control ************
**********************************************************/

/**********************************************************
Software Serial Rx, Tx -- 2, 3
Servo Pins Pitch, Roll -- 5, 10
ypr.x, ypr.y, ypr.z -- roll, pitch, yaw

ROS subscriber to : /yprdesired topic : The desired angle
setpoints for the gimbal.

ROS publisher on : /yprgimbal topic : The current angles as
percieved by the gimbal
**********************************************************/

#include <ros.h>
#include <AltSoftSerial.h>
//#include <Servo.h>
#include <geometry_msgs/Vector3.h>


#define Pitch_Servo_pin 6
#define Roll_Servo_pin 11
#define rxpin 2
#define txpin 3

/*** Constants for the Gimbal ***/
# define yaw_max_len 7
# define yaw_min_len 5

# define max_pitch_PWM 2000
# define min_pitch_PWM 700
# define max_roll_PWM 1900
# define min_roll_PWM 1000

# define max_pitch_PWM_ana 225
# define min_pitch_PWM_ana 50
# define max_roll_PWM_ana 220
# define min_roll_PWM_ana 120

/*** PID max-min ***/
#define U_max 180 // deg/s
#define U_min -180 // deg/s

#define alpha 40 // deg/s to delta_pwm/s

/*** PID constants ***/
#define Kp_pitch 0.9
#define Ki_pitch 0.01
#define Kd_pitch 0.01

#define Kp_roll 0.9
#define Ki_roll 0.01
#define Kd_roll 0.01

#define pitch_PWM_to_ana 0.3
#define roll_PWM_to_ana 0.15

float yaw_IMU = 0;
float pitch_IMU = 0;
float roll_IMU = 0;

float roll_PWM = 1450;
float pitch_PWM = 1100;

float roll_des = 0;
float pitch_des = 0;
float yaw_des = 0;

float error_pitch_prev = 0;
float error_roll_prev = 0;
float U_pitch_i_prev=0;
float U_roll_i_prev=0;
float prev_time = 0;

//Servo Pitch_Servo;
//Servo Roll_Servo;


geometry_msgs::Vector3 gimbal_ang_msg;
AltSoftSerial razor;

void cb (const geometry_msgs::Vector3& ypr) {

roll_des = ypr.x;
pitch_des = ypr.y;
yaw_des = ypr.z;
}

/*** ROS things ***/
ros::NodeHandle nh;
ros::Subscriber<geometry_msgs::Vector3> sub("/yprdesired", &cb);
ros::Publisher yprpub ("/yprgimbal", &gimbal_ang_msg);


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
	//if (pitch_PWM > max_pitch_PWM) pitch_PWM = max_pitch_PWM;
	//if (pitch_PWM < min_pitch_PWM) pitch_PWM = min_pitch_PWM;
	//if (roll_PWM > max_roll_PWM) roll_PWM = max_roll_PWM;
	//if (roll_PWM < min_roll_PWM) roll_PWM = min_roll_PWM;
	//Pitch_Servo.writeMicroseconds(pitch_PWM);
	//Roll_Servo.writeMicroseconds(roll_PWM);
 float pitch_PWM_ana = pitch_PWM_to_ana * pitch_PWM;
 float roll_PWM_ana = roll_PWM_to_ana * roll_PWM;
  if (pitch_PWM_ana > max_pitch_PWM_ana) pitch_PWM_ana = max_pitch_PWM_ana;
  if (pitch_PWM_ana < min_pitch_PWM_ana) pitch_PWM_ana = min_pitch_PWM_ana;
  if (roll_PWM_ana > max_roll_PWM_ana) roll_PWM_ana = max_roll_PWM_ana;
  if (roll_PWM_ana < min_roll_PWM_ana) roll_PWM_ana = min_roll_PWM_ana;
  analogWrite(Pitch_Servo_pin,pitch_PWM_ana);
  analogWrite(Roll_Servo_pin,roll_PWM_ana);

}

void SerialReadIMU() {

	char inchar = razor.read();

	while (inchar != '=') {
		inchar = razor.read();
	}

	String str = "";
	inchar = razor.read();

	while (inchar != ',') {
		if (inchar != -1 && inchar != '#')
		str = str + inchar;
		inchar = razor.read();
	}

	//if (str.length() <= yaw_max_len && str.length() >= yaw_min_len) {
	char aarray[str.length() + 1];
        str.toCharArray(aarray, sizeof(aarray));
        yaw_IMU = atof(aarray);
	//}
	str = "";
	inchar = razor.read();

	while (inchar != ',') {
		if (inchar != -1 && inchar != '#') str = str + inchar;
		inchar = razor.read();
	}

	char barray[str.length() + 1];
        str.toCharArray(barray, sizeof(barray));
	pitch_IMU = atof(barray);
	str = "";

	inchar = razor.read();

	while (inchar != ',') {
		if (inchar != -1 && inchar != '#') str = str + inchar;
		inchar = razor.read();
	}

      	char carray[str.length() + 1];
        str.toCharArray(carray, sizeof(carray));
	roll_IMU = atof(carray
);
	//for inverted
	//  if (roll_IMU>0) roll_IMU=roll_IMU-180;
	//  else if (roll_IMU<0) roll_IMU=roll_IMU+180;
	//  pitch_IMU=-pitch_IMU;

	gimbal_ang_msg.x = roll_IMU;
	gimbal_ang_msg.y = pitch_IMU;
	gimbal_ang_msg.z = yaw_IMU;

	//Serial.print(pitch_IMU);Serial.print('\t');
	//Serial.print(roll_IMU);Serial.print('\n');
}

void setup() {


        pinMode(rxpin, INPUT);
        pinMode(txpin, OUTPUT);
        
        Serial.begin(57600);
        razor.begin(57600);
	
        //Pitch_Servo.attach(Pitch_Servo_pin);
	      //Roll_Servo.attach(Roll_Servo_pin);
	      //Pitch_Servo.writeMicroseconds(pitch_PWM);
	      //Roll_Servo.writeMicroseconds(roll_PWM);
       pinMode(Pitch_Servo_pin,OUTPUT);
       pinMode(Roll_Servo_pin,OUTPUT);
       analogWrite(Pitch_Servo_pin,225);//50 to 225
       analogWrite(Roll_Servo_pin,170);//120 to 220
	
        nh.initNode();
        nh.advertise(yprpub);
	nh.subscribe (sub);
}

void loop() {
  
       //analogWrite(Pitch_Servo_pin,20);//10 to 220
       //analogWrite(Roll_Servo_pin,170);//120 to 220
       
	if (razor.available()) {
		SerialReadIMU();
		controlGimbal();
		yprpub.publish (&gimbal_ang_msg);
	}

	else {
              razor.print('#');
              razor.print('o');
              razor.print('1');
              razor.print('\n');
        }
	delay (1);
        nh.spinOnce();
}
