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
	PerspectiveCamera(const Vector3D& eye, const Vector3D& front, const Vector3D& up, double fov, double w_h = 1)
		: _eye(eye)
		, _front(front.norm())
		, _refUp(up.norm())
		, _fov(fov) {
		_right = _front.cross(_refUp);
		_up = _right.cross(_front);

		if (w_h < 1) {
			_fovScaleH = std::tan(_fov*0.5*Math::PI / 180) * 2;
			_fovScaleV = _fovScaleH / w_h;
		} else {
			_fovScaleV = std::tan(_fov*0.5*Math::PI / 180) * 2;
			_fovScaleH = _fovScaleV * w_h;
		}
	}

	Ray3D generateRay(double x, double y) const {
		const Vector3D r = _right * ((x - 0.5)*_fovScaleH);
		const Vector3D u = _up * ((y - 0.5)*_fovScaleV);

		return Ray3D(_eye, (_front+r+u).norm());
	}

private:
	Vector3D _eye, _front, _refUp, _up, _right;
	double _fov, _fovScaleH, _fovScaleV;
};