#########################################################
### Program for testing SLIC for detection of objects ###
#########################################################

### Imports
import cv2
import argparse
import numpy as np
from skimage import io
from skimage.segmentation import slic
from skimage import img_as_float
from skimage.exposure import rescale_intensity

#########################################################
### Will operate on the image using mask
def operate_on_segment(image, mask):
    (B, G, R) = cv2.split (image.astype("float"))
    R = np.ma.masked_array (R, mask = mask)
    B = np.ma.masked_array (B, mask = mask)
    G = np.ma.masked_array (G, mask = mask)

    rg = np.absolute (R - G)
    yb = np.absolute (0.5 * (R + G) - B)
    stdRoot = np.sqrt ((rg.std() ** 2) + (yb.std() ** 2))
    meanRoot = np.sqrt ((rg.mean() ** 2) + (yb.mean() ** 2))

    return stdRoot + (0.3 * meanRoot)

#########################################################
### Argument parser
ap = argparse.ArgumentParser()
ap.add_argument ("-i", "--image", required = "True",
    help="Path to input image file")
ap.add_argument ("-n", "--numsegments", type=int, default=100,
    help="Number of segments for SLIC")
args = vars(ap.parse_args())

#########################################################
### Loading Images and doing some shit.
cvimage = cv2.imread (args["image"])
skimage = io.imread (args["image"])

visimage = np.zeros (cvimage.shape[:2], dtype="float")
segments = slic (img_as_float (skimage), n_segments = args["numsegments"],
        slic_zero = True)

#########################################################
### Loop over all segments
for seg in np.unique(segments):
    mask = np.ones (cvimage.shape[:2])
    mask[segments == seg] = False
    out = operate_on_segment (cvimage, mask)
    visimage[segments == seg] = out

#########################################################
### Displaying shit
visimage = rescale_intensity (visimage, out_range=(0, 255)).astype("uint8")
alpha = 0.6
overlay = np.dstack ([visimage]*3)
outputimage = cvimage.copy()
cv2.addWeighted (overlay, alpha, outputimage, 1 - alpha, 0, outputimage)
cv2.imshow("input", cvimage)
cv2.imshow("visualization", visimage)
cv2.imshow("output", outputimage)
cv2.waitKey(0)
