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

	static Vector3D Zero;

	static Vector3D Xaxis;

	static Vector3D Yaxis;

	static Vector3D Zaxis;

	// add
	Vector3D& operator += (const Vector3D& rhs) {
		_x += rhs._x; _y += rhs._y; _z += rhs._z;
		return *this;
	}

	inline friend Vector3D operator + (const Vector3D& lhs, const Vector3D& rhs) {
		Vector3D ret(lhs);
		return ret += rhs;
	}

	// subtract
	Vector3D& operator -= (const Vector3D& rhs) {
		_x -= rhs._x; _y -= rhs._y; _z -= rhs._z;
		return *this;
	}

	inline friend Vector3D operator - (const Vector3D& lhs, const Vector3D& rhs) {
		Vector3D ret(lhs);
		return ret -= rhs;
	}

	// multiply
	Vector3D& operator *= (double value) {
		_x *= value; _y *= value; _z *= value;
		return *this;
	}

	inline friend Vector3D operator * (const Vector3D& lhs, double value) {
		Vector3D ret(lhs);
		return ret *= value;
	}

	inline friend Vector3D operator * (double value, const Vector3D& rhs) {
		return rhs * value;
	}

	// divide
	Vector3D& operator /= (double value) {
		double inv = 1.0 / value;
		_x *= inv; _y *= inv; _z *= inv;
		return *this;
	}

	inline friend Vector3D operator / (const Vector3D& lhs, double value) {
		Vector3D ret(lhs);
		return ret /= value;
	}

	// dot
	double dot(const Vector3D& rhs) const {
		return _x*rhs._x + _y*rhs._y + _z*rhs._z;
	}

	double operator % (const Vector3D& rhs) const {
		return dot(rhs);
	}

	// cross
	Vector3D cross(const Vector3D& rhs) const {
		double x = _y*rhs._z - _z*rhs._y;
		double y = _z*rhs._x - _x*rhs._z;
		double z = _x*rhs._y - _y*rhs._x;
		return Vector3D(x, y, z);
	}

	Vector3D& operator ^= (const Vector3D& rhs) {
		double x = _y*rhs._z - _z*rhs._y;
		double y = _z*rhs._x - _x*rhs._z;
		double z = _x*rhs._y - _y*rhs._x;
		_x = x; _y = y; _z = z;
		return *this;
	}

	inline friend Vector3D operator ^ (const Vector3D& lhs, const Vector3D& rhs) {
		Vector3D ret(lhs);
		return ret ^= rhs;
	}

	// output
	friend std::ostream& operator << (std::ostream& os, const Vector3D& rhs);

	double length() const { return std::sqrt(_x*_x + _y*_y + _z*_z); }
	
	double sqrLength() const { return _x*_x + _y*_y + _z*_z; }
	
	Vector3D norm() const {
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

Vector3D Vector3D::Zero = Vector3D(0,0,0);

Vector3D Vector3D::Xaxis = Vector3D(1,0,0);

Vector3D Vector3D::Yaxis = Vector3D(0,1,0);

Vector3D Vector3D::Zaxis = Vector3D(0,0,1);


// output
std::ostream& operator << (std::ostream& os, const Vector3D& rhs) {
	os << String::format("[x,y,z]: %f %f %f\n", rhs._x, rhs._y, rhs._z);
	
	return os;
}