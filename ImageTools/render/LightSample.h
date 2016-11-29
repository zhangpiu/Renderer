////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (C)  2016-2099, ZJU.
// 
// File name:     LightSample.h
// 
// Author:        Piu Zhang
// 
// Version:       V1.0
// 
// Date:          2016.10.28
// 
// Description:   
//
// 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Vector3D.h"
#include "Color.h"

class LightSample {
public:
	LightSample(const Vector3D& l, const Color& el)
		: _l(l)
		, _el(el)
	{}

	static const LightSample zero;

	const Vector3D& L() const { return _l; }

	const Color& EL() const { return _el; }

private:
	Vector3D _l;
	Color _el;
};

const LightSample LightSample::zero = LightSample(Vector3D::Zero, Color::BLACK);