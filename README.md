**Program**: Programs to implement Brute Force and Jarvis March Convex Hull algorithms and display the results using OpenGL 

**Author**: Anirudh Ravi

**About**:

These programs are written in C++. They use 2 algorithms to compute Convex Hull for a set of points input either through mouse click or through points declared in a file. It takes the input and stores the convex hull points in an OFF file. 

**Brute Force Algorithm**:

Given an input set S consisting of n points, S = {(x1, y1), (x2, y2), ... , (x n, yn)}. The brute force algorithm involves the following steps to compute the convex hull of S:

1. For a point p in S draw a line to another point q in S.
2. Let the equation of the line pq be ax + by – c =0
3. For all other points in S substitute their x and y coordinates into the equation of the line pq. It can evaluate to be 0, positive or negative
4. If all points in S lie on the same side of the line (eg. equation evaluates to being positive or 0 for all points in S) then line pq belongs
to the convex hull of S. Draw the line pq. Change p to q as the next convex hull point
5. If there is a change in side in which points occur for a particular line, then line does not belong to convex hull. Change q to the next point in S.
6. Repeat steps 2 to 5 until q becomes p itself
7. Repeat above steps for all points in S

**Jarvis March Algorithm**:

The Jarvis March algorithm is as follow:
1. Initialize p as leftmost point
2. Do the following while point is not the leftmost point:
- The next point q is the point such that the triplet (p, q, r) is
counter clockwise for any other point r in the input set
- Store q as next of p in the output convex hull
- Set p as q for next iteration, since q is the next convex hull point


**Executing**:

g++ ProgramName -lGL -lGLU -lglut -lm -o ExecutableName

./ExecutableName

[Ensure sample.txt and sample2.txt are present. RandomPointGenerators.cpp can be used to generate a set of 1000 points]
