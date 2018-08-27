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

Mat src; Mat src_gray;Mat src_small;
int thresh = 255;
int max_thresh = 255;
RNG rng(12345);

/// Function header
void thresh_callback(int, void* );

/** @function main */
int main( )
{
  /// Load source image and convert it to gray
  //src = imread("/home/vishal/Pictures/pattern.png" , 1 );
  src = imread("/home/vishal/Pictures/20150211_121912.jpg", 1 );
  resize(src, src_small, Size(640,480));
  /// Convert image to gray and blur it
  cvtColor( src_small, src_gray, CV_BGR2GRAY );
  blur( src_gray, src_gray, Size(3,3) );

  /// Create Window
  namedWindow("Source", CV_WINDOW_AUTOSIZE);
  imshow( "Source", src_small );

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
      namedWindow( "Canny", CV_WINDOW_AUTOSIZE);
  imshow( "Canny", canny_output );

  /// Find contours
  findContours( canny_output, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
  float area, length, ratio,width,ang;
  /// Draw contours
  Mat cimage = Mat::zeros( canny_output.size(), CV_8UC3 );
  for( int i = 0; i < contours.size(); i++ )
     {     
           length=arcLength(contours[i],true);
           area=contourArea(contours[i],false);
	   vector<Point> poly;
	   approxPolyDP(contours[i], poly,length*0.002, true);
            if(area >= 200)
          {											
            if(!isContourConvex(poly))
          { 
           if(poly.size()>10)
             {   									
                   
                    Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
       	            //drawContours( drawing, contours, i, color, 2, 8, hierarchy, 0, Point() );
                    size_t count = contours[i].size();
        if( count < 6 )
            continue;

        Mat pointsf;
        Mat(contours[i]).convertTo(pointsf, CV_32F);
        RotatedRect box = fitEllipse(pointsf);

        if( MAX(box.size.width, box.size.height) > MIN(box.size.width, box.size.height)*30 )
            continue;
        drawContours(cimage, contours, (int)i, Scalar::all(255), 1, 8);

        ellipse(cimage, box, Scalar(0,0,255), 1, CV_AA);
        ellipse(cimage, box.center, box.size*0.5f, box.angle, 0, 360, Scalar(0,255,255), 1, CV_AA);
        Point2f vtx[4];
        box.points(vtx);
        width=(box.size.height>box.size.width)?(box.size.height):(box.size.width);
        float heigth=(box.size.height<box.size.width)?(box.size.height):(box.size.width);
        float x=3.0*3.14*heigth*heigth/(64.0*area);
        ang=acos(pow(x,1.0/3.0));
	float p;
	p=acos(0);
        for( int j = 0; j < 4; j++ )
            line(cimage, vtx[j], vtx[(j+1)%4], Scalar(0,255,0), 1, CV_AA);
        cout<<i<<endl<<ang<<endl;
	cout<<p<<endl;
        ang=acos(heigth/width);
        cout<<ang<<endl;
             }
          }
        } 
     }

  /// Show in a window
    namedWindow( "Contours", CV_WINDOW_AUTOSIZE );
                     imshow( "Contours", cimage );
 
}

