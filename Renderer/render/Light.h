////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (C)  2016-2099, ZJU.
// 
// File name:     Light.h
// 
// Author:        Piu Zhang
// 
// Version:       V1.0
// 
// Date:          2016.10.28
// 
// Description:   Abstract light class.
//
// 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Geometry.h"
#include "LightSample.h"

class Light {
public:
	virtual LightSample sample(const Geometry& scene, const Vector3D& position) const = 0;
private:
};