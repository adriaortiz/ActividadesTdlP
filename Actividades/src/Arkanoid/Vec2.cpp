//#include "Vec2.h"
//#include "..\inc\Vec2.h"
#include "..\..\dep\inc\Vec2.h"

Vec2::Vec2() : x(1), y(1) {}
Vec2::Vec2(int _x, int _y) : x(_x), y(_y) {}
Vec2::~Vec2() { x = 0; y = 0;}

int& Vec2::operator[](int id)
{
	switch (id)
	{
	case X:
		return x;
		break;
	case Y:
		return y;
		break;
	default:
		break;
	}
}

void operator+(Vec2 & vec1,const Vec2 & vec2){
	vec1.x += vec2.x;
	vec1.y += vec2.y;
}

void operator+=(Vec2 & vec1,const Vec2 & vec2){
	vec1.x += vec2.x;
	vec1.y += vec2.y;
}

bool operator==(const Vec2 & vec1,const Vec2 & vec2)
{
		return  vec1.x == vec2.x && vec1.y == vec2.y;
}

void operator*(Vec2 &vec1, const int & escalar)
{
	vec1.x *= escalar;
	vec1.y *= escalar;
}

void operator*=(Vec2 &vec1, const Vec2 & vec2)
{
	vec1.x *= vec2.x;
	vec1.y *= vec2.y;
}

bool operator!=(const Vec2 &vec1, const Vec2 & vec2)
{
	return vec1.x != vec2.x || vec1.y != vec2.y;
}



