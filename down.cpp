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

Mat src; Mat src_gray;Mat src_small,src_erode,src_thresh;
int element_shape = MORPH_RECT;
int thresh = 30;
int max_thresh = 255;
RNG rng(12345);
 
struct coord
{
 Point pt;
 int dx;
 int dy;
 float ang;
};

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
                    }

                   
             }
          }
       
     }
   for(int i=0;i<poly_final.size();i++)
      {
        points.push_back(poly_final[i]);
      }
   Point centre,side1,side2,down1;
   coord top,side,down;
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
           {top.pt=side1;top.dx=x1;top.dy=y1;top.ang=ang1;
            side.pt=side2;side.dx=x2;side.dy=y2;side.ang=ang2;}
         else
            {top.pt=side2;top.dx=x2;top.dy=y2;top.ang=ang2;
             side.pt=side1;side.dx=x1;side.dy=y1;side.ang=ang1;}
         }
        else
           {
             if(ang1<((pi/2)+0.23))
               {top.pt=side1;top.dx=x1;top.dy=y1;top.ang=ang1;
                side.pt=side2;side.dx=x2;side.dy=y2;side.ang=ang2;}
             else 
               {top.pt=side2;top.dx=x2;top.dy=y2;top.ang=ang2;
                side.pt=side1;side.dx=x1;side.dy=y1;side.ang=ang1;}
           }
 
     if(top.dx!=0)  
      {
      if (top.dx>0)
       {
        down1.x=centre.x-1;
        down1.y=(top.dy/top.dx)+centre.y; 
                circle(drawing, down1, 3, Scalar(255, 255,255), CV_FILLED, CV_AA);
        imshow("hull",drawing);

          while(pointPolygonTest(poly_final,down1,false)!=0) 
            {
             down1.x--;
             down1.y=(top.dy/top.dx)*(centre.x-down1.x)+centre.y; 
        circle(drawing, down1, 3, Scalar(255, 255,255), CV_FILLED, CV_AA);
        imshow("hull",drawing);

            }
         }
       else 
         {
        down1.x=centre.x+1;
        down1.y=(-1)*(top.dy/top.dx)+centre.y; 
        circle(drawing, down1, 3, Scalar(255, 255,255), CV_FILLED, CV_AA);
        imshow("hull",drawing);

          while(pointPolygonTest(poly_final,down1,false)!=0) 
            {
             down1.x++;
             down1.y=(top.dy/top.dx)*(centre.x-down1.x)+centre.y; 
        circle(drawing, down1, 3, Scalar(255, 255,255), CV_FILLED, CV_AA);
        imshow("hull",drawing);

            }
         }
       }
       else if(top.dy>0)
            {
              down1.x=centre.x;
              down1.y=centre.y+1; circle(drawing, down1, 3, Scalar(255, 255,255), CV_FILLED, CV_AA);
        imshow("hull",drawing);
              while(pointPolygonTest(poly_final,down1,false)!=0) 
               {
                down1.y++;circle(drawing, down1, 3, Scalar(255, 255,255), CV_FILLED, CV_AA);
        imshow("hull",drawing);
               }
            }
       else 
            {
              down1.x=centre.x;
              down1.y=centre.y-1; 
        circle(drawing, down1, 3, Scalar(255, 255,255), CV_FILLED, CV_AA);
        imshow("hull",drawing);

              while(pointPolygonTest(poly_final,down1,false)!=0) 
               {
                down1.y--;        circle(drawing, down1, 3, Scalar(255, 255,255), CV_FILLED, CV_AA);
        imshow("hull",drawing);

               }
            }
       
        //down.pt=down1;           
            
    

            
        circle(drawing, centre, 3, Scalar(0, 255, 0), CV_FILLED, CV_AA);
        circle(drawing, top.pt, 3, Scalar(255,0,0), CV_FILLED, CV_AA);
        circle(drawing, side.pt, 3, Scalar(0, 255,0), CV_FILLED, CV_AA);
        circle(drawing, down1, 3, Scalar(255, 255,255), CV_FILLED, CV_AA);
        imshow("hull",drawing);



 
}
