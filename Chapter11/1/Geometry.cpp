#include "Geometry.h"
#include<cmath>//数学系関数ヘッダ
#include<cassert>//assert系が入ってるヘッダ
#include<DxLib.h>
void Vector2::Normalize() {
	float len = std::hypot(x, y);
	if (len == 0.0f) {
		x = 0.0f;
		y = 0.0f;
		return;
	}
	assert(len != 0.0f);
	x /= len;
	y /= len;
}
Vector2 Vector2::Normalized() const
{
	float len = Length();
	if (len == 0.0f) {
		return {};
	}
	return { x / len,y / len };
}
float Vector2::Length()const {
	return std::hypot(x, y);
}

Vector2 Vector2::operator-(const Vector2& right) const
{
	return { x - right.x,
			 y - right.y };
}

Vector2 Vector2::operator-() const
{
	return { -x,-y };
}

Vector2 Vector2::operator*(float scale)const
{
	return {x*scale,y*scale};
}

Vector2 Vector2::operator/(float div)const
{
	assert(div != 0.0f);
	return {x/div,y/div};
}

void Vector2::operator+=(const Vector2& vec)
{
	x += vec.x;
	y += vec.y;
}

void Vector2::operator-=(const Vector2& vec)
{
	x -= vec.x;
	y -= vec.y;
}

void Vector2::operator*=(float scale)
{
	x *= scale;
	y *= scale;
}

void Vector2::operator/=(float div)
{
	assert(div != 0.0f);
	x /= div;
	y /= div;
}

Vector2 operator+(const Vector2& left, const Vector2& right)
{
	return {left.x+right.x,
			left.y+right.y};
}

float Dot(const Vector2& lvec, const Vector2& rvec) {
	return lvec.x * rvec.x + lvec.y * rvec.y;
}


float DegreeToRadian(float degree)
{
	//度数法(0～360)→弧度法(0.0～2π)
	return (degree * DX_PI_F) / 180.0f;
}

void Circle::Draw(uint32_t color)
{
	DrawCircleAA(pos.x, pos.y,
		radius, 32, color,false,3.0f);
}
