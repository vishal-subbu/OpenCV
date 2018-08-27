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

using namespace cv;
using namespace std;

Mat src; Mat src_gray;
int thresh = 100;
int max_thresh = 255;
RNG rng(12345);

/// Function header
void thresh_callback(int, void* );

/** @function main */
int main( )
{
  /// Load source image and convert it to gray
  //src = imread("/home/vishal/Pictures/pattern.png" , 1 );
  src = imread("/home/vedant/learningcv/20150211_121906.jpg" , 1 );
	resize(src,src,Size(644,480));
  /// Convert image to gray and blur it
  cvtColor( src, src_gray, CV_BGR2GRAY );
  blur( src_gray, src_gray, Size(3,3) );

  /// Create Window
  namedWindow("Source", CV_WINDOW_AUTOSIZE );
  imshow( "Source", src );

  createTrackbar( " Canny thresh:", "Source", &thresh, max_thresh, thresh_callback );
  thresh_callback( 0, 0 );

  waitKey(0);
  return(0);
}

/** @function thresh_callback */
void thresh_callback(int, void* )
{
  Mat canny_output;
  vector<vector<Point> > contours;
  vector<Vec4i> hierarchy;

  /// Detect edges using canny
  Canny( src_gray, canny_output, thresh, thresh*2, 3 );
imshow("vedant",canny_output);
waitKey(0);
  /// Find contours
  findContours( canny_output, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
  float area, length, ratio;
  /// Draw contours
  Mat drawing = Mat::zeros( canny_output.size(), CV_8UC3 );
  for( int i = 0; i< contours.size(); i++ )
     {     
           length=arcLength(contours[i],true);
           area=contourArea(contours[i],false);
	   vector<Point> poly;
	   approxPolyDP(contours[i], poly,length*0.002, true);

            if(!isContourConvex(poly))
          { 
           if(poly.size()>10)
             {      
		cout<<length<<endl;
		cout<<area<<endl;
              area=area*4.0/3.0;
              length=length*2.0/7.0;
              ratio=area/(length*length);
              if(((ratio-3.145)>-3)&&((ratio-3.145)<3))    
                   {
                    Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
       	            drawContours( drawing, contours, i, color, 2, 8, hierarchy, 0, Point() );
                   }
             }
          }
       
     }
  /// Show in a window
  namedWindow( "Contours", CV_WINDOW_AUTOSIZE );
  imshow( "Contours", drawing );
}

