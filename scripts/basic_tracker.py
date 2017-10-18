#!/usr/bin python
################################################################################
# Code for Object tracking
################################################################################
########## Imports
import argparse
import datetime
import glob
import os
import sys
from collections import deque
import cv2
import imutils
import numpy as np
import rospy
from geometry_msgs.msg import Vector3

################################################################################
########## Argument parser
ap = argparse.ArgumentParser()
ap.add_argument ("-v", "--video",  help = "path to the (optional) video file")
ap.add_argument ("-b", "--buffer", type = int, default = 64, help = "max buffer size")
ap.add_argument ("-c", "--calibration", help = "read the calibration file")
ap.add_argument ("-t", "--topic", help = "If topic is supplied subscribe to it")
args = vars(ap.parse_args())

################################################################################
########## ROS Things
pixel_cord = Vector3(0, 0, 0)
pub = rospy.Publisher('/camera_pose', Vector3, queue_size=100)
rospy.init_node('camera_node', anonymous=True)
rate = rospy.Rate(100)

################################################################################
########### Trackbar things
# placeholder function for trackbar draw
def nothing(x):
    pass

# The default values are for the postits
cv2.namedWindow('Color Range')
cv2.createTrackbar('Blue Low',  'Color Range', 12, 255,  nothing)
cv2.createTrackbar('Green Low', 'Color Range', 45, 255,  nothing)
cv2.createTrackbar('Red Low',   'Color Range', 100, 255, nothing)
cv2.createTrackbar('Blue High', 'Color Range', 41, 255,  nothing)
cv2.createTrackbar('Green High','Color Range', 202, 255, nothing)
cv2.createTrackbar('Red High',  'Color Range', 255, 255, nothing)

# Original Default color thresolds
color_low  = np.array([0, 0, 0])
color_high = np.array([250, 105, 50])
pts = deque(maxlen=args["buffer"])

################################################################################
########## Open video file is specified or open camera

if not args.get("video", False):
    camera = cv2.VideoCapture(0)

# otherwise, grab a reference to the video file
else:
    camera = cv2.VideoCapture(args["video"])
################################################################################
########## The main loop
while True:
    (grabbed, frame) = camera.read()

    # end of the video
    if args.get("video") and not grabbed:
        break

    frame   = imutils.resize(frame, width=600)
    blurred = cv2.GaussianBlur(frame, (11, 11), 0)
    hsv     = cv2.cvtColor(blurred, cv2.COLOR_BGR2HSV)

    # Blobs
    mask = cv2.inRange(hsv, color_low, color_high)
    mask = cv2.erode(mask, None, iterations=2)
    mask = cv2.dilate(mask, None, iterations=2)

    # Show trackbars and find their position
    # use them to set new threshold
    cv2.imshow("Color Range", mask)
    bl = cv2.getTrackbarPos("Blue Low",  "Color Range")
    gl = cv2.getTrackbarPos("Green Low", "Color Range")
    rl = cv2.getTrackbarPos("Red Low",   "Color Range")
    bh = cv2.getTrackbarPos("Blue High", "Color Range")
    gh = cv2.getTrackbarPos("Green High","Color Range")
    rh = cv2.getTrackbarPos("Red High",  "Color Range")
    color_low = np.array([bl, gl, rl])
    color_high= np.array([bh, gh, rh])

	# contouring and (x, y) center of the blob
    cnts = cv2.findContours(mask.copy(), cv2.RETR_EXTERNAL,
        cv2.CHAIN_APPROX_SIMPLE)[-2]
    center = None

	# only proceed if at least one contour was found
    if len(cnts) > 0:
        # find the largest contour in the mask, then use
        # it to compute the minimum enclosing circle and
        # centroid
        c = max(cnts, key=cv2.contourArea)
        ((x, y), radius) = cv2.minEnclosingCircle(c)
        M = cv2.moments(c)
        center = (int(M["m10"] / M["m00"]), int(M["m01"] / M["m00"]))
        # only proceed if the radius meets a minimum size
        if radius > 5:
            # draw the circle and centroid on the frame,
            # then update the list of tracked points
            dist = 3500.0/(int(radius))
            cv2.putText(frame, "distance : " + str(int(dist)),
                        (int(x - radius), int(y - radius)), cv2.FONT_HERSHEY_SIMPLEX, 0.5, 0, 2)
            cv2.circle(frame, (int(x), int(y)), int(radius), (0, 255, 255), 2)
            pixel_cord.x = int(x)
            pixel_cord.y = int(y)
            pixel_cord.z = int(dist)
            pub.publish(pixel_cord)
            cv2.circle(frame, center, 5, (0, 0, 255), -1)

	# update the points queue
    pts.appendleft(center)
    	# loop over the set of tracked points
    for i in xrange(1, len(pts)):
		# if either of the tracked points are None, ignore
		# them
		if pts[i - 1] is None or pts[i] is None:
			continue

		# draw the connecting lines
		thickness = int(np.sqrt(args["buffer"] / float(i + 1)) * 2.5)
		cv2.line(frame, pts[i - 1], pts[i], (0, 0, 255), thickness)

	# show the frame to our screen
    cv2.imshow("Frame", frame)
    key = cv2.waitKey(1) & 0xFF

    # if the 's' key is pressed, store the trackbar values.
    if key == ord('s'):
        f = open('../utils/trackbars/' + datetime.datetime.now().strftime("%Y%m%d%H%M"), 'w')
        f.write(str(bl) + ' ' + str(gl) + ' ' + str(rl) + ' ' + str(bh) + ' ' + str(gh) + ' ' + str(rh))
        f.close()
	# if the 'q' key is pressed, stop the loop
    if key == ord("q"):
        break

################################################################################
# cleanup the camera and close any open windows
camera.release()
cv2.destroyAllWindows()