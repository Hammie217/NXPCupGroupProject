import numpy as np
import cv2
import time

cap = cv2.VideoCapture(0)

while(True):
    # Capture frame-by-frame
    start_time = time.time()
    ret, frame = cap.read()

    # Our operations on the frame come here
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

    # Find the edges in the image using canny detector
    edges = cv2.Canny(gray, 100, 200)
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
    print("{0:4.2f} Seconds  ({1:4.1f} FPS)".format((time.time() - start_time), (1/(time.time() - start_time))))
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

# When everything done, release the capture
cap.release()
cv2.destroyAllWindows()
