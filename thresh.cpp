#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include <stack>
#include <iostream>
#include <math.h>
#include <stdlib.h>

using namespace cv;

int front[6] = {};
int back[6] = {};

void threshold(Mat img)
{
	Mat dst;
	int low_v,low_s,low_h,high_s,high_v,high_h;

	namedWindow("Object Detection",WINDOW_AUTOSIZE);
	createTrackbar("Low H","Object Detection", &low_h, 180);
    createTrackbar("High H","Object Detection", &high_h, 180);
    createTrackbar("Low S","Object Detection", &low_s, 255);
    createTrackbar("High S","Object Detection", &high_s, 255);
    createTrackbar("Low V","Object Detection", &low_v, 255);
    createTrackbar("High V","Object Detection", &high_v, 255);

    cvtColor(img , img , CV_BGR2HSV);
   

    while(1)
    {
	    
		inRange(img , Scalar(low_h , low_s , low_v) , Scalar(high_h , high_s , high_v) , dst);
		imshow("Object Detection",dst);
		waitKey(10);
	}
}

void getBotpoints(Mat img , point &pt , int flag )
{
	Mat dst;
	vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    vector<Rect> boundRect( contours.size() );

    int low_v,low_s,low_h,high_s,high_v,high_h;

	cvtColor(img , img , CV_BGR2HSV);
	if(flag)
	{
		inRange(img , Scalar(front[0], front[1] , front[2]) , Scalar(front[3] , front[4] , front[5]) , dst);
	}
	else
	{
		inRange(img , Scalar(back[0], back[1] , back[2]) , Scalar(back[3] , back[4] , back[5]) , dst);	
	}


    findContours( img, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);

  	for (int i = 0; i < contours.size(); ++i)
  	{
  		if(contourArea(contours[i]) > 1000)
  		{
  			boundRect[i] = boundingRect(contours[i]);
  		}
  	}

	pt.x = boundRect[0].x + boundRect[0].width/2;
	pt.y = boundRect[0].y + boundRect[0].hieght/2


}



