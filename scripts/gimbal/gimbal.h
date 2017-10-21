#include <math.h>

/*** Constants for the Gimbal ***/
# define yaw_max_len 7
# define yaw_min_len 5

# define MAX_PITCH_PWM 2000
# define MIN_PITCH_PWM 700
# define MAX_ROLL_PWM 1900
# define MIN_ROLL_PWM 1000

/*** PID max-min ***/
#define U_MAX 180 // deg/s
#define U_MIN -180 // deg/s

#define ALPHA 40 // deg/s to delta_pwm/s

/*** PID constants ***/
#define Kp_pitch 0.9
#define Ki_pitch 0.01
#define Kd_pitch 0.01

#define Kp_roll 0.9
#define Ki_roll 0.01
#define Kd_roll 0.01

class Gimbal {

	public:
		float yaw_imu = 0;
		float pitch_imu = 0;
		float roll_imu = 0;

		float roll_pwm = 1450;
		float pitch_pwm = 1100;

		float roll_des = 0;
		float pitch_des = 0;

		float error_pitch_prev = 0;
		float error_roll_prev = 0;
		float u_pitch_i_prev=0;
		float u_roll_i_prev=0;
		float prev_time = 0;

	public:
		void pitch_pid (float);
		void roll_pid (float);
		void control_gimbal (float);

};
