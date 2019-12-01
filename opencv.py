import cv2
import numpy as np 
import datetime

cap = cv2.VideoCapture('nameofthevideo.mp4')

while True:
    ret, frame = cap.read()

    if ret == True:
        font = cv2.FONT_HERSERY_SIMPLEX
        datet = str(datetime.datetime.now())
        #frame =  cv2.sputText(frame,text,location,font,font size,font color, font weight, line)
        frame =  cv2.putText(frame,datet,(10,50),font,1 ,(0,255,255), 2, cv2.LINE_AA)
        cv2.imshow("Frame",frame)

        if cv2.waitkey(25) & 0xFF == ord("q"):
            break
    
    else:
        break

cap.release()

cv2.destroyAllWindows()


#!/usr/bin/env python 
from __future__ import division

import tensorflow as tf
model = __import__("model")
from cv2 import cv2
import sys
import os
import time
import math
import numpy as np
from math import sin , cos


# Radian <-> Degree conversion functions
def deg2rad(deg):
	return deg * math.pi / 180.0
def rad2deg(rad):
    return 180.0 * rad / math.pi

#Get and set the number of cores to be used by TensorFlow
if(len(sys.argv) > 1):
	NCPU = int(sys.argv[1])
else:
	NCPU = 1
config = tf.ConfigProto(intra_op_parallelism_threads=NCPU, \
                        inter_op_parallelism_threads=NCPU, \
                        allow_soft_placement=True, \
                        device_count = {'CPU': 1})

#The max number of frames to be processed, 
#    and the number of frames already processed
NFRAMES = 1000
curFrame = 0

#Periodic task options
period = 50
is_periodic = True

#Load the model
sess = tf.InteractiveSession(config=config)
saver = tf.train.Saver()
model_load_path = "model/model.ckpt"
saver.restore(sess, model_load_path)

#Create lists for tracking operation timings
cap_time_list = []
prep_time_list = []
pred_time_list = []
tot_time_list = []

print('---------- Processing video for epoch 1 ----------')

#Open the video file
vid_path = 'epoch-1.avi'
assert os.path.isfile(vid_path)
cap = cv2.VideoCapture(vid_path)


#Process the video while recording the operation execution times
print('performing inference...')
time_start = time.time()
first_frame = True
while cap.isOpened():
	if curFrame < NFRAMES:
		cam_start = time.time()
		ret, img = cap.read()


		if cv2.waitKey(1) & 0xFF == ord('q'):
			break

		prep_start = time.time()
		frame = img
		#Preprocess the input frame
		img = cv2.resize(img, (200, 66))
		img = img / 255.

		pred_start = time.time()	
		#Feed the frame to the model and get the control output
		rad = model.y.eval(feed_dict={model.x: [img]})[0][0]
		deg = rad2deg(rad)
		pred_end   = time.time()

		#Calculate the timings for each step
		cam_time  = (prep_start - cam_start)*1000
		prep_time = (pred_start - prep_start)*1000
		pred_time = (pred_end - pred_start)*1000
		tot_time  = (pred_end - cam_start)*1000

		font = cv2.FONT_HERSHEY_SIMPLEX
		currDeg = "Pred Deg: " + str(deg)
		# if (deg < 0):
		yCor = 200-( 80 + 120*(sin(math.radians(90-deg))) ) +80
		xCor = 160 + 120*(cos(math.radians(90-deg)))
		# else: 
		# 	xCor = 80 + 120*(sin(math.radians(deg)))
		# 	yCor = 160 + 120*(cos(math.radians(deg)))
		calPoint = (int(xCor),int(yCor))
		frame = cv2.line(frame, pt1=(160, 80), pt2=(160, 200), color=(255, 255, 255), thickness=3, lineType=9, shift=0)
		frame = cv2.line(frame, pt1=calPoint, pt2=(160, 200), color=(0,255,0),thickness=3, lineType=9, shift=0)
		frame =  cv2.putText(frame,currDeg,(5,20),font,.5 ,(0,255,255), 2, cv2.LINE_AA)
		cv2.imshow('frame',frame)

		print(f"Degree: {deg} | Coordinate x : {int(xCor)} y : {int(yCor)}")
		# print('pred: {:0.2f} deg. took: {:0.2f} ms | cam={:0.2f} prep={:0.2f} pred={:0.2f}'.format(deg, tot_time, cam_time, prep_time, pred_time))
		
		#Don't include the timings for the first frame due to cache warmup
		if first_frame:
			first_frame = False
		else:
			tot_time_list.append(tot_time)
			curFrame += 1
        
		#Wait for next period
		wait_time = (period - tot_time) / 1000
		if is_periodic and wait_time > 0:
			time.sleep(wait_time)
	else:
		break
	
cap.release()
cv2.destroyAllWindows()
#Calculate and output FPS/frequency
fps = curFrame / (time.time() - time_start)
print('completed inference, total frames: {}, average fps: {} Hz'.format(curFrame+1, round(fps, 1)))

# #Calculate and display statistics of the total inferencing times
# print("count: {}".format(len(tot_time_list)))
# print("mean: {}".format(np.mean(tot_time_list)))
# print("max: {}".format(np.max(tot_time_list)))
# print("99.999pct: {}".format(np.percentile(tot_time_list, 99.999)))
# print("99.99pct: {}".format(np.percentile(tot_time_list, 99.99)))
# print("99.9pct: {}".format(np.percentile(tot_time_list, 99.9)))
# print("99pct: {}".format(np.percentile(tot_time_list, 99)))
# print("min: {}".format(np.min(tot_time_list)))
# print("median: {}".format(np.median(tot_time_list)))
# print("stdev: {}".format(np.std(tot_time_list)))


