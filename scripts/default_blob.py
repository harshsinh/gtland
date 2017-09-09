#!/usr/bin/python

# Imports
import os, sys, glob
import cv2
import numpy as np
import argparse
from collections import deque

# argument parser
ap = argparse.ArgumentParser()

ap.add_argument ("--cam", "--camera",
            help = "which camera to use", type = int, default = 0)
ap.add_argument ("-v", "--video",
            help = "path to the (optional) video file")
ap.add_argument ("-b", "--buffer", type = int, default = 64,
            help = "max buffer size")

args = vars (ap.parse_args ())

# Default Size of Image
defaultHeight = 640
defaultWidth  = 480

# Color of ball to be tracked
low  = (0, 0, 0)
high = (0, 255, 100)

# Trajectory srorage
traj = deque (maxlen = args["buffer"])

# Camera Or video source
if not args.get("cam", False):
    print (args["cam"])
    camera = cv2.VideoCapture (args["cam"])
else:
    camera = cv2.VideoCapture (args["video"])

# Loop over image
while (True):
    (isread, frame) = camera.read ()

# If video file has ended if no more frames.
    if args.get("video") and not isread:
        break

# Resize the image according to the default size
    height, width = frame.shape[:2]

    if (height * width >= defaultHeight * defaultWidth):
        frame = frame.resize (defaultHeight, defaultWidth, cv2.INTER_AREA)
    else:
        frame = frame.resize (defaultHeight, defaultWidth, cv2.INTER_LINEAR)

    hsv  = cv2.cvtColor (frame, cv2.COLOR_BGR2HSV)

    mask = cv2.inRange(hsv, low, high)
    mask = cv2.erode(mask, None, iterations=2)
    mask = cv2.dilate(mask, None, iterations=2)

    cv2.imshow("Frame", mask)
    key = cv2.waitKey(1) & 0xFF
    # if the 'q' key is pressed, stop the loop
    if key == ord("q"):
        break
    # cleanup the camera and close any open windows
camera.release()
cv2.destroyAllWindows()
