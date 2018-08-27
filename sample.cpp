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
int main( )
{
Mat src;
// the first command-line parameter must be a filename of the binary
// (black-n-white) image
 src=imread("/home/vishal/Desktop/images1.png");
Mat dst = Mat::zeros(src.rows, src.cols, CV_8UC3);
namedWindow( "Source", 1 );
imshow( "Source", src );
vector<vector<Point> > contours;
vector<Vec4i> hierarchy;
findContours( src, contours, hierarchy,
CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE );
// iterate through all the top-level contours,
// draw each connected component with its own random color
int idx = 0;
for( ; idx >= 0; idx = hierarchy[idx][0] )
{
Scalar color( rand()&255, rand()&255, rand()&255 );
drawContours( dst, contours, idx, color, CV_FILLED, 8, hierarchy );
}
namedWindow( "Components", 1 );
imshow( "Components", dst );
waitKey(0);
}
