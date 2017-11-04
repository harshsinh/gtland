#!/usr/bin python
################################################################################
# Code for Object tracking Detector 2
################################################################################
########## Imports
import glob
import os
import sys
import cv2
import imutils
import numpy as np

def nada(x):
    pass

cv2.namedWindow('Normal Threshold')
cv2.createTrackbar('Threshold', 'Normal Threshold', 10, 255, nada)

camera = cv2.VideoCapture(0)

while True:
    
    # Image blurred so as to seperate anybimodal dist for otsu
    # Image grayscaled for adaptive threshold
    (grabbed, frame) = camera.read()
    resized   = imutils.resize(frame, width=600)
    gray    = cv2.cvtColor(resized, cv2.COLOR_BGR2GRAY)
    blurred = cv2.GaussianBlur(gray, (11, 11), 0)
    th = cv2.getTrackbarPos("Threshold", "Normal Threshold")

    ratio = frame.shape[0] / float(resized.shape[0])

    # different thresholdings done
    # TODO: find the best suited one
    retst, binstan = cv2.threshold(blurred, th, 255, cv2.THRESH_BINARY)
    binadap = cv2.adaptiveThreshold(blurred, 255, cv2.ADAPTIVE_THRESH_GAUSSIAN_C, cv2.THRESH_BINARY, 11, 2)
    retot, binotsu = cv2.threshold(blurred,0,255, cv2.THRESH_BINARY+cv2.THRESH_OTSU)
    
    #show images and binarized ones for comparision
    cv2.imshow("Original", blurred)
    cv2.imshow("Otsu Binarized", binotsu)
    cv2.imshow("Normal Threshold", binstan)
    cv2.imshow("Adaptive Threshold", binadap)

    # finding contours
    adapcnts = cv2.findContours(binadap.copy(), cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)[-2]
    stancnts = cv2.findContours(binstan.copy(), cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)[-2]
    otsucnts = cv2.findContours(binotsu.copy(), cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)[-2]

    # Looping over the countors
    for c in stancnts:
        peri = cv2.arcLength(c, True)
        approx = cv2.approxPolyDP(c, 0.04 * peri, True)

        # ctr = np.array(c).reshape((-1,1,2)).astype(np.int32)
        ctr = c.astype("float")
        ctr = ctr*ratio
        ctr = ctr.astype("int")

        if len(approx) == 4:
            print("rectangle found !!")
            cv2.drawContours(resized, c, -1, (0, 0, 255), 2)            
    
    key = cv2.waitKey(1) & 0xFF
    if key == ord("q"):
        break

camera.release()
cv2.destroyAllWindows()
