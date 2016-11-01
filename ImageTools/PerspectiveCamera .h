////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (C)  2016-2099, ZJU.
// 
// File name:     PerspectiveCamera.h
// 
// Author:        Piu Zhang
// 
// Version:       V1.0
// 
// Date:          2016.10.28
// 
// Description:   PerspectiveCamera
// 
// Others: 
//
// Example:      
//
// 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include "Vector3D.h"
#include "Math.h"
#include "Ray3D.h"

class PerspectiveCamera {
public:
	PerspectiveCamera(const Vector3D& eye, const Vector3D& front, const Vector3D& up, double fov)
		: _eye(eye)
		, _front(front.normalize())
		, _up(up.normalize())
		, _fov(fov) {
		_right = _front.cross(_up);
		_fovScale = std::tan(_fov*0.5*Math::PI/180)*2;
	}

	Ray3D generateRay(double x, double y) const {
		const Vector3D r = _right * ((x - 0.5)*_fovScale);
		const Vector3D u = _up * ((y - 0.5)*_fovScale);

		return Ray3D(_eye, (_front+r+u).normalize());
	}

private:
	Vector3D _eye, _front, _up, _right;
	double _fov, _fovScale;
};