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
