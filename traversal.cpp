#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <termios.h>

#define thresh_distance 1
#define thresh_angle .2
#define pi 3.14

struct point
{
	float x;
	float y;
};

void sendCommand(const char command) {
	char send=command;
	
	//write(fd, command, 1);
	FILE *serport =fopen("/dev/ttyACM1","w");
	if (serport!= NULL)
	{
		fprintf(serport,"%c\n",send);
		fclose(serport);
		printf("%c\n",command);
	}
	else
	{
		printf("%s\n","FILE NOT open" );
	}
}

float angle(point pt1 , point pt2)//returns angle in range -pi to pi 
{
	float angle;
	float dy = pt1.y - pt2.y;
	float dx = pt1.x - pt2.x;
	angle = atan(dx/dy);

	if(dy < 0 && dx < 0)
	{
		angle = angle - pi;
	}
	else if (dy < 0 && dx > 0)
	{
		angle = angle + pi;
	}

	return angle;
}

float dis(point pt1 , point pt2)
{
	float d;
	d = sqrt(pow(pt1.x - pt2.x , 2) + pow(pt1.y - pt2.y , 2));
	return d;
}

int move(point bot_front , point bot_back , point final)
{	
	float angle1 , orientation;
	point center;
	center.x = (bot_back.x + bot_front.x)/2;
	center.y = (bot_back.y + bot_front.y)/2;
	if (dis(center , final) < thresh_distance)
		return 1;

	angle1 = angle(final , center);
	orientation = angle(bot_front , bot_back);

	if (abs(angle1 - orientation) < thresh_angle)
	{
		printf("%s %f\n","Sending forward" , abs(angle1 - orientation));
		//sendCommand("W");
	}
	else
	{
		if((angle1 - orientation) < 0)
		{
			if(abs(angle1- orientation) <= pi) 
			{
				printf("%s\n","Sending left" );
				//sendCommand("A");
			}
			else
			{
				printf("%s\n","Sending right");
				//sendCommand("D");
			}
		}
		else
		{
			if (abs(angle1 - orientation) <= pi)
			{
				printf("%s\n", "Sending Right" );
				//sendCommand("D");
			}
			else
			{
				printf("%s\n","Sending left" );
				//sendCommand("A");
			}
		}
	}

	

}


int main()
{
	point pt1,pt2;
	pt1.x = 1;
	pt2.x = 2;
	pt1.y = 2;
	pt2.y = 4;

	std::cout << angle(pt2 , pt1);
	return 0;
}
