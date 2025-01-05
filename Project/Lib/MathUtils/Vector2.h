#pragma once

//============================================================================*/
//	Vector2 class
//============================================================================*/
class Vector2 final {
public:

	float x, y;

	Vector2() : x(0.0f), y(0.0f) {}
	Vector2(float x, float y) : x(x), y(y) {}

	//========================================================================*/
	//	operators
	//========================================================================*/

	Vector2 operator+(const Vector2& other) const;
	Vector2 operator-(const Vector2& other) const;
	Vector2 operator*(const Vector2& other) const;
	Vector2 operator/(const Vector2& other) const;

	Vector2& operator+=(const Vector2& v);
	Vector2& operator-=(const Vector2& v);

	Vector2 operator*(float scalar) const;
	friend Vector2 operator*(float scalar, const Vector2& v);

	Vector2 operator/(float scalar) const;
	friend Vector2 operator/(float scalar, const Vector2& v);

	Vector2& operator*=(float scalar);

	bool operator==(const Vector2& other) const;
	bool operator!=(const Vector2& other) const;

};