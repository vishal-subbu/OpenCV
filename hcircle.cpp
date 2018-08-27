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
#include <cv.h>
#include <highgui.h>
#include <math.h>
using namespace cv;
int main()
{
Mat img, gray;
img=imread("/home/vishal/CVG/opencv-2.4.10/samples/python2/data/starry_night.jpg");
cvtColor(img, gray, CV_BGR2GRAY);
// smooth it, otherwise a lot of false circles may be detected
GaussianBlur( gray, gray, Size(9, 9), 2, 2 );
vector<Vec3f> circles;
HoughCircles(gray, circles, CV_HOUGH_GRADIENT,2, gray.rows/4, 200, 100 );
for( size_t i = 0; i < circles.size(); i++ )
{
Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
int radius = cvRound(circles[i][2]);
// draw the circle center
circle( img, center, 3, Scalar(0,255,0), -1, 8, 0 );
// draw the circle outline
circle( img, center, radius, Scalar(0,0,255), 3, 8, 0 );
}
namedWindow( "circles" );
imshow( "circles", img );
return 0;
}
