#include "vector3f.h"
#include <iostream>
#include <iomanip>
#include <cmath>
#include <raymath.h>
#include <sstream>

using namespace psim;

const Vector3f Vector3f::ZERO = { 0, 0, 0 };

Vector3f::Vector3f(float x, float y, float z) {
	this->x = x;
	this->y = y;
	this->z = z;
};

Vector3f::Vector3f(const Vector3f& other) {
	this->x = other.x;
	this->y = other.y;
	this->z = other.z;
}

Vector3f::Vector3f(const Vector3& other) {
	this->x = other.x;
	this->y = other.y;
	this->z = other.z;
}

Vector3f Vector3f::operator+(const Vector3f& other) const {
	return Vector3f{ 
		this->x + other.x,
		this->y + other.y,
		this->z + other.z 
	};
}

void Vector3f::operator+=(const Vector3f& other) {
	this->x += other.x;
	this->y += other.y;
	this->z += other.z;
}

Vector3f Vector3f::operator-(const Vector3f& other) const {
	return Vector3f{ 
		this->x - other.x,
		this->y - other.y,
		this->z - other.z 
	};
}

void Vector3f::operator-=(const Vector3f& other) {
	this->x -= other.x;
	this->y -= other.y;
	this->z -= other.z;
}

float Vector3f::operator*(const Vector3f& other) const {
	return this->x * other.x +
		this->y * other.y +
		this->z * other.z;

}

Vector3f operator*(const float f, const Vector3f& v)
{
	return Vector3f{
		v.x * f,
		v.y * f,
		v.z * f,
	};
}

Vector3f Vector3f::operator*(const float& f) const {
	return Vector3f{
		this->x * f,
		this->y * f,
		this->z * f,
	};
}

void Vector3f::operator*=(const float& f) {
	this->x *= f;
	this->y *= f;
	this->z *= f;
}

Vector3f Vector3f::operator/(const float& f) const {
	return Vector3f{
		this->x / f,
		this->y / f,
		this->z / f
	};
}

void Vector3f::operator/=(const float& f) {
	this->x /= f;
	this->y /= f;
	this->z /= f;
}

Vector3f Vector3f::cross(const Vector3f& other) const {
	return Vector3f{ 
		this->y * other.z - this->z * other.y,
		this->z * other.x - this->x * other.z,
		this->x * other.y - this->y * other.x 
	};
}

float Vector3f::mag() const {
	return std::sqrt(x * x + y * y + z * z);
}

float Vector3f::mag2() const {
	return x * x + y * y + z * z;
}

Vector3f& Vector3f::operator=(const Vector3f& other) {
	this->x = other.x;
	this->y = other.y;
	this->z = other.z;
	return *this;
}

Vector3f Vector3f::normalize() const {

	float len = mag();
	if (len != 0.0f)
	{
		float n = 1.0f / len;

		Vector3f normal = *this;

		normal.x *= n;
		normal.y *= n;
		normal.z *= n;
	
		return normal;
	}
	else
	{
		return Vector3f::ZERO;
	}
	
}

float psim::Vector3f::angle(const Vector3f& other)
{

	float len1 = this->mag();
	float len2 = other.mag();

	if (len1 == 0 || len2 == 0)
		return 0;

	float dotproduct;
	float cos_angle;
	float angle;

	// cos(a) = (a.b) / |a|*|b|  
	dotproduct = (*this) * other;
	cos_angle = dotproduct / (this->mag() * other.mag());

	angle = acosf(cos_angle);
	return angle;
}

//class sign_padding :public std::num_put<char>
//{
//public:
//	// only for float and double
//	iter_type do_put(iter_type s, std::ios_base& f,
//		char_type fill, double v) const
//	{
//		if (std::signbit(v) == false)
//			*s++ = ' ';
//		return std::num_put<char>::do_put(s, f, fill, v);
//	}
//};

std::string psim::Vector3f::toString() const
{
	std::ostringstream oss;
	//oss.imbue(std::locale(oss.getloc(), new sign_padding));
	//oss << std::setfill('0') << std::setw(10);
	oss << "{ " << x << ", " << y << ", " << z << " }";

	return oss.str();
}

Vector3f psim::operator*(const float f, const Vector3f& v)
{
	return Vector3f{
		v.x * f,
		v.y * f,
		v.z * f
	};
}

std::ostream& psim::operator<<(std::ostream& out, const Vector3f& v)
{
	return out << v.toString();
}

// extra operators for compatibility with raylib's vector3

Vector3f operator+(const Vector3& a, const Vector3& b)
{
	return Vector3f{ a.x + b.x, a.y + b.y, a.z + b.z };
}

Vector3f operator-(const Vector3& a, const Vector3& b)
{
	return Vector3f{ a.x - b.x, a.y - b.y, a.z - b.z };
}

void operator+=(Vector3& a, const Vector3& b)
{
	a.x += b.x;
	a.y += b.y;
	a.z += b.z;
}

psim::Vector3f operator*(const Matrix& m, const Vector3& v)
{
	Vector3f ret = { 0 };

	ret.x = m.m0 * v.x + m.m4 * v.y + m.m8 * v.z;
	ret.y = m.m1 * v.x + m.m5 * v.y + m.m9 * v.z;
	ret.z = m.m2 * v.x + m.m6 * v.y + m.m10 * v.z;

	return ret;
}