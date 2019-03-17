#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <cmath>
#include <iostream>
#include <fstream>

using namespace std;

class Point
{
public:
	float x;
	float y;
	Point () {
		x = 0.0;
		y = 0.0;
	}
	Point& operator=(Point const &rhs) {
		this->x = rhs.x;
		this->y = rhs.y;
		return *this;
	}
	bool operator==(Point const &rhs) {
		return this->x == rhs.x && this->y == rhs.y;
	}
	Point operator+(Point const &rhs) {
		this->x += rhs.x;
		this->y += rhs.y;
		return *this;
	}
};

class Curve
{
public:
	int ptNum;

	int id;
	int type;
	int res;
	int k;
	float r;
	float g;
	float b;
	int stroke;
	
	vector<Point> pts;
	vector<float> knots;

	Curve() {
		id = -1;
		type = -1;
		ptNum = 0;
		res = 10;
		k = 3;
		r = 0;
		g = 0;
		b = 0;
		stroke = 1;
	}
	bool operator==(Curve const &rhs) {
		return this->id == rhs.id;
	}

	
};