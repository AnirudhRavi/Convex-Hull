#include <iostream>
#include <algorithm>
#include <cmath>
#include <vector>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <GL/glui.h>
#include <stdlib.h>
#include <fstream>
#include <string>
		 
//For checking if a number lies within a given range
int IsBetween( int a, int low, int high )
{
	return ( a >=low && a <=high );
}

//Point class for storing points and drawing points and lines
class Point2D
{
	 float x, y;
 
 public:
	Point2D( ) 
     	{ 
       		x = 0;
       		y = 0; 
     	}
     
	Point2D( float px, float py ) 
     	{ 
       		x = px; 
       		y = py; 
     	}	
     
     	float Get_x( ) { return x; }
	float Get_y( ) { return y; }
	 
     	int operator != ( Point2D p ) 
     	{ 
       		return ( x != p.x) && ( y != p.y) ; 
     	}
	 
     	int operator == ( Point2D p ) 
     	{ 
       		return ( x == p.x) && ( y == p.y) ; 
     	}
     
	void Mark();
	void DrawLine( Point2D p );
};

//Line class for creating a line of the form ax + by = c
class Line
{
	 float a,b,c;
	 
  public:
	Line( ) 
        { 
        	a = 0;
        	b = 0;
           	c = 0; 
        }
         
	Line( Point2D p, Point2D q ) 
        { 
           	Make( p,q ); 
        }
         
	void Make( Point2D p, Point2D q );
	int Evaluate( Point2D p );
};
 	 
//Drawing a point at the given location
void Point2D::Mark( void )
{
    	glBegin(GL_POINTS);
      		glVertex2i( x, 720 - y );
    	glEnd();
    	glutSwapBuffers();
}
		
// Draws a line between in the 2 points given
void Point2D::DrawLine( Point2D p )
{
    	glBegin(GL_LINES);
     		glVertex2f( x , 720 - y );
     		glVertex2f( p.x , 720 - p.y );
    	glEnd();
    	glutSwapBuffers();
}
		
//Creates a line equation for the given 2 points
void Line::Make( Point2D p, Point2D q )
{
	 a = q.Get_y( ) - p.Get_y( );
	 b = - (q.Get_x( ) - p.Get_x( ));
	 c = ( (float)p.Get_x( ) * ( q.Get_y( ) - p.Get_y( ) )) - ( (float)p.Get_y( ) * ( q.Get_x( ) - p.Get_x( ) ));
}

// Evaluates the equation of the line at the given point and returns
//	 1 if aX + bY > c
//	 0 if aX + bY = c
//	-1 if aX + bY < c
int Line::Evaluate( Point2D p )
{
	 float val = a*p.Get_x( ) + b*p.Get_y( );
	 if( val > c )
	     	return 1;
	 else if( val == c )
         	return 0;
	 else
	     	return -1;
}

//Default intializer function
void init()
{
	glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
	glutInitWindowSize(1280,720);
	glutInitWindowPosition(0,0);
 	glutCreateWindow("Convex Hull - Brute Force");     	
	glClearColor(0.392157, 0.584314, 0.929412, 0.0f);
     	glColor3f(0.0980392, 0.0980392, 0.439216);
	glPointSize(4.0);
     	glMatrixMode(GL_PROJECTION);
     	glLoadIdentity();
     	gluOrtho2D(0,1280,0,720);
}

//Declare values to be used in BruteForce() and MouseClick() function
float x = 0, y = 0, z = 0, min_y = -1;
int n = 0, k = 0, ch = 0, ch_pts = 0, i, j, min_i = 0, side, prev_side;
std::vector<Point2D> points, convexhull;
Point2D p, first_pt;// q;
Line l;
std::string file;

void intializer()
{
	x = y = z = 0; min_y = -1; min_i = n = k = ch_pts = 0;
}

void thanks()
{
	std::cout << "\n\nThanks for using the Brute Force Convex Hull calculator!" << std::endl << std::endl;
}

void points_print()
{
	std::cout<<std::endl<<"Points are:"<<std::endl<<std::endl;				
	for (i=0; i < n; i++)
		std::cout<<"#"<<(i+1)<<"\t"<<"("<<points[i].Get_x()<<", "<<points[i].Get_y()<<")"<<std::endl;
}

void convexhull_print()
{
	std::cout<<std::endl<<"Convex Hull points are:"<<std::endl<<std::endl;				
	for (i=0; i < ch_pts; i++)
		std::cout<<"#"<<(i+1)<<"\t"<<"("<<convexhull[i].Get_x()<<", "<<convexhull[i].Get_y()<<")"<<std::endl;
}

//Finding the leftmost point in the given data set
void findmin()
{
	if( min_y == -1 )
	{
		min_y = y;
		first_pt = Point2D( x, y );
		min_i = k;
	}
			
	else if( y < min_y )
	{		
		min_y = y;
		first_pt = Point2D( x, y );
		min_i = k;
	}
}

//Write the convex hull points into an OFF file
void FileWrite()
{
	convexhull.resize(ch_pts);
	std::ofstream ofptr("off_file.off", std::ofstream::out);
	if(ofptr.is_open())
	{
		ofptr << "OFF" << std::endl;
		ofptr << ch_pts << " 1" << " " << ch_pts <<std::endl;
		for( i = 0 ; i < ch_pts ; i++ )
			ofptr << convexhull[i].Get_x() << " " << convexhull[i].Get_y() << " 0" << std::endl;
		ofptr << ch_pts; 	
		for( i = 0 ; i < ch_pts ; i++ )
			ofptr << " " << i;
		ofptr << " 1.000 0.000 0.000 1.000";
	}
	ofptr.close();
	thanks();
	std::cout << "Your OFF file (off_file.off) is ready" << std::endl << std::endl;
}

