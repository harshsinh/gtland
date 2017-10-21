#include "gimbal.h"

void Gimbal::pitch_pid (float dt)
{

	float error_pitch = pitch_des - pitch_imu;
	float u_pitch_p = Kp_pitch * error_pitch;
	float u_pitch_i = u_pitch_i_prev + 0.5 * Ki_pitch * ( error_pitch + error_pitch_prev ) * dt;
	float u_pitch_d = Kd_pitch * ( error_pitch - error_pitch_prev ) / dt;
	if (u_pitch_i > U_MAX)
		u_pitch_i = U_MAX;

	else if (u_pitch_i < U_MIN)
		u_pitch_i = U_MIN;

	float u_pitch = u_pitch_p + u_pitch_i + u_pitch_d;

	if (u_pitch > U_MAX)
		u_pitch = U_MAX;

	else if (u_pitch < U_MIN)
		u_pitch = U_MIN;

	u_pitch_i_prev = u_pitch_i;
	error_pitch_prev = error_pitch;

	if (std::abs(u_pitch)>4)
		pitch_pwm = pitch_pwm + ALPHA*u_pitch*dt;
}

void Gimbal::roll_pid (float dt)
{

	float error_roll = roll_des - roll_imu;
	float u_roll_p = Kp_roll * error_roll;
	float u_roll_i = u_roll_i_prev + 0.5 * Ki_roll * ( error_roll + error_roll_prev ) * dt;
	float u_roll_d = Kd_roll * ( error_roll - error_roll_prev ) / dt;

	if (u_roll_i > U_MAX)
		u_roll_i = U_MAX;

	else if (u_roll_i < U_MIN)
		u_roll_i = U_MIN;

	float u_roll = u_roll_p + u_roll_i + u_roll_d;

	if (u_roll > U_MAX)
		u_roll = U_MAX;

	else if (u_roll < U_MIN)
		u_roll = U_MIN;

	u_roll_i_prev = u_roll_i;
	error_roll_prev = error_roll;

	if (std::abs(u_roll)>4)
		roll_pwm = roll_pwm + ALPHA*u_roll*dt;

}

void Gimbal::control_gimbal (float current_time)
{
	float dt = current_time - prev_time;
	prev_time = current_time;
	pitch_pid(dt);
	roll_pid(dt);

	if (pitch_pwm > MAX_PITCH_PWM)
		pitch_pwm = MAX_PITCH_PWM;

	if (pitch_pwm < MIN_PITCH_PWM)
		pitch_pwm = MIN_PITCH_PWM;

	if (roll_pwm > MAX_ROLL_PWM)
		roll_pwm = MAX_ROLL_PWM;

	if (roll_pwm < MIN_ROLL_PWM)
		roll_pwm = MIN_ROLL_PWM;

}
