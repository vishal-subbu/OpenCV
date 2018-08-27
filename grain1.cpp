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
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include<math.h>

using namespace cv;
using namespace std;

Mat src; Mat src_gray;
int element_shape = MORPH_RECT;
int thresh = 30;
int max_thresh = 255;
RNG rng(12345);


/// Function header
void thresh_callback(int, void* );

/** @function main */
int main( )
{
	src_gray = imread("/home/vishal/CVG/Programs/MgO1200-50-3_002.jpg", 0 );
	imshow("INPUT",src_gray);
	//cvtColor( src, src_gray, CV_BGR2GRAY );
	blur( src_gray, src_gray, Size(3,3) );
	threshold( src_gray,src_gray , 130,255,3 );
	createTrackbar( " Canny thresh:", "INPUT", &thresh, max_thresh, thresh_callback );
	thresh_callback( 0, 0 );
	waitKey(0);
	return(0);
}

/** @function thresh_callback */
void thresh_callback(int, void* )
{
	Mat canny_output,canny_output1;
	vector<vector<Point> > contours,contours2;
	vector<Vec4i> hierarchy,condef;
	vector<Point> poly,poly_final,points;
	Canny( src_gray, canny_output, thresh, thresh*2, 3 );
	imshow("Canny",canny_output);
	findContours( canny_output, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
	Mat drawing = Mat::zeros( canny_output.size(), CV_8UC3 );
	Mat pers = Mat::zeros( canny_output.size(), CV_8UC3 );
	Mat pers_gray ;
	for( int i = 0; i< contours.size(); i++ )
		{
		Scalar color( rand()&255, rand()&255, rand()&255 );		 
		drawContours( drawing, contours[i], 0,color, 1, 8 );		 
     }
/*
   for(int i=0;i<poly_final.size();i++)
      {
        points.push_back(poly_final[i]);
      }
              vector<int> hull;
        convexHull(Mat(points), hull, true);

        for( int i = 0; i < poly_final.size(); i++ )
            circle(drawing, points[i], 3, Scalar(0, 0, 255), CV_FILLED, CV_AA);

        int hullcount = (int)hull.size();
        Point pt0 = points[hull[hullcount-1]];

        for( int  i = 0; i < hullcount; i++ )
        {
            Point pt = points[hull[i]];
            line(drawing, pt0, pt, Scalar(0, 255, 0), 1, CV_AA);
            pt0 = pt;
        }
        convexityDefects( poly_final,hull, condef );
        float max=condef[0][3];
        for(int i=0;i<condef.size();i++)
          if(condef[i][3]>=max)
            {

             max=condef[i][3];
             cenpos=condef[i][2];
             pos=i;
            }
  
imshow("Pers",pers);*/
imshow("OUTPUT",drawing);

    waitKey(0);
    //return 0;



}




