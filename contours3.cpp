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
int dex;

Mat src; Mat src_gray;Mat src_small,src_erode,src_thresh;
int element_shape = MORPH_RECT;
int thresh = 30;
int max_thresh = 255;
RNG rng(12345);

/// Function header
void thresh_callback(int, void* );

/** @function main */
int main(int argc,char* argv[] )
{
  /// Load source image and convert it to gray
  //src = imread("/home/vishal/Pictures/pattern.png" , 1 );
  src = imread(argv[1], 1 );
  resize(src, src_small, Size(640,480));
  /// Convert image to gray and blur it
  cvtColor( src_small, src_gray, CV_BGR2GRAY );
  blur( src_gray, src_gray, Size(3,3) );
  //imshow("Gray",src_gray);
  Mat element = getStructuringElement(element_shape, Size(5, 5), Point(2, 2) );
  erode(src_gray,src_erode, element);
  threshold( src_erode,src_thresh , 130,255,3 );
  //imshow("Erode",src_erode);
  //imshow("thresh",src_thresh);
  /// Create Window
  //namedWindow("Source", CV_WINDOW_AUTOSIZE);
  //imshow( "Source", src_small );

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
  vector<Vec4i> hierarchy,condef;
  vector<Point> poly,poly_final,points;

  /// Detect edges using canny
  Canny( src_thresh, canny_output, thresh, thresh*2, 3 );
      //namedWindow( "Canny", CV_WINDOW_AUTOSIZE);
  //imshow( "Canny", canny_output );

  /// Find contours
  findContours( canny_output, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
  float area, length, ratio;
  /// Draw contours
  Mat drawing = Mat::zeros( canny_output.size(), CV_8UC3 );
  for( int i = 0; i< contours.size(); i++ )
     {     
           length=arcLength(contours[i],true);
           area=contourArea(contours[i],false);
	   approxPolyDP(contours[i], poly,length*0.002, true);
            if(area>400	)											
            if(!isContourConvex(poly))
          { 
          if(poly.size()>10)
             {   
                 ratio=(((length)*(length))/area);
                 if((ratio>15)&&(ratio<30 ))									
                   
                    {
                       poly_final=poly;
                       dex=i;
                    }

                   
             }
          }
       
     }
   for(int i=0;i<poly_final.size();i++)
      {
        points.push_back(poly_final[i]);
      }
   Point centre,top,side,centroid,side1,side2;
   int cenpos,pos;
   float ang1,ang2,y1,y2,x1,x2,pi=4*(atan(1)),diff;
 
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
      
           side1=points[condef[pos][0]];
           side2=points[condef[pos][1]]; 
           centre=points[condef[pos][2]];
        y1=(-1)*(side1.y-centre.y);
        x1=(side1.x-centre.x); 
        y2=(-1)*(side2.y-centre.y);
        x2=(side2.x-centre.x);  
           

        ang1=atan(y1/x1);
        ang2=atan(y2/x2); 
        
       if((x1>0)&&(y1<0))
          {ang1=2*pi+ang1;}
       else if(x1<0)
          {ang1=pi+ang1;}
       else if(x1==0)
           {if(y1<0)
             ang1=pi+ang1;
        } 
       else
           ang1=ang1;
 
       if((x2>0)&&(y2<0))
          {ang2=2*pi+ang2;}
       else if(x2<0)
          {ang2=pi+ang2;}
       else if(x1==0)
           {if(y1<0)
             ang1=pi+ang1;
        } 
       else
           ang2=ang2;       
      
   diff=ang1-ang2;
 if(diff<0)
   diff=(-1*diff);
       if(diff<=((pi/2)+0.23))
        {if(ang1>ang2)
           {top=side1;
            side=side2;}
         else
            {top=side2;
             side=side1;}
         }
        else
           {
             if(ang1<((pi/2)+0.23))
               {top=side1;
                side=side2;}
             else 
               {top=side2;
                side=side1;}
           }
    

        centroid.x=(centre.x+top.x+side.x)/3;
        centroid.y=(centre.y+top.y+side.y)/3;        
            
    

        circle(drawing, centre, 3, Scalar(0, 255, 0), CV_FILLED, CV_AA);
        circle(drawing, top, 3, Scalar(255,0,0), CV_FILLED, CV_AA);
        circle(drawing, side, 3, Scalar(0, 255,0), CV_FILLED, CV_AA);
        circle(drawing, centroid, 3, Scalar(0, 255,0), CV_FILLED, CV_AA);
        //circle(drawing, down.pt, 3, Scalar(255, 255,255), CV_FILLED, CV_AA);
        imshow("hull",drawing);




/////////////////////////////////////////////////////////////////////////////////////

Mat nosmoke = imread("/home/vishal/Desktop/no smoking.png", 1);
    Mat cnosmoke= Mat::zeros( nosmoke.size(), CV_8UC1 );
    
    cvtColor(nosmoke, cnosmoke, COLOR_BGR2GRAY);
    medianBlur(cnosmoke, cnosmoke, 5);
    Point top1,rightside,centre1,centroid1;
    vector<Vec3f> circles;
    HoughCircles(cnosmoke, circles, CV_HOUGH_GRADIENT, 1, 10,
                 100, 30, 100, 400 // change the last two parameters
                                // (min_radius & max_radius) to detect larger circles
                 );
    int index=0;
    float maxr=circles[0][2];
    for( size_t i = 0; i < circles.size(); i++ )
    {
        /*Vec3i c = circles[i];
        circle( cimg, Point(c[0], c[1]), c[2], Scalar(0,0,255), 3, CV_AA);
        circle( cimg, Point(c[0], c[1]), 2, Scalar(0,255,0), 3, CV_AA);*/
        if(maxr< circles[i][2])
           {
            maxr=circles[i][2];
            index=i;
           }
    }
    centre1=Point(circles[index][0],circles[index][1]);
    top1 =  Point(circles[index][0],(circles[index][1]-circles[index][2]));
    rightside =  Point((circles[index][0]+circles[index][2]),circles[index][1]);
    centroid1.x=(circles[index][0]+circles[index][2]/3);
    centroid1.y=(circles[index][1]-circles[index][2]/3);
        circle(nosmoke, centre1, 3, Scalar(0, 255, 0), CV_FILLED, CV_AA);
        circle(nosmoke, top1, 3, Scalar(255,0,0), CV_FILLED, CV_AA);
        circle(nosmoke, rightside, 3, Scalar(0, 255,0), CV_FILLED, CV_AA);
        circle(nosmoke, centroid1, 3, Scalar(0, 255,0), CV_FILLED, CV_AA);
    imshow("detected circles", nosmoke);
    
/////////////////////////////////////////////////////////////////////////////////////

// Input Quadilateral or Image plane coordinates
    Point2f inputQuad[4];
    // Output Quadilateral or World plane coordinates
    Point2f outputQuad[4];
         
    // Lambda Matrix
    Mat lambda( 3, 3, CV_32FC1 );
    //Input and Output Image;
     
    //Load the image
    //input = imread( "lena.jpg", 1 );
    // Set the lambda matrix the same type and size as input
    lambda = Mat::zeros( 3, 3, CV_8UC1 );
 
 // The 4 points that select quadilateral on the input , from top-left in clockwise order
    // These four pts are the sides of the rect box used as input
    inputQuad[0] = Point2f( centre.x,centre.y );
    inputQuad[1] = Point2f( top.x,top.y);
    inputQuad[2] = Point2f( centroid.x,centroid.y  ); 
    inputQuad[3] = Point2f( side.x,side.y);
    // The 4 points where the mapping is to be done , from top-left in clockwise order
    outputQuad[0] = Point2f(centre1.x,centre1.y );
    outputQuad[1] = Point2f( top1.x,top1.y);
    outputQuad[2] = Point2f( centroid1.x,centroid1.y);
    outputQuad[3] = Point2f( side1.x,side1.y );
 
    // Get the Perspective Transform Matrix i.e. lambda
    lambda = getPerspectiveTransform(outputQuad, inputQuad );
    // Apply the Perspective Transform just fotnd to the src image
//resize(output, output_small, Size(640,480));
    warpPerspective(nosmoke,src_small,lambda,Size(640,480) );
// resize(output, output_small, Size(640,480));
    //Display input and output
    imshow("Input",src_small); 
    waitKey(0);
    //return 0;


 
}


