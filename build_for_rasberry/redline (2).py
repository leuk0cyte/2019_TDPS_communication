import numpy as np
import cv2 as cv
import matplotlib.pyplot as plt
import math as m
import time


def dline(img, frame):
    lines = cv.HoughLinesP(img, 1, np.pi / 180, 20, 0, 50, 10)
    result = frame.copy()
    num = 0
    angleall = 0
    angleaverage = 0
    k = 0
    a = 0
    d = 0
    try:
        lines1 = lines[:, 0, 0]
        while k < lines1.size:
            for x1, y1, x2, y2 in lines[k]:
                if x1 != x2 and y1 != y2:
                    cv.line(result, (x1, y1), (x2, y2), (0, 0, 255), 1)
                    d = y1 - y2
                    c = x2 - x1
                    x = m.sqrt(d*d + c*c)
                    radian = m.asin(d/x)
                    angle = (radian * 180) / np.pi
                    if angle < 0:
                        angle = angle + 180
                    angleall += angle
                    num += 1
            k = k + 1;
                ##print(angle)
        angleaverage = angleall / num
        if angleaverage<=93 and angleaverage>87:
            d = 0
            print("mid")
        elif angleaverage>93 and angleaverage<135:
            d = 5
            print("left")
        elif angleaverage>=135 and angleaverage<180:
            d = 30
            print("left2")
        elif angleaverage<=87 and angleaverage>=45:
            d = 6
            print("right")
        elif angleaverage<45 and angleaverage>0:
            d = 31
            print("right2")
        cv.imshow('result', result)
        #time.sleep(0.5)
        print(num)
        print(angleaverage)
        return d
    except:
        ##cv.imshow('wrong', frame)
        print("no line")



def prep(img):
    HSV = cv.cvtColor(img, cv.COLOR_BGR2HSV)
    H, S, V = cv.split(HSV)
    Lower = np.array([0, 0, 100])
    Upper = np.array([255, 255, 255])
    mask = cv.inRange(HSV, Lower, Upper)
    cv.imshow("HSV", mask)

    kernel = cv.getStructuringElement(cv.MORPH_RECT, (10, 10), (-1, -1))
    ##kernel2 = cv.getStructuringElement(cv.MORPH_RECT, (24, 24), (-1, -1))
    dilate = cv.dilate(mask, kernel)
    cv.imshow("dilate", dilate)

    return dilate


cap = cv.VideoCapture(0)
cap.set(3, 640)  # set Width
cap.set(4, 480)  # set Height
while (True):
    degree = 0
    time_start = time.time()
    ret, frame = cap.read()
    cv.imshow('frame',frame)
    frame1 = prep(frame)
    canny = cv.Canny(frame1, 5, 200, 5)  # gray·¨
    cv.imshow("canny", canny)
    gblur = cv.GaussianBlur(canny, (3,3), 4,4);
    degree = dline(canny, frame)
    time_end = time.time()
    td = time_end-time_start
    print("degree=",degree)
    k = cv.waitKey(30) & 0xff
    if k == 27:
        break
cap.release()
cv.destroyAllWindows()