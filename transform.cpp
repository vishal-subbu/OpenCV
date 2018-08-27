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
  Mat canny_output,canny_output1;
  vector<vector<Point> > contours,contours2;
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
  Mat pers = Mat::zeros( canny_output.size(), CV_8UC3 );
 Mat pers_gray ;
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
   Point centre,side1,side2,down1,otherside1;
   coord top,side,down,otherside;
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
     Point beg;Point mid;

    float tan;
    float y;
     if(top.dx!=0)  
      {
       tan=(float)(top.dy)/(top.dx);
      if (top.dx>0)
       {
        down1.x=centre.x-1;
        y=tan+centre.y; 
        down1.y=y;
          while(pointPolygonTest(poly_final,down1,false)!=-1) 
            {
             down1.x--;
             y+=tan; 
             down1.y=y;
            }
           
          beg.y = down1.y - tan ;
         if((pointPolygonTest(poly_final,down1,true)>2 || pointPolygonTest(poly_final,down1,true)<-2))
           down.pt=down1;        
        else
{ 
         mid.y = (down1.y+beg.y)/2;
         mid.x=down1.x - 1;  
       while(pointPolygonTest(poly_final,mid,true)>2 || pointPolygonTest(poly_final,mid,true)<-2)
               {
               if(pointPolygonTest(poly_final,mid,true)<0)
                  {down1.y=mid.y ;}
                else if(pointPolygonTest(poly_final,mid,true)>0)
                  {beg.y = mid.y ;
                    }
                else
                      { break ;}
                if(((mid.y- ((down1.y+beg.y)/2))<2)&&(mid.y- ((down1.y+beg.y)/20))>-2)
                      break;
                 mid.y = (down1.y+beg.y)/2;
                  
              }
             down.pt=mid;
            }
 
         }
       else 
         {
         
        down1.x=centre.x+1;
        y=centre.y-tan; 
        down1.y=y;
          while(pointPolygonTest(poly_final,down1,false)!=-1) 
            {

             down1.x++;
             y-=tan;
             down1.y=y;    
            }
          beg.y = down1.y - tan ;
        if((pointPolygonTest(poly_final,down1,true)<2 || pointPolygonTest(poly_final,down1,true)>-2))
           down.pt=down1;         
        else
         {   
         mid.y = (down1.y+beg.y)/2;
         mid.x=down1.x - 1;  
       while(pointPolygonTest(poly_final,mid,true)>2 || pointPolygonTest(poly_final,mid,true)<-2)
               {
               if(pointPolygonTest(poly_final,mid,true)<0)
                  {down1.y=mid.y ;}
                else if(pointPolygonTest(poly_final,mid,true)>0)
                  {beg.y = mid.y ;
                    }
                else
                      { break ;}
               if(((mid.y- ((down1.y+beg.y)/2))<2)&&(mid.y- ((down1.y+beg.y)/20))>-2)
                      break; 
                mid.y = (down1.y+beg.y)/2;
               
              }
              down.pt=mid; 
             } 
 
          }
       }
       else if(top.dy>0)
            { cout<<" dy +ve"<<endl;
              down1.x=centre.x;
              down1.y=centre.y+1; 
              while(pointPolygonTest(poly_final,down1,false)!=-1) 
               {
                down1.y++;
               }
              down.pt=down1;
            }
       else 
            {cout<<" dy -ve"<<endl;
              down1.x=centre.x;
              down1.y=centre.y-1; 
              while(pointPolygonTest(poly_final,down1,false)!=-1) 
               {
                down1.y--; 
               }
               down.pt=down1;
            }
       
//////FOR OTHERSIDE
  
