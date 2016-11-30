////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (C)  2016-2099, ZJU.
// 
// File name:     PhongMaterial .h
// 
// Author:        Piu Zhang
// 
// Version:       V1.0
// 
// Date:          2016.10.29
// 
// Description:   Phong material  class.
//      
//
// 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include "Material.h"
#include <algorithm>

class PhongMaterial : public Material {
public:

	PhongMaterial(const Color& diffuse, const Color& specular, double shininess, double reflectiveness)
		: Material(shininess, reflectiveness)
		, _diffuse(diffuse)
		, _specular(specular)
	{}

	virtual Color sample(const Ray3D& ray, const LightSample& lightSample, const Vector3D& position, const Vector3D& normal) const;

private:
	Color _diffuse, _specular;
};


Color PhongMaterial::sample(const Ray3D& ray, const LightSample& lightSample, const Vector3D& position, const Vector3D& normal) const {
	
	// Using Phong Model
	const double NdotL = normal.dot(lightSample.L());
	const Vector3D H = (lightSample.L() - ray.getDirection()).norm();
	const double NdotH = normal.dot(H);
	const Color diffuseTerm = _diffuse   * std::max(NdotL, 0.0);
	const Color specularTerm = _specular * std::pow(std::max(NdotH, 0.0), _shininess);

	return lightSample.EL().modulate(diffuseTerm + specularTerm);
}
