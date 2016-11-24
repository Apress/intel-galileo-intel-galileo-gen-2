#import numpy as np
import cv2
import cv
import sys

cap = cv2.VideoCapture(-1)

w, h = 960, 544
cap.set(cv.CV_CAP_PROP_FRAME_WIDTH, w)
cap.set(cv.CV_CAP_PROP_FRAME_HEIGHT, h)

if not cap.isOpened():
    print "Webcam could not be opened successfully"
    sys.exit(-1)
else:
    print "Webcam is OK! I found it!"

ret, frame = cap.read()
cv2.imwrite('pythontest.jpg', frame)

cap.release()

