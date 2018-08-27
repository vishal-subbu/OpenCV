//
// Copyright (c) 2018, Vishal_S
// All rights reserved. Please read the "license.txt" for license terms.
//
// Project Title: Augmented Reality
//
// Developer: Vishal S
//
// Contact Info: vishalsubbu97@gmail.com
//
#include "cv.h"
#include "highgui.h"
using namespace cv;
int main(int, char **)
{
VideoCapture cap(0);
if(!cap.isOpened()) return -1;
Mat frame, edges;
namedWindow("edges",1);
for(;;)
{
cap >> frame;
cvtColor(frame, edges, CV_BGR2GRAY);
GaussianBlur(edges, edges, Size(7,7), 1.5, 1.5);
Canny(edges, edges, 0, 30, 3);
imshow("edges", edges);
if(waitKey(30) >= 0) break;
}
return 0;
}
