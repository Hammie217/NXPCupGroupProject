import numpy as np
import cv2
import time

cap = cv2.VideoCapture(0)
sumOfFPS=0
sumOfFPSQ=0
while(True):
    # Capture frame-by-frame
    start_time = time.time()
    ret, frame = cap.read()

    # Our operations on the frame come here
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    #Blur
    gray = cv2.GaussianBlur(gray,(5,5),0)

    # Find the edges in the image using canny detector
    edges = cv2.Canny(gray, 100, 200)

    #create and multiply ROI
    height = edges.shape[0]
    width = edges.shape[1]
    lanes = np.array([ [(1*width/10,height), (4*width/10,height), (5*width/10,0)] , [(9*width/10 ,height), (6*width/10 ,height), (5*width/10 ,0)] ], np.int32)

    mask = np.zeros_like(edges)
    frame = cv2.polylines(frame, lanes,1,(255,0,0),2)


    cv2.fillPoly(mask, lanes,255)

    edges = cv2.multiply(edges,mask)



    # Detect points that form a line
    lines = cv2.HoughLinesP(edges, 1, np.pi/180, 100, minLineLength=10, maxLineGap=250)
    # Draw lines on the image
    try:
        for line in lines: 
            for x1,y1,x2,y2 in line:
                cv2.line(frame,(x1,y1),(x2,y2),(0,255,0),2)
    except:
        print("Error")
    # Show result

    # Display the resulting frame
    cv2.imshow('frame',frame)
    print("{0:4.2f} Seconds".format((time.time() - start_time)))
    sumOfFPS+=(1/(time.time() - start_time))
    sumOfFPSQ +=1
    print("Average FPS {0:4.1f}".format( (sumOfFPS/sumOfFPSQ) ) ) 
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

# When everything done, release the capture
cap.release()
cv2.destroyAllWindows()
