////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (C)  2016-2099, ZJU.
// 
// File name:     LambertMaterial.h
// 
// Author:        Piu Zhang
// 
// Version:       V1.0
// 
// Date:          2016.10.28
// 
// Description:   Lambert material.
//
// 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once 

#include "Material.h"

class LambertMaterial : public Material {
public:
	LambertMaterial(Color diffuse)
		: _diffuse(diffuse)
	{}

	virtual Color sample(const Ray3D& ray, const LightSample& lightSample, const Vector3D& position, const Vector3D& normal) const;

private:
	Color _diffuse;
};


Color LambertMaterial::sample(const Ray3D& ray, const LightSample& lightSample, const Vector3D& position, const Vector3D& normal) const {
	const double NdotL = normal.dot(lightSample.L());

	return lightSample.EL().modulate(_diffuse*NdotL);
}
