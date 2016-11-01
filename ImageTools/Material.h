////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (C)  2016-2099, ZJU.
// 
// File name:     Material.h
// 
// Author:        Piu Zhang
// 
// Version:       V1.0
// 
// Date:          2016.10.29
// 
// Description:   Material class.
// 
// Others: 
//
// Example:      
//
// 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include "Ray3D.h"
#include "Vector3D.h"
#include "Color.h"

class Material {
public:

	Material(double shininess, double reflectiveness)
		: _shininess(shininess)
		, _reflectiveness(reflectiveness)
	{}

	virtual ~Material(){}

	virtual Color sample(const Ray3D& ray, const Vector3D& position, const Vector3D& normal) const = 0;

	double getShininess() const { return _shininess; }

	double getReflectiveness() const { return _reflectiveness; }

protected:
	double _shininess;
	double _reflectiveness;
};