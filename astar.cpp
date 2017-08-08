#include <stdio.h>
#include <math.h>
#include <vector>
#include <stdlib.h>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"

using namespace std;
using namespace cv;

typedef struct{
	int x;
	int y;
}point;

class astar{

private:
	Mat prob;
	int **manhat;
	int **open;
	int **closed;
	point start, endd;

public:
	astar(Mat img);
	void specifications(point s, point e);
	void pathplanning();
	Mat get_path();
	int is_valid(int x, int y);

};

astar::astar(Mat img){

	prob = img.clone();
	
	manhat = (int **)malloc(img.rows*sizeof(int *));
	closed = (int **)malloc(img.rows*sizeof(int *));
	open = (int **)malloc(img.rows*sizeof(int *));


	for(int i=0;i<img.rows;i++){
		manhat[i] = (int *)malloc(img.cols*sizeof(int));
		closed[i] = (int *)malloc(img.cols*sizeof(int));
		open[i] = (int *)malloc(img.cols*sizeof(int));
	}
	
	
}

void astar::specifications(point s, point e){

	start.x = s.x;
	start.y = s.y;
	endd.x = e.x;
	endd.y = e.y;
	for (int i = 0; i < prob.rows; i++){
		for (int j = 0; j < prob.cols; j++){
			manhat[i][j] = abs(j - endd.y) + abs(i - endd.x);
			closed[i][j] = open[i][j] = 0;
		}	
	}


}

int astar::is_valid(int x, int y){

	if(x>=0 && x<prob.rows && y>=0 && y<prob.cols)
		return 1;
	else
		return 0;
}

void astar::pathplanning(){

		printf("path pathplanning entered\n");
		point current,temp;
		vector<point> q;
		int h, flag_end = 0, min, pos;
		q.push_back(start);
		open[start.x][start.y] = 1;
		closed[start.x][start.y] = 1;
	
		while (!q.empty()){
			current = q.back();
			q.pop_back();
			closed[current.x][current.y] = 1;
			if (current.x == endd.x && current.y == endd.y)
				break;
			for (int i = current.x - 1; i <= current.x + 1; i++){
				for (int j = current.y - 1; j <= current.y + 1; j++){
					h = is_valid(i, j);
					if (h == 1 && closed[i][j] == 0 && prob.at<uchar>(i,j)==0){
						
							if (open[i][j] == 0){
								if( i == current.x || j ==current.y)
									open[i][j] = open[current.x][current.y] + 10;
								else 
									open[i][j] = open[current.x][current.y] + 14;
								temp.x=i;
								temp.y=j;
								q.push_back(temp);
							}
							else{
								if( i == current.x || j ==current.y){
									if ((open[current.x][current.y] + 10) < open[i][j])
										open[i][j] = open[current.x][current.y] + 10;
								}
								else{
									if ((open[current.x][current.y] + 14) < open[i][j])
										open[i][j] = open[current.x][current.y] + 14;
								}
							}
						
						
					}
				}
	
			}

			if (!q.empty()){
				int flag = 0;
				current = q.back();
				min = open[current.x][current.y] + manhat[current.x][current.y];
				for (int k = 0; k < q.size(); k++){
					current = q[k];
					if (open[current.x][current.y] + manhat[current.x][current.y] < min){
						min = open[current.x][current.y] + manhat[current.x][current.y];
						pos = k;
						flag = 1;
					}
				}
				if (flag){
					current = q[pos];
					q[pos] = q[q.size() - 1];
					q[q.size() - 1] = current;
				}
			}
	
		}
	printf("pathplanning exit\n");
}

vector <point> astar::get_path(){    // IMP ::THE FUNCTIONS GIVES A POINTS IN REVERSE ORDER SO START FROM BACKWARD WHICH IS THE POINT 
	//  AFTER START POINT (DOES NOT INCLUDE START POINT) AND THE FIRST POINT IS THE LAST POINT  
	int start = 0, dist;
	point current, p;
	current.x = endd.x;
	current.y = endd.y;
	int pos_i, pos_j, min, h;

	vector<point> node;
	p = current;
	node.push_back(p);

	while (open[current.x][current.y] != 1){
		prob.at<uchar>(current.x,current.y) = 255;           // remember to comment this out
		min = open[current.x][current.y];
		pos_i = current.x;
		pos_j = current.y;
		for (int i = current.x - 1; i <= current.x + 1; i++){
			for (int j = current.y - 1; j <= current.y + 1; j++){
				
					h = is_valid(i, j);
					if (h == 1 && closed[i][j] == 1 && open[i][j] <min ){
						min = open[i][j];
						pos_i = i;
						pos_j = j;
					}
				
			}
		}

		if(start==0){
			dist = abs(current.x - pos_i) + abs(current.y - pos_j);
			start = 1;
		}
		else{
			if(abs(current.x - pos_i) + abs(current.y - pos_j) != dist){
				p = current;
				node.push_back(p);
				start = 0;
				circle(prob, Point(pos_j, pos_i), 5, Scalar(255));
			}
		}	

		current.x = pos_i;
		current.y = pos_j;
	}

	return node;

}