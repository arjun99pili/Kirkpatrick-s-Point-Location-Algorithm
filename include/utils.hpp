#include<iostream>
#include <math.h>
#include <fstream>
#include <vector>
#include <cmath>
#define PI 3.14159265358979323846

class Vector
{
public:
	float x;
	float y;
	float z;
	float length;
	Vector crossProduct(Vector v);
	void normalize();
	void calculateLength();
	Vector(float x1,float y1,float z1);
	
};

class Quaternion{
	public:	
		Quaternion();
		~Quaternion();
		void CreateMatrix(float *pMatrix);
		void CreateFromAxisAngle(const float &in_x,
		 const float &in_y,
		 const float &in_z,
		 const float &in_degrees);
		Quaternion operator *(const Quaternion &q);
	
		float x, y, z, w;
};