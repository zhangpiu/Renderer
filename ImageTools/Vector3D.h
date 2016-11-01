////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (C)  2016-2099, ZJU.
// 
// File name:     Vector3D.h
// 
// Author:        Piu Zhang
// 
// Version:       V1.0
// 
// Date:          2016.10.28
// 
// Description:   3D vector
// 
// Others: 
//
// Example:      
//
// 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <cmath>
#include <iostream>
#include "String.h"

class Vector3D {
public:
	Vector3D() : _x(0), _y(0), _z(0) {}

	Vector3D(double x, double y, double z) : _x(x), _y(y), _z(z) {}

	static Vector3D zero;

	// add
	Vector3D& operator += (const Vector3D& rhs);

	friend Vector3D operator + (const Vector3D& lhs, const Vector3D& rhs);

	// subtract
	Vector3D& operator -= (const Vector3D& rhs);

	friend Vector3D operator - (const Vector3D& lhs, const Vector3D& rhs);

	// multiply
	Vector3D& operator *= (double value);

	friend Vector3D operator * (const Vector3D& lhs, double value);

	friend Vector3D operator * (double value, const Vector3D& rhs);

	// divide
	Vector3D& operator /= (double value);

	friend Vector3D operator / (const Vector3D& lhs, double value);

	// dot
	double dot(const Vector3D& rhs) const;

	// cross
	Vector3D cross(const Vector3D& rhs);

	// output
	friend std::ostream& operator << (std::ostream& os, const Vector3D& rhs);

	double length() const { return std::sqrt(_x*_x + _y*_y + _z*_z); }
	
	double sqrLength() const { return _x*_x + _y*_y + _z*_z; }
	
	Vector3D normalize() const {
		double inv = 1.0 / length();
		return Vector3D(_x*inv, _y*inv, _z*inv);
	}

	Vector3D negate() { return Vector3D(-_x, -_y, -_z); }

	double x() const { return _x; }

	double y() const { return _y; }

	double z() const { return _z; }

private:
	double _x, _y, _z;
};

Vector3D Vector3D::zero = Vector3D(0,0,0);

Vector3D& Vector3D::operator += (const Vector3D& rhs) {
	_x += rhs._x;
	_y += rhs._y;
	_z += rhs._z;
	return *this;
}


Vector3D operator + (const Vector3D& lhs, const Vector3D& rhs) {
	Vector3D ret(lhs);
	ret += rhs;
	return ret;
}


Vector3D& Vector3D::operator -= (const Vector3D& rhs) {
	_x -= rhs._x;
	_y -= rhs._y;
	_z -= rhs._z;
	return *this;
}

Vector3D operator - (const Vector3D& lhs, const Vector3D& rhs) {
	Vector3D ret(lhs);
	ret -= rhs;
	return ret;
}

Vector3D& Vector3D::operator *= (double value) {
	_x *= value;
	_y *= value;
	_z *= value;
	return *this;
}


Vector3D operator * (const Vector3D& lhs, double value) {
	Vector3D ret(lhs);
	ret *= value;
	return ret;
}


Vector3D operator * (double value, const Vector3D& rhs) {
	return rhs * value;
}


Vector3D& Vector3D::operator /= (double value) {
	double inv = 1.0/value;
	_x *= inv;
	_y *= inv;
	_z *= inv;
	return *this;
}


Vector3D operator / (const Vector3D& lhs, double value) {
	Vector3D ret(lhs);
	ret /= value;
	return ret;
}


double Vector3D::dot (const Vector3D& rhs) const{
	return _x*rhs._x + _y*rhs._y + _z*rhs._z;
}

Vector3D Vector3D::cross (const Vector3D& rhs) {
	const double x = _y*rhs._z - _z*rhs._y;
	const double y = _z*rhs._x - _x*rhs._z;
	const double z = _x*rhs._y - _y*rhs._x;

	return Vector3D(x,y,z);
}


// output
std::ostream& operator << (std::ostream& os, const Vector3D& rhs) {
	os << String::format("[x,y,z]: %f %f %f\n", rhs._x, rhs._y, rhs._z);
	
	return os;
}