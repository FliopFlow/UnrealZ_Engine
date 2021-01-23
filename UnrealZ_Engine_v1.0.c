#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAXLINECOUNT 20
#define MAXCIRCLECOUNT 20
#define PDELTA 0.4
#define CDELTA 0.5
#define PI 3.14159265358979

#define getFrame(x,y) Frame[(x+80)%159][(y+80)%159]
#define setFrame(x,y,value) Frame[x+80][y+80]=value
#define flushFrame() memset(Frame,0,sizeof(Frame))
#define clearScreen() system("clear")

short Frame[160][160];

struct Line
{
	double x1;
	double y1;
	double x2;
	double y2;
	double k;
	double b;
};

struct Circle
{
	double x;
	double y;
	double r;
};

typedef struct Line defLine;
typedef struct Circle defCircle;

double getSmaller(double a,double b);
double getBigger(double a,double b);
double calDistLine2Point(defLine line,double x,double y);
double calDistPoint2Point(double x1,double y1,double x2,double y2);
char TransSymb2Pixel(short Symb);
void printLine(defLine line);
void calLineKandB(defLine* line);
void printCircle(defCircle circle);
void ShowOneFrame(short trX,short trY);
void printLines(defLine lines[MAXLINECOUNT],int linesCount);
void printCircles(defCircle circles[MAXCIRCLECOUNT],int circlesCount);
void createCircle(double x,double y,double r,defCircle* circles,int* circlesCount);
void createLine(double x1,double y1,double x2,double y2,defLine* lines,int* linesCount);
void createTriangle(double x1,double y1,double x2,double y2,double x3,double y3,defLine* lines,int* linesCount);
void createQuadrilateral(double x1,double y1,double x2,double y2,double x3,double y3,double x4,double y4,defLine* lines,int* linesCount);
void Render(defLine lines[MAXLINECOUNT],int linesCount,defCircle circles[MAXCIRCLECOUNT],int circlesCount,short trX,short trY,double rollDeg);

int main()
{
	defLine lines[MAXLINECOUNT];
	defCircle circles[MAXCIRCLECOUNT];
	int linesCount=0,circlesCount=0;
	flushFrame();
	clearScreen();

	createTriangle(-30,-10,30,-10,0,10,lines,&linesCount);
	createCircle(0,10,7,circles,&circlesCount);
	createCircle(0,-10,7,circles,&circlesCount);
	createQuadrilateral(-40,-19,40,-19,40,19,-40,19,lines,&linesCount);
	createLine(0,-20,0,19,lines,&linesCount);
	
	for(double rad=0;;rad++)
	{
		Render(lines,linesCount,circles,circlesCount,rad,0,rad*PI/180);
		flushFrame();
		usleep(20000);
		clearScreen();
	}
	
	return 0;
}

double getSmaller(double a,double b)
{
	return a<b?a:b;
}

double getBigger(double a,double b)
{
	return a>b?a:b;
}

void calLineKandB(defLine* line)
{
	line->k=(line->y2-line->y1)/(line->x2-line->x1);
	line->b=line->y1-line->k*line->x1;
}

double calDistLine2Point(defLine line,double x,double y)
{
	return fabs(line.k*x-y+line.b)/sqrt(line.k*line.k+1);
}

double calDistPoint2Point(double x1,double y1,double x2,double y2)
{
	return sqrt((y2-y1)*(y2-y1)+(x2-x1)*(x2-x1));
}

void printLine(defLine line)
{
	for(double y=getSmaller(line.y1,line.y2);y<=getBigger(line.y1,line.y2);y++)
	{
		for(double x=getSmaller(line.x1,line.x2);x<=getBigger(line.x1,line.x2);x++)
		{
			if(calDistLine2Point(line,x,y)<=PDELTA)
			{
				setFrame(((short)x),((short)y),4);
			}
		}
	}
}

void printLines(defLine lines[MAXLINECOUNT],int linesCount)
{
	for(int i=0;i<linesCount;i++)
	{
		printLine(lines[i]);
	}
}

void printCircle(defCircle circle)
{
	double distDelta;
	for(double y=-20;y<=19;y++)
	{
		for(double x=-80;x<=79;x++)
		{
			distDelta=fabs(calDistPoint2Point(x,y,circle.x,circle.y)-circle.r);
			if(distDelta<=CDELTA)
			{
				setFrame(((short)x),((short)y),4);
			}
		}
	}
}

void printCircles(defCircle circles[MAXCIRCLECOUNT],int circlesCount)
{
	for(int i=0;i<circlesCount;i++)
	{
		printCircle(circles[i]);
	}
}

char TransSymb2Pixel(short Symb)
{
	switch (Symb)
	{
		case 0:
			return ' ';
		case 1:
			return '.';
		case 2:
			return '*';
		case 3:
			return '0';
		case 4:
			return '#';
	}
}

void ShowOneFrame(short trX,short trY)
{
	printf("===================================================================================================================================================================\n");
	for(short y=19;y>=-20;y--)
	{
		printf("||");
		for(short x=-80;x<79;x++)
		{
			printf("%c",TransSymb2Pixel(getFrame(x+trX,y+trY)));
		}
		printf("||\n");
	}
	printf("===================================================================================================================================================================\n");
}

void createCircle(double x,double y,double r,defCircle* circles,int* circlesCount)
{
	circles[*circlesCount].x=x;
	circles[*circlesCount].y=y;
	circles[*circlesCount].r=r;
	(*circlesCount)++;
}

void createLine(double x1,double y1,double x2,double y2,defLine* lines,int* linesCount)
{
	lines[*linesCount].x1=x1;
	lines[*linesCount].y1=y1;
	lines[*linesCount].x2=x2;
	lines[*linesCount].y2=y2;
	calLineKandB(&lines[*linesCount]);
	(*linesCount)++;
}

void createTriangle(double x1,double y1,double x2,double y2,double x3,double y3,defLine* lines,int* linesCount)
{
	lines[*linesCount].x1=x1;
	lines[*linesCount].y1=y1;
	lines[*linesCount].x2=x2;
	lines[*linesCount].y2=y2;
	calLineKandB(&lines[*linesCount]);
	(*linesCount)++;
	lines[*linesCount].x1=x1;
	lines[*linesCount].y1=y1;
	lines[*linesCount].x2=x3;
	lines[*linesCount].y2=y3;
	calLineKandB(&lines[*linesCount]);
	(*linesCount)++;
	lines[*linesCount].x1=x2;
	lines[*linesCount].y1=y2;
	lines[*linesCount].x2=x3;
	lines[*linesCount].y2=y3;
	calLineKandB(&lines[*linesCount]);
	(*linesCount)++;
}

void createQuadrilateral(double x1,double y1,double x2,double y2,double x3,double y3,double x4,double y4,defLine* lines,int* linesCount)
{
	lines[*linesCount].x1=x1;
	lines[*linesCount].y1=y1;
	lines[*linesCount].x2=x2;
	lines[*linesCount].y2=y2;
	calLineKandB(&lines[*linesCount]);
	(*linesCount)++;
	lines[*linesCount].x1=x2;
	lines[*linesCount].y1=y2;
	lines[*linesCount].x2=x3;
	lines[*linesCount].y2=y3;
	calLineKandB(&lines[*linesCount]);
	(*linesCount)++;
	lines[*linesCount].x1=x3;
	lines[*linesCount].y1=y3;
	lines[*linesCount].x2=x4;
	lines[*linesCount].y2=y4;
	calLineKandB(&lines[*linesCount]);
	(*linesCount)++;
	lines[*linesCount].x1=x4;
	lines[*linesCount].y1=y4;
	lines[*linesCount].x2=x1;
	lines[*linesCount].y2=y1;
	calLineKandB(&lines[*linesCount]);
	(*linesCount)++;
}

void Render(defLine lines[MAXLINECOUNT],int linesCount,defCircle circles[MAXCIRCLECOUNT],int circlesCount,short trX,short trY,double rollDeg)
{
	defLine *rendLines=malloc(sizeof(defLine)*linesCount);
	defCircle *rendCircles=malloc(sizeof(defCircle)*circlesCount);
	for(int i=0;i<linesCount;i++)
	{
		rendLines[i].x1=lines[i].x1*cos(rollDeg)-lines[i].y1*sin(rollDeg);
		rendLines[i].y1=lines[i].x1*sin(rollDeg)+lines[i].y1*cos(rollDeg);
		rendLines[i].x2=lines[i].x2*cos(rollDeg)-lines[i].y2*sin(rollDeg);
		rendLines[i].y2=lines[i].x2*sin(rollDeg)+lines[i].y2*cos(rollDeg);
		calLineKandB(&rendLines[i]);
	}
	printLines(rendLines,linesCount);
	free(rendLines);
	for(int i=0;i<circlesCount;i++)
	{
		rendCircles[i].x=circles[i].x*cos(rollDeg)-circles[i].y*sin(rollDeg);
		rendCircles[i].y=circles[i].x*sin(rollDeg)+circles[i].y*cos(rollDeg);
		rendCircles[i].r=circles[i].r;
	}
	printCircles(rendCircles,circlesCount);
	free(rendCircles);
	ShowOneFrame(trX,trY);
}