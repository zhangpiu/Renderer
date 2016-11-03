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
// Description:   PhongMaterial  class.
// 
// Others: 
//
// Example:      
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

	virtual Color sample(const Ray3D& ray, const vector<LightSample>& lightSamples, const Vector3D& position, const Vector3D& normal) const;

private:
	Color _diffuse, _specular;
};


// Global illumination
const Vector3D lightDir = Vector3D(1, 0, 2).normalize();
const Vector3D lightDir2 = Vector3D(1, 0, 0).normalize();
const Color lightColor = Color::WHITE;

const vector<Vector3D> lights{lightDir, lightDir2};

Color PhongMaterial::sample(const Ray3D& ray, const vector<LightSample>& lightSamples, const Vector3D& position, const Vector3D& normal) const {
	
	// Using Phong Model
	Color clr;

	for (auto& lightSample : lightSamples) {
		const double NdotL = normal.dot(lightSample.L());
		const Vector3D H = (lightSample.L() - ray.getDirection()).normalize();
		const double NdotH = normal.dot(H);
		const Color diffuseTerm = _diffuse   * std::max(NdotL, 0.0);
		const Color specularTerm = _specular * std::pow(std::max(NdotH, 0.0), _shininess);

		clr += lightSample.EL().modulate(diffuseTerm + specularTerm);
	}

	return clr;
}
