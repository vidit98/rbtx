#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;

/** Function Headers */
void detectAndDisplay( Mat frame );

String face_cascade_name = "haar/cascade.xml";

CascadeClassifier face_cascade;
vector <vector <Point> > contours;


int main( int argc, const char** argv )
{
    
    Mat frame = imread("arena32.png" , 0);
    Canny(frame , frame , 50 ,100);
   
    findContours(frame,contours,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_SIMPLE);
    Mat dst(frame.rows,frame.cols,CV_8UC1,Scalar(0));
    drawContours(dst,contours,-1,Scalar(255));
     imshow("1" , dst);

    if( !face_cascade.load( face_cascade_name ) ){ printf("--(!)Error loading\n"); return -1; };
    detectAndDisplay(dst);

    waitKey(0);
    return 0;

}



void detectAndDisplay(Mat frame)
{
    std::vector<Rect> faces;
    Mat frame_gray = frame;

    
    //equalizeHist( frame, frame );
    printf("%s\n","check0" );
    //-- Detect faces
    face_cascade.load( face_cascade_name );
    printf("%s\n","check1.6" );
    face_cascade.detectMultiScale( frame, faces, 1.01, 1,0,Size(50, 28),Size(200,120));
    printf("%s\n","check1" );
    for( size_t i = 0; i < faces.size(); i++ )
    {
        printf("%s\n","check2" );
        Point center( faces[i].x + faces[i].width*0.5, faces[i].y + faces[i].height*0.5 );
        rectangle( frame, faces[i], Scalar::all(255), 2, 8, 0 );
        //ellipse( frame, center, Size( faces[i].width*0.5, faces[i].height*0.5), 0, 0, 360, Scalar( 255, 0, 255 ), 4, 8, 0 );

        Mat faceROI = frame( faces[i] );

    }
    printf("%s\n","check3" );
    imshow("face" , frame);
    imwrite("test.png" , frame);

}