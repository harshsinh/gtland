#include <ros.h>
#include <std_msgs/UInt32.h>

long yawmask = 16711680;
long pitchmask = 65280;
long rollmask = 255;

int yawpwm = 0;
int pitchpwm = 0;
int rollpwm = 0;

int ypin = 2;
int ppin = 3;
int rpin = 4;

ros::NodeHandle nh;

void cb(const std_msgs::UInt32& ypr) {
    yawpwm      = ypr.data & yawmask;
    pitchpwm    = ypr.data & pitchmask;
    rollpwm     = ypr.data & rollmask;

    analogWrite(ypin, yawpwm);
    analogWrite(ppin, pitchpwm);
    analogWrite(rpin, rollpwm);

    Serial.print("ypr : ");
    Serial.print(yawpwm);
    Serial.print(pitchpwm);
    Serial.println(rollpwm);
}

ros::Subscriber<std_msgs::UInt32> sub("ypr", &cb);

void setup() {
    Serial.begin(57600);
    pinMode(ypin, OUTPUT);
    pinMode(ppin, OUTPUT);
    pinMode(rpin, OUTPUT);
    nh.initNode();
    nh.subscribe(sub);
}

void loop() {
    nh.spinOnce();
    delay(1);
}
