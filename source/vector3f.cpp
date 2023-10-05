#include "vector3f.h"
#include <iostream>
#include <cmath>

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
	return sqrt(x * x + y * y + z * z);
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
	float n = isqrt(x * x + y * y + z * z);
	// float n = 1.0f / std::sqrtf(this->mag2());

	Vector3f normal = *this;

	normal.x *= n;
	normal.y *= n;
	normal.z *= n;
	
	return normal;
	
}

float Vector3f::isqrt(float f) const {
	long i;
	float x2, y;
	const float threehalfs = 1.5F;

	x2 = f * 0.5F;
	y = f;
	i = *(long*)&y;                       // evil floating point bit level hacking
	i = 0x5f3759df - (i >> 1);               // what the fuck?
	y = *(float*)&i;
	y = y * (threehalfs - (x2 * y * y));   // 1st iteration
	// y  = y * ( threehalfs - ( x2 * y * y ) );   // 2nd iteration, this can be removed

	return y;
}