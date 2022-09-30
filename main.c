#include <stdio.h>
#include <GL/glut.h>
#include <math.h>

#define PI 3.1415926535
#define mapWidth 16
#define mapHeight 16

//utils

int windowWidth = 1024;
int windowHeight = 512;

float frame1, frame2, fps;

typedef struct{
	int w;
	int a;
	int s;
	int d;
}keyStruct; keyStruct keys;

float degToRad(float a){
	return a*M_PI/180.0;
}

float FixAng(float a){ 
	if(a>359){
		a-=360;
	} 
	
	if(a<0){
		a+=360;
	} 
	
	return a;
}

float distance(ax,ay,bx,by,ang){ 
	return cos(degToRad(ang))*(bx-ax)-sin(degToRad(ang))*(by-ay);
}
///////

int mapS = 16;
int mapX = 16;
int mapY = 16;

float px, py, pdx, pdy, pa, fov;

/////////////////////////////////
/* COLOURS FOR MAP DEFINITIONS */
/* 0 = empty space / no wall   */
/* 1 = white wall              */
/* 2 = red wall                */
/* 3 = orange wall             */
/* 4 = yellow wall             */
/* 5 = green wall              */
/* 6 = blue wall               */
/* 7 = purple wall             */
/* 8 = pink wall			   */
/* 9 = door / lime green wall  */
/////////////////////////////////

int map[16][16] ={
	{2,2,2,1,1,1,1,1,1,1,1,1,1,1,1,1},
	{2,0,0,3,0,0,0,0,0,0,0,1,0,0,0,1},
	{2,0,0,3,0,0,0,0,0,0,0,1,0,0,0,1},
	{2,0,0,3,0,0,0,0,0,0,0,1,0,0,0,1},
	{1,0,0,3,0,0,0,0,0,0,0,1,1,0,0,1},
	{1,0,0,1,0,0,0,0,0,0,0,0,1,0,0,1},
	{5,0,0,6,0,0,0,0,0,0,0,0,1,0,0,1},
	{5,0,0,6,6,6,1,1,7,7,0,0,1,0,0,1},
	{5,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1},
	{5,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1},
	{4,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1},
	{4,0,0,0,0,0,0,1,8,8,1,0,0,0,0,1},
	{4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};

void drawRays(){
	float rR, rG, rB;
	
	float precision = 0.01f;
	int r, mx, my, dof;
	float rx, ry, ra, xo, yo, d, height;
	
	ra = pa;
	
	for(r=1-windowWidth/2;r<windowWidth/2;r++){
		ra = pa + degToRad(r-fov/2);
		rx = px;
		ry = py;
		
		float sinr = precision*sin(ra);
		float cosr = precision*cos(ra);
		float j = 0;
		
		while( 1 == 1){
			rx += cosr;
			ry += sinr;
			j += 1;
			if(map[(int)rx][(int)ry] != 0){
				int tile = map[(int)rx][(int)ry];
				d = j;
				j = j * cos(degToRad(r-fov/2));
				height = (10/j*2500);
				
				//choose colour to render wall//
				
				switch(tile){
					case 1:
						rR = 1;
						rG = 1;
						rB = 1;
						break;
						
					case 2:
						rR = 1;
						rG = 0.25;
						rB = 0.25;
						break;
						
					case 3:
						rR = 1;
						rG = 0.5;
						rB = 0;
						break;
						
					case 4:
						rR = 1;
						rG = 1;
						rB = 0;
						break;
						
					case 5:
						rR = 0;
						rG = 1;
						rB = 0;
						break;
						
					case 6:
						rR = 0;
						rG = 0;
						rB = 1;
						break;
						
					case 7:
						rR = 0.5;
						rG = 0;
						rB = 1.0;
						break;
						
					case 8:
						rR = 1;
						rG = 0;
						rB = 1;
						break;
				}
				
				
				break;
			}
		}
		
		if(d/2>255){
			d = 510;
		}
		
		glColor3f(rR-(15/height), rG-(15/height), rB-(15/height));
		glLineWidth(8);
		glBegin(GL_LINES);
		
		height += height;						
		
		glVertex2f(r*(1024/fov), 256 + height);
		glVertex2f(r*(1024/fov), 256 - height);
		
		glVertex2f(r*(1024/fov) + 8, 256 + height);
		glVertex2f(r*(1024/fov) + 8, 256 - height);
		
		glVertex2f(r*(1024/fov) + 10, 256 + height);
		glVertex2f(r*(1024/fov) + 10, 256 - height);
		glEnd();
	}
}

void buttonDown(unsigned char key,int x,int y)
{
	if(key=='w'){
		keys.w = 1;
	}
	if(key=='a'){
		keys.a = 1;
	}
	if(key=='s'){
		keys.s = 1;
	}
	if(key=='d'){
		keys.d = 1;
	}
	glutPostRedisplay();
}

void buttonUp(unsigned char key,int x,int y)
{
	if(key=='w'){
		keys.w = 0;
	}
	if(key=='a'){
		keys.a = 0;
	}
	if(key=='s'){
		keys.s = 0;
	}
	if(key=='d'){
		keys.d = 0;
	}
	glutPostRedisplay();
}

void display(){   

	frame2 = glutGet(GLUT_ELAPSED_TIME);
	fps = (frame2-frame1);
	frame1 = glutGet(GLUT_ELAPSED_TIME);
	
	/////////////////////
	if(keys.a == 1){
		pa -= 0.002f * fps;
	} 	
	if(keys.d ==1){
		pa += 0.002f * fps;
	} 
	if(keys.w == 1){
		px = px+0.002f*cos(pa) * fps;
		py = py+0.002f*sin(pa) * fps;
	}
	if(keys.s == 1){ 
		px = px-0.002f*cos(pa) * fps;
		py = py-0.002f*sin(pa) * fps;
	}
	////////////////////////
	glutPostRedisplay();
	
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	drawRays();
	
	glutSwapBuffers();  
	glFlush();
}

void init(){
	glClearColor(0,0,0.25f,0);
	gluOrtho2D(0,1024,512,0);
	
	px = 14;
	py = 14;
	pa = 0;
	fov=60;
	pdx=cos(degToRad(pa));
	pdy=-sin(degToRad(pa)); 
}

int main(int argc, char** argv){
	glutInit(&argc, argv);
 	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
 	glutInitWindowSize(1024,512);
 	glutCreateWindow("C Raycaster");
 	init();
 	glutDisplayFunc(display);
 	glutKeyboardFunc(buttonDown);
 	glutKeyboardUpFunc(buttonUp);
 	
 	glutMainLoop();
}
