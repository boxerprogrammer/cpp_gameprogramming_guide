#pragma once

struct Vector2 {
	float x;//Xç¿ïW
	float y;//Yç¿ïW
	void operator+=(const Vector2& vec);
	void operator-=(const Vector2& vec);
	void operator*=(float scalar);
	Vector2 operator+(const Vector2& vec) const;
	Vector2 operator-(const Vector2& vec) const;
	Vector2 operator*(float scalar) const;
	Vector2 operator/(float scalar) const;

};

using Position2 = Vector2;//à íu

struct Size {
	int width;//ïù
	int height;//çÇÇ≥
};
