#include <iostream>     
#include <algorithm>    
#include <vector>       
#include <ctime>        
#include <cstdlib>      
#include <fstream>

int main () 
{
	std::vector<int> x;
	std::vector<int> y;
	int k = 0;
	
	for (int i=0; i < 1280; ++i) 
		x.push_back(i); 
	
	for (int i=0; i < 720; ++i) 
		y.push_back(i); 

	std::random_shuffle ( x.begin(), x.end() );
	std::random_shuffle ( y.begin(), y.end() );

  	std::ofstream fptr;
  	fptr.open ("random_points.txt");
	fptr << "1000" << std::endl;
	
	for(int i = 0; i < 1000; ++i)
	{
		if( i < 720)
			fptr << x[i] << " " << y[i] << std::endl;
		else if ( i == 720 )
		{
			std::random_shuffle ( y.begin(), y.end() );
			fptr << x[i] << " " << y[k] << std::endl;
		}
		else
			fptr << x[i] << " " << y[++k] << std::endl;
	}
	fptr.close();
  	return 0;
}
