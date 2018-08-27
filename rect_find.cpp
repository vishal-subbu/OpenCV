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
using namespace std;


 

int main()
{
  
    Mat image, cont,edges;
    namedWindow( "Display window", WINDOW_AUTOSIZE );// Create a window for display.
    image = imread("/home/vishal/Desktop/rect.png");   // Read the file
    if(! image.data )                              // Check for invalid input
    {
        cout <<  "Could not open or find the image" << std::endl ;
        return -1;
    }
    cvtColor(image, edges, CV_BGR2GRAY);
    
    threshold(edges, edges,120,255,THRESH_BINARY);
   // findContours(edges,cont,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_NONE);
    
    imshow( "Display window", edges );                   // Show our image inside it.

    waitKey(0);                                          // Wait for a keystroke in the window
    return 0;
}
