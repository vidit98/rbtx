#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"

using namespace std;
using namespace cv;

Mat img,arena;
int row,col;
vector <vector <Point> > contours;
vector<Rect> bound; 
vector <vector <Point> > contours2;

Rect inside(Point check){

	cout<<"*********** checking for point inside **********\n";
	cout<<"point is "<<check.x<<"  "<<check.y<<endl;

	for(int i=0;i<bound.size();i++){
		cout<<"box "<<i <<endl;
		cout<<bound[i].tl().x<<" "<<bound[i].br().x<<endl;
		cout<<bound[i].br().y<<" "<<bound[i].tl().y<<endl;
		if(check.x>=bound[i].tl().x-15 && check.x<=bound[i].br().x+15 && check.y>=bound[i].tl().y-15 && check.y<=bound[i].br().y+15)
			return bound[i];	
	}


}


Point matchingMethod(Mat src, Mat temp,double *min){
	Mat result;

	int result_cols =  src.cols - temp.cols + 1;
	int result_rows = src.rows - temp.rows + 1;

	result.create( result_rows, result_cols, CV_32FC1 );

	matchTemplate( src, temp, result, 0 );
	//normalize( result, result, 0, 1, NORM_MINMAX, -1, Mat() );

   double minVal, maxVal; 
   Point minLoc, maxLoc;

   minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );
    
   *min=maxVal;
   cout<<"orignal : "<<minVal<<endl;

   Rect box=inside(minLoc);
   
   cout<<"******** checking done **********\n";
   cout<<endl<<box.br().x-box.tl().x<<"    "<<temp.cols<<endl;
   cout<<box.br().y-box.tl().y<<"    "<<temp.rows<<endl;

   
   if(!(fabs(box.br().x-box.tl().x-temp.cols)<50 && fabs(box.br().y-box.tl().y-temp.rows)<50 ))
   		*min=0;

   return minLoc;

}

Mat get_size(){
	Mat temp;
	
	//cvtColor(arena,temp,CV_BGR2GRAY);
	//Canny(temp,temp,50,100);
	threshold(arena,temp,200,255,THRESH_BINARY_INV);
	findContours(temp,contours,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_SIMPLE);
	
	for(int i=0;i<contours.size();i++){
		bound.push_back(boundingRect(Mat(contours[i])));
	}

	Mat dst(temp.rows,temp.cols,CV_8UC1,Scalar(0));
	drawContours(dst,contours,-1,Scalar(255),-1);

	Mat imgg=dst(bound[2]);
	//imshow("check", imgg);
	return dst;
}

void find_pattern(){

	Point p,final;
	double min,m_min=-1;
	Mat temp1,img_temp;
	int dim1,dim2;

	//cvtColor(img,temp1,CV_BGR2GRAY);
	//Canny(temp1,temp1,50,100);
	threshold(img,temp1,200,255,THRESH_BINARY);
	findContours(img,contours2,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_SIMPLE);
		

	Mat dst(temp1.rows,temp1.cols,CV_8UC1,Scalar(0));
	drawContours(dst,contours2,-1,Scalar(255),-1);
	
	Mat img_ROI=dst(boundingRect(Mat(contours2[0])));
	imshow("region of interest",img_ROI);
	cout<<contours2.size()<<endl;

	Mat temp2=get_size();

	for(int i=0;i<bound.size();i++){
		
			resize(img_ROI,img_temp,Size(bound[i].br().x-bound[i].tl().x , bound[i].br().y-bound[i].tl().y));
			imshow("window1",img_temp);
			waitKey(3000);
			p=matchingMethod(temp2,img_temp,&min);
			cout<<p.x<<" "<<p.y<<endl;
			cout<<min<<endl;
			if(m_min==-1 && min!=0){
				m_min=min;
				final=p;
				dim1=bound[i].br().x-bound[i].tl().x;
				dim2=bound[i].br().y-bound[i].tl().y;
				//imshow("resized object",img_temp);
			}
			else if(min<m_min && min!=0){
				m_min=min;
				final=p;
				dim1=bound[i].br().x-bound[i].tl().x;
				dim2=bound[i].br().y-bound[i].tl().y;
			}
		
	}

	
	cout<<"\n\n"<<final.x<<" "<<final.y<<endl;
	rectangle( temp2, final,Point(final.x+dim1,final.y+dim2), Scalar::all(255), 2, 8, 0 );
  	cout << "final" << final.x << " " << final.y << "\n";
  	cout << "final1" << final.x + dim1 << " " << final.y + dim2;


  	imshow( "image_window", temp2 );
  	imshow( "result_window", img );
  	//imshow( "template", img_temp );
 
}


int main(){
    arena=imread("event.PNG",0);
    img=imread("rotate.png",0);

    find_pattern();

    waitKey(0);
    return 0;
}