mid.x=0;mid.y=0;y=0;tan=0;
           
          if(side.dx!=0)  
      {
       tan=(float)(side.dy)/(side.dx);
      if (side.dx>0)
       {
        otherside1.x=centre.x-1;
        y=tan+centre.y; 
        otherside1.y=y;
          while(pointPolygonTest(poly_final,otherside1,false)!=-1) 
            {
             otherside1.x--;
             y+=tan; 
             otherside1.y=y;
            }
           
          beg.y = otherside1.y - tan ;
         if((pointPolygonTest(poly_final,otherside1,true)<2 || pointPolygonTest(poly_final,otherside1,true)>-2))
           otherside.pt=otherside1;        
        else
{ 
         mid.y = (otherside1.y+beg.y)/2;
         mid.x=otherside1.x - 1;  
       while(pointPolygonTest(poly_final,mid,true)>2 || pointPolygonTest(poly_final,mid,true)<-2)
               {
               if(pointPolygonTest(poly_final,mid,true)<0)
                  {otherside1.y=mid.y ;}
                else if(pointPolygonTest(poly_final,mid,true)>0)
                  {beg.y = mid.y ;
                    }
                else
                      { break ;}
                if(((mid.y- ((otherside1.y+beg.y)/2))<2)&&(mid.y- ((otherside1.y+beg.y)/20))>-2)
                      break;
                 mid.y = (otherside1.y+beg.y)/2;
                  
              }
             otherside.pt=mid;
            }
 
         }
       else 
         {
         
        otherside1.x=centre.x+1;
        y=centre.y-tan; 
        otherside1.y=y;
          while(pointPolygonTest(poly_final,otherside1,false)!=-1) 
            {

             otherside1.x++;
             y-=tan;
             otherside1.y=y;    
            }
          beg.y = otherside1.y - tan ;
        if((pointPolygonTest(poly_final,otherside1,true)<2 || pointPolygonTest(poly_final,otherside1,true)>-2))
           down.pt=otherside1;         
        else
         {   
         mid.y = (otherside1.y+beg.y)/2;
         mid.x=otherside1.x - 1;  
       while(pointPolygonTest(poly_final,mid,true)>2 || pointPolygonTest(poly_final,mid,true)<-2)
               { cout<<"Entered mid loop" <<endl;
               if(pointPolygonTest(poly_final,mid,true)<0)
                  {otherside1.y=mid.y ;}
                else if(pointPolygonTest(poly_final,mid,true)>0)
                  {beg.y = mid.y ;
                    }
                else
                      { break ;}
               if(((mid.y- ((otherside1.y+beg.y)/2))<2)&&(mid.y- ((otherside1.y+beg.y)/20))>-2)
                      break; 
                mid.y = (otherside1.y+beg.y)/2;
               
              }
              otherside.pt=mid; 
             } 
 
          }
       }
       else if(side.dy>0)
            { cout<<" dy +ve"<<endl;
              otherside1.x=centre.x;
              otherside1.y=centre.y+1; 
              while(pointPolygonTest(poly_final,otherside1,false)!=-1) 
               {
                otherside1.y++;
               }
              otherside.pt=otherside1;
            }
       else 
            {cout<<" dy -ve"<<endl;
              otherside1.x=centre.x;
              otherside1.y=centre.y-1; 
              while(pointPolygonTest(poly_final,otherside1,false)!=-1) 
               {
                otherside1.y--; 
               }
               otherside.pt=otherside1;
            }
             
    

        circle(drawing, otherside.pt, 3, Scalar(0, 255, 0), CV_FILLED, CV_AA);
        circle(drawing, top.pt, 3, Scalar(255,0,0), CV_FILLED, CV_AA);
        circle(drawing, side.pt, 3, Scalar(0, 255,0), CV_FILLED, CV_AA);
        circle(drawing, down.pt, 3, Scalar(255, 0,0), CV_FILLED, CV_AA);
        
        


/////////////////////////////////////////////////////////////////////////////////////

Mat nosmoke = imread("/home/vishal/Desktop/no smoking.png", 1);
    Mat cnosmoke= Mat::zeros( nosmoke.size(), CV_8UC1 );
    
    cvtColor(nosmoke, cnosmoke, COLOR_BGR2GRAY);
    medianBlur(cnosmoke, cnosmoke, 5);
    Point top1,rightside,centre1,leftside,bottom;
    vector<Vec3f> circles;
    HoughCircles(cnosmoke, circles, CV_HOUGH_GRADIENT, 1, 10,
                 100, 30, 100, 400 // change the last two parameters
                                // (min_radius & max_radius) to detect larger circles
                 );
    int index=0;
    float maxr=circles[0][2];
    for( size_t i = 0; i < circles.size(); i++ )
    {
        if(maxr< circles[i][2])
           {
            maxr=circles[i][2];
            index=i;
           }
    }
    centre1=Point(circles[index][0],circles[index][1]);
    top1 =  Point(circles[index][0],(circles[index][1]-circles[index][2]));
    rightside =  Point((circles[index][0]+circles[index][2]),circles[index][1]);
    bottom = Point(circles[index][0],(circles[index][1]+circles[index][2]));
    leftside =  Point((circles[index][0]-circles[index][2]),circles[index][1]);
   
    
