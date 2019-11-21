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

