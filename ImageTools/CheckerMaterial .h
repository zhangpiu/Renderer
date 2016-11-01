////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (C)  2016-2099, ZJU.
// 
// File name:     CheckerMaterial.h
// 
// Author:        Piu Zhang
// 
// Version:       V1.0
// 
// Date:          2016.10.29
// 
// Description:   CheckerMaterial class.
// 
// Others: 
//
// Example:      
//
// 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include "Material.h"

class CheckerMaterial : public Material {
public:

	CheckerMaterial(double scale, double reflectiveness) 
		: Material(0, reflectiveness)
		, _scale(scale)
	{}

	virtual Color sample(const Ray3D& ray, const Vector3D& position, const Vector3D& normal) const;

private:
	double _scale;
};

Color CheckerMaterial::sample(const Ray3D& ray, const Vector3D& position, const Vector3D& normal) const {
	return std::abs((int)(std::floor(position.x() * 0.1) + std::floor(position.y() * _scale)) % 2) < 1 ? Color::BLACK : Color::WHITE;
}