/////////////////////////////////////////////////////////////////////////////////////

// Input Quadilateral or Image plane coordinates
    Point2f inputQuad[4];
    // Output Quadilateral or World plane coordinates
    Point2f outputQuad[4];
         
    // Lambda Matrix
    //Input and Output Image;
     
    //Load the image
    //input = imread( "lena.jpg", 1 );
    // Set the lambda matrix the same type and size as input
 
 // The 4 points that select quadilateral on the input , from top-left in clockwise order
    // These four pts are the sides of the rect box used as input
    inputQuad[0] = Point2f( top.pt.x,top.pt.y );
    inputQuad[1] = Point2f( side.pt.x,side.pt.y  ); 
    inputQuad[2] = Point2f( down.pt.x,down.pt.y);
    inputQuad[3] = Point2f( otherside.pt.x,otherside.pt.y);
    // The 4 points where the mapping is to be done , from top-left in clockwise order
    outputQuad[0] = Point2f(top1.x,top1.y );
    outputQuad[1] = Point2f( rightside.x,rightside.y);
    outputQuad[2] = Point2f( bottom.x,bottom.y );
    outputQuad[3] = Point2f( leftside.x,leftside.y);
 
    // Get the Perspective Transform Matrix i.e. lambda
   Mat lambda = getPerspectiveTransform(outputQuad, inputQuad );
    // Apply the Perspective Transform just fotnd to the src image
//resize(output, output_small, Size(640,480));
    warpPerspective(nosmoke,pers,lambda,Size(640,480));
// resize(output, output_small, Size(640,480));
    //Display input and output
    
 
/*vector<vector<Point> > contours1;

 cvtColor( pers, pers_gray, CV_BGR2GRAY );
  blur( pers_gray, pers_gray, Size(3,3) );
Canny( pers_gray, canny_output1, thresh, thresh*2, 3 );
   
findContours( canny_output1, contours1, hierarchy,
CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE ); 
//drawContours( pers, contours1, 0, Scalar(255,0,0), 2, 8, hierarchy );

int i,j;
 Point p1;
	for(i=0;i<pers.rows;i++)
	{
	for(j=0;j<pers.cols;j++)
	{p1.x=i;p1.y=j;
      if(pointPolygonTest(contours1[0],p1,false)<0) 
		pers.at<Vec3b>(i,j)[0]=255;
                pers.at<Vec3b>(i,j)[1]=255;
		pers.at<Vec3b>(i,j)[2]=255;
	}
        }

int maxarea;int number=0;


 cvtColor( pers, pers_gray, CV_BGR2GRAY );
  blur( pers_gray, pers_gray, Size(3,3) );
Canny( pers_gray, canny_output1, thresh, thresh*2, 3 );
findContours( canny_output1, contours2, hierarchy,
CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE ); 

 
drawContours( pers, contours2, 0, Scalar(255,0,0), 2, 8, hierarchy );

/* Vec3b p; 
  Point per;
for(int i=0;i<src_small.rows;i++)
for(int j=0;j<src_small.cols;j++)
  {per.x=i;per.y=j;
   if((pointPolygonTest(contours1[0],per,false)<0)) 
     {
      		p=pers.at<Vec3b>(i,j);	
		src_small.at<Vec3b>(i,j)[0]=p[0];
                src_small.at<Vec3b>(i,j)[1]=p[1];
		src_small.at<Vec3b>(i,j)[2]=p[2];
     }
   }*/
  
imshow("Pers",pers);
imshow("OUTPUT",src_small);

    waitKey(0);
    //return 0;



}




