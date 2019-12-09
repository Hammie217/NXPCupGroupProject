import numpy as np
import cv2
import time

cap = cv2.VideoCapture(1)
sumOfFPS=0
sumOfFPSQ=0
while(True):
    # Capture frame-by-frame
    start_time = time.time()
    ret, frame = cap.read()
    #frame = cv2.resize(frame,None,fx=0.25,fy=0.25)

    # Our operations on the frame come here
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    #Blur
    gray = cv2.GaussianBlur(gray,(5,5),0)

    ret, BorW = cv2.threshold(gray,80,255,0)

    height = BorW.shape[0]
    width = BorW.shape[1]
    #print(width)
    averageL = [None] * 10
    averageR = [None] * 10
    for ii in range(0,10):
        total=0
        hits=0
        for i in range(0,int(BorW.shape[1]/2)):
            if(BorW[int(ii*height/10)][i]==0):
                total+=i
                hits+=1
        if(hits>0):
            averageLT = round(total/hits)
        else:
            averageLT=int(BorW.shape[1]/4)
        averageL[ii]=averageLT
        
        total=0
        hits=0
        for i in range(int(BorW.shape[1]/2),BorW.shape[1]):
            if(BorW[int(ii*height/10)][i]==0):
                total+=(i)-BorW.shape[1]/2
                hits+=1
        if(hits>0):
            averageRT = round(total/hits) + int(BorW.shape[1]/2)
        else:
            averageRT=3*int(BorW.shape[1]/4)
        averageR[ii]=averageRT
    for ii in range(0,10):
        cv2.rectangle(frame,(0,int((ii)*height/10)),(width,int((ii+1)*height/10)),(0,255,255),2)
        cv2.line(frame,(int(0.25*width),int((ii)*height/10)),(int(0.25*width),int((ii+1)*height/10)),(0,255,255),2)
        cv2.line(frame,(int(0.5*width),int((ii)*height/10)),(int(0.5*width),int((ii+1)*height/10)),(255,255,255),2)
        cv2.line(frame,(int(0.75*width),int((ii)*height/10)),(int(0.75*width),int((ii+1)*height/10)),(0,255,255),2)
        cv2.line(frame,(int(averageL[ii]),int((ii)*height/10)),(int(averageL[ii]),int((ii+1)*height/10)),(0,0,255),2)
        cv2.line(frame,(int(averageR[ii]),int((ii)*height/10)),(int(averageR[ii]),int((ii+1)*height/10)),(0,0,255),2)

    print("Right:" +str(averageR[ii])  + "  Left: "+str(averageL[ii]))
    

    #contours, hierarchy = cv2.findContours(BorW,cv2.RETR_TREE,cv2.CHAIN_APPROX_SIMPLE)

    #img = cv2.drawContours(frame, contours, -1, (0,255,0), 3)

    #cropped = BorW[0:50, 0:50]
    # Find the edges in the image using canny detector
    #edges = cv2.Canny(gray, 100, 200)

    #create and multiply ROI
    #height = gray.shape[0]
    #width = gray.shape[1]
    #lanes = np.array([ [(1*width/10,height), (4*width/10,height), (5*width/10,0)] , [(9*width/10 ,height), (6*width/10 ,height), (5*width/10 ,0)] ], np.int32)

    #mask = np.zeros_like(edges)
    #frame = cv2.polylines(frame, lanes,1,(255,0,0),2)


    #cv2.fillPoly(mask, lanes,255)

    #edges = cv2.multiply(edges,mask)



    # Detect points that form a line
    #lines = cv2.HoughLinesP(edges, 1, np.pi/180, 100, minLineLength=10, maxLineGap=250)
    # Draw lines on the image
    #try:
    #    for line in lines: 
    #        for x1,y1,x2,y2 in line:
    #except:
    ##            cv2.line(frame,(x1,y1),(x2,y2),(0,255,0),2)
    #    print("Error")
    # Show result
    '''
    IMG_SIZE = frame.shape[::-1][1:]
    OFFSET = 300
    PRES_SRC_PNTS = np.float32([
        (596, 447), # Top-left corner
        (190, 720), # Bottom-left corner
        (1125, 720), # Bottom-right corner
        (685, 447) # Top-right corner
    ])

    PRES_DST_PNTS = np.float32([
    [OFFSET, 0], 
    [OFFSET, IMG_SIZE[1]],
    [IMG_SIZE[0]-OFFSET, IMG_SIZE[1]], 
    [IMG_SIZE[0]-OFFSET, 0]
    ])

    M = cv2.getPerspectiveTransform(PRES_SRC_PNTS, PRES_DST_PNTS)

    warped = cv2.warpPerspective(frame, M, IMG_SIZE, flags=cv2.INTER_LINEAR)


    '''
    # Display the resulting frame
    cv2.imshow('frame',frame)
    #print("{0:4.2f} Seconds".format((time.time() - start_time)))
    #sumOfFPS+=(1/(time.time() - start_time))
    #sumOfFPSQ +=1
    #print("Average FPS {0:4.1f}".format( (sumOfFPS/sumOfFPSQ) ) ) 
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

# When everything done, release the capture
cap.release()
cv2.destroyAllWindows()
