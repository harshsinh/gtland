#!/usr/bin/python

# Imports
import os, sys, glob
import cv2
import numpy as np

# Image as argument
frame = str(sys.argv)

# Read Image
im = cv2.imread (frame, cv2.IMREAD_GRAYSCALE)