//Implements the Brute Force Convex Hull algorithm
void BruteForce()
{
	//p, q equal to bottom most point (This is the starting point)
	p = first_pt;
	int pi = min_i, qi = min_i;
	convexhull.push_back(p);
	ch_pts++;

	do
	{
		//For all points
		for( i = 0; i < n ; i++ )
		{
			//Don't include p, q as points to form a line with			 
			if( i == pi || i == qi )
			 	continue;
			//Create the required line
			l.Make( p, points[i] );
			prev_side = 0;
			// Check if all the points are on the same side of the line
			for( j = 0; j < n ; j++ )
			{
				if( j == i || j == pi )
					continue;
				side = l.Evaluate( points[j] );
				if( prev_side == 0 )
					prev_side = side;
				//If one point lies to the left and the other to the right then line not part of convex hull 
				else if( ( prev_side == 1 && side == -1 ) || ( prev_side == -1 && side == 1 ) ) 
					break;
			 }
			 
			//If a break doesn't occur then line part of convex hull. Draw line
			if( j == n && side == prev_side )
			{
				if((ch == 1) || (ch == 2))
				{								 
					p.DrawLine( points[i] );
					p.Mark( );
					points[i].Mark( );
				}
				if( i != min_i)
				{
					convexhull.push_back(points[i]);
					ch_pts++;
				}
				//q = p;
				qi = pi;
				//Make new point the starting point for finding next line of convex hull
				pi = i;
				p = points[i]; 
				break;
			 }
		}
	}while( pi != min_i );
}

//Read points from given 'sample.txt' file
void FileRead()
{
	std::ifstream ifptr(file.c_str(), std::ifstream::in);
	if ( ifptr.is_open() ) 
	{
		while( !ifptr.eof() )
		{
			ifptr >> x; ifptr >> y; 
			if(ch == 3)
			{
				ifptr >> z;
				//y = z;
			}
			
			points.push_back( Point2D(x, y) );

			if(ch == 1 || ch == 2)
				findmin();
		
			k++;
		}
		ifptr.close();
	}

	if(ch == 2)
	{
		n = --k;
		std::cout << "\nRead " << n << " points from 'sample.txt'" << std::endl;
		points.resize(n);
		BruteForce();
	}

	else if(ch == 3)
	{
		std::random_shuffle ( points.begin(), points.end() );
		std::cout << "\nRead " << --k << " points from 'sample2.txt'" << std::endl;
		std::cout << "\nSampling "<< n <<" points from 'sample2.txt'" << std::endl;
		points.resize(n);
		min_y = points[0].Get_y();
		first_pt = points[0];
		for ( i = 1; i < n; i++)
			if(min_y > points[i].Get_y())
			{
				min_y = points[i].Get_y();
				min_i = i;
				first_pt = points[i];
			}
		BruteForce();
	}

	convexhull_print();
	FileWrite();
}

//Store points registered through mouse click	
void MouseClick( int button , int state , int s , int t )
{   
	//Check for left click of Mouse	
	if(state==GLUT_DOWN)
	{
		if(button==GLUT_LEFT_BUTTON)
		{
			//Should be less than total number of points			
			if(k < n)
			{
				//Set points[k] to point inputted through screen
				x = s, y = t;				
				points[k] = Point2D( (float)x, (float)y );
				findmin();
				points[k].Mark( );
				k++;
			}
			//When all points inputted - call BruteForce() Algorithm
			else if(k == n)
			{ 
				points_print();
				k++;
				BruteForce();
				convexhull_print();
				FileWrite();
			}
		}
	}
}

//Default display function
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	
	if(ch == 2)
	{
		FileRead();
		ch = 0;
	}
}

int main( int argc , char **argv )
{
	glutInit(&argc,argv);
	do
	{
		intializer();	
		std::cout << "\nEnter your choice: " << std::endl;
		std::cout << "1. Mouse Click " << std::endl;
		std::cout << "2. Read from 'sample.txt' " << std::endl;
		std::cout << "3. Sample from 'sample2.txt' " << std::endl;
		std::cout << "4. EXIT" << std::endl;
		std::cout << "Enter your choice: ";
		std::cin >> ch;
			
		switch(ch)
		{
		case 1:		
			std::cout << "\nEnter the number of points (max. Anything you want. Your the one clicking): ";
			std::cin >> n;
			points.resize(n);
			init();
			glutMouseFunc(MouseClick);
			glutDisplayFunc(display);
		 	glutMainLoop();
		break;

		case 2:
			file = "sample.txt";
			init();
			glutDisplayFunc(display);
			glutMainLoop();
		break;

		case 3:
			file = "sample2.txt";
			do
			{
				std::cout << "\nEnter the number of points you want to sample (max. 4,74,343): ";
				std::cin >> n;
				if(n > 474343)
					std::cout << "\n\nSorry. That was an incorrect option. Try again." << std::endl;
			}while(n > 474343);
			FileRead();
			exit(0);
		break;

		case 4:
			thanks();
			exit(0);
		break;

		default:
			std::cout << "\n\nSorry. That was an incorrect option. Try again." << std::endl << std::endl;	
		}
	}while(ch != 4);
	return 0;
}
