void SerialReadIMU() {

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
