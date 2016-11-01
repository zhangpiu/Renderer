////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (C)  2016-2099, ZJU.
// 
// File name:     Ray3D.h
// 
// Author:        Piu Zhang
// 
// Version:       V1.0
// 
// Date:          2016.10.28
// 
// Description:   Ray
// 
// Others: 
//
// Example:      
//
// 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include "Vector3D.h"

class Ray3D {
public:
	Ray3D(const Vector3D& origin, const Vector3D& direction)
		: _origin(origin)
		, _direction(direction)
	{}

	Vector3D getPoint(double t) const { return _origin + _direction*t; }

	const Vector3D& getOrigin() const { return _origin; }

	const Vector3D& getDirection() const { return _direction; }

private:
	Vector3D _origin;
	Vector3D _direction;
};