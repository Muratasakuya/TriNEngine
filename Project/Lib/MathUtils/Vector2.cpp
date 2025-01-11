#include "Vector2.h"

//============================================================================*/
//	Vector2 classMethods
//============================================================================*/

Vector2 Vector2::operator+(const Vector2& other) const {
	return Vector2(x + other.x, y + other.y);
}
Vector2 Vector2::operator-(const Vector2& other) const {
	return Vector2(x - other.x, y - other.y);
}
Vector2 Vector2::operator*(const Vector2& other) const {
	return Vector2(x * other.x, y * other.y);
}
Vector2 Vector2::operator/(const Vector2& other) const {
	return Vector2(x / other.x, y / other.y);
}

Vector2& Vector2::operator+=(const Vector2& v) {
	x += v.x;
	y += v.y;
	return *this;
}
Vector2& Vector2::operator-=(const Vector2& v) {
	x += v.x;
	y += v.y;
	return *this;
}

Vector2 Vector2::operator*(float scalar) const {
	return Vector2(x * scalar, y * scalar);
}
Vector2 operator*(float scalar, const Vector2& v) {
	return Vector2(v.x * scalar, v.y * scalar);
}
Vector2 Vector2::operator/(float scalar) const {
	return Vector2(x / scalar, y / scalar);
}
Vector2 operator/(float scalar, const Vector2& v) {
	return Vector2(v.x / scalar, v.y / scalar);
}

Vector2& Vector2::operator*=(float scalar) {
	x *= scalar;
	y *= scalar;
	return *this;
}

bool Vector2::operator==(const Vector2& other) const {
	return x == other.x && y == other.y;
}
bool Vector2::operator!=(const Vector2& other) const {
	return !(*this == other);
}