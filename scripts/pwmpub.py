#####################################
###  PWM Publish on three chanels ###
#####################################

#!/usr/bin/ python
import rospy
from std_msgs.msg import UInt32

yaw = 10;
pitch = 14;
roll = 19;

def ypr_():
    pub = rospy.Publisher('/ypr', UInt32, queue_size=10)
    rospy.init_node('pwmpublisher', anonymous=True)
    rate = rospy.Rate(100)
    
    while not rospy.is_shutdown():
        ypr = (yaw << 16) + (pitch << 8) + (roll)
        pub.publish(ypr)
        rate.sleep()

if __name__ == '__main__':
    try:
        ypr_()
    except rospy.ROSInterruptException:
        pass
