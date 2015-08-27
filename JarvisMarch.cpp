#include <iostream>
#include <algorithm>
#include <cmath>
#include <vector>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <fstream>
#include <string>

#define FLOAT_MIN 0.000001		 
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
       		return ( x != p.x) && (y != p.y) ; 
     	}
	 
     	int operator == ( Point2D p ) 
     	{ 
       		return ( x == p.x) && ( y == p.y) ; 
     	}
     
	void Mark();
	void DrawLine( Point2D p );
};

//Drawing a point at the given location
void Point2D::Mark()
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
		
// To find orientation of ordered triplet (p, q, r).
// The function returns following values
// 0 --> p, q and r are collinear
// 1 --> Clockwise from p->q->r
// 2 --> Counterclockwise from p->q->r
int orientation(Point2D p, Point2D q, Point2D r)
{
    	float val = (q.Get_y() - p.Get_y()) * (r.Get_x() - q.Get_x()) - (q.Get_x() - p.Get_x()) * (r.Get_y() - q.Get_y());
	//std::cout << val << std::endl;
 
    	if (val == 0) 
		return 0;  		// collinear
	else if (val > 0)
    		return 1; 		// clock wise
	else
		return 2;		// counter clock wise
}

//Default intializer function
void init()
{
	glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
	glutInitWindowSize(1280,720);
	glutInitWindowPosition(0,0);
 	glutCreateWindow("Convex Hull - Jarvis March");     	
	glClearColor(0.392157, 0.584314, 0.929412, 0.0f);
     	glColor3f(0.0980392, 0.0980392, 0.439216);
     	glPointSize(4.0);
     	glMatrixMode(GL_PROJECTION);
     	glLoadIdentity();
     	gluOrtho2D(0,1280,0,720);
}

//Declare values to be used in BruteForce() and MouseClick() function
float x = 0, y = 0, z = 0, min_x = -1;
int n = 0, k = 0, ch = 0, ch_pts = 0, i, j, min_i = 0;
std::vector<Point2D> points, convexhull;
Point2D p, first_pt;
std::string file;

void intializer()
{
	x = y = z = 0; min_x = -1; min_i = n = k = ch_pts = 0;
}

void thanks()
{
	std::cout << "\n\nThanks for using the Jarvis March Convex Hull calculator!" << std::endl << std::endl;
}

//Finding the leftmost point in the given data set
void findmin()
{
	if( min_x == -1 )
	{
		min_x = x;
		first_pt = Point2D( x, y );
		min_i = k;
	}
			
	else if( x < min_x )
	{		
		min_x = x;
		first_pt = Point2D( x, y );
		min_i = k;
	}
}

void print_points()
{
	std::cout<<std::endl<<"Points are:"<<std::endl<<std::endl;				
	for (i=0; i < n; i++)
		std::cout<<"#"<<(i+1)<<"\t"<<"("<<points[i].Get_x()<<", "<<points[i].Get_y()<<")"<<std::endl;
}

void print_convexhull()
{
	std::cout<<std::endl<<"Convex Hull points are:"<<std::endl<<std::endl;				
	for (i=0; i < ch_pts; i++)
		std::cout<<"#"<<(i+1)<<"\t"<<"("<<convexhull[i].Get_x()<<", "<<convexhull[i].Get_y()<<")"<<std::endl;
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

//Implements the Jarvis March Convex Hull algorithm
void jarvis_march()
{
	//p, q equal to bottom most point (This is the starting point)
	p = first_pt;
	int pi = min_i, qi;
	convexhull.push_back(p);
	ch_pts++;

	do
	{	
		// Iterate through all points in points[]
		qi = (pi+1)%n;
		//Search for a point q such that orientation of (p, q , i) is clockwise or collinear for all i in points[] 
        	for (i = 0; i < n; i++)
		{
			//No point including p and q while searching
			if( i == pi || i == qi )
				continue;
			//If orientation is counter clock wise for a point then change q to new point i          		
			if ( orientation(points[pi], points[i], points[qi]) == 2 )
             			qi = i;
		}
		//Left ost point has already been added. No point adding it again
		if( points[qi] != first_pt )
		{
			//q has been found - add it to convex hull points
			convexhull.push_back(points[qi]);
			ch_pts++;
		}
		//Draw points and edges of the convex hull
		if((ch == 1) || (ch == 2))
		{								 
			p.DrawLine( points[qi] );
			p.Mark( );
			points[qi].Mark( );
		}
		
		//Make new point convex hull point the starting point for searching
		pi = qi;
		p = points[qi]; 
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
		jarvis_march();
	}

	else if(ch == 3)
	{
		std::random_shuffle ( points.begin(), points.end() );
		std::cout << "\nRead " << --k << " points from 'sample2.txt'" << std::endl;
		std::cout << "\nSampling "<< n <<" points from 'sample2.txt'" << std::endl;
		points.resize(n);
		min_x = points[0].Get_x();
		first_pt = points[0];
		for ( i = 1; i < n; i++)
			if(min_x > points[i].Get_x())
			{
				min_x = points[i].Get_x();
				min_i = i;
				first_pt = points[i];
			}
		jarvis_march();
	}

	print_convexhull();
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
			//When all points inputted - call jarvis_march() Algorithm
			else if(k == n)
			{ 
				print_points();
				k++;
				jarvis_march();
				print_convexhull();				
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

//Gives an option to input points either through MouseClick() or from an Input File - FileRead()
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
