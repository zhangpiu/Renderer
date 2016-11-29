////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (C)  2016-2099, ZJU.
// 
// File name:     PointLight.h
// 
// Author:        Piu Zhang
// 
// Version:       V1.0
// 
// Date:          2016.10.29
// 
// Description:   µ„π‚‘¥
//    
//
// 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Light.h"

class PointLight : public Light {
public:

	PointLight(const Color& intensity, const Vector3D& position)
		: _intensity(intensity)
		, _position(position)
		, _shadow(true)
	{}

	virtual LightSample sample(const Geometry& scene, const Vector3D& position) const;

private:
	Color _intensity;
	Vector3D _position;
	bool _shadow;
};



LightSample PointLight::sample(const Geometry& scene, const Vector3D& position) const {
	const Vector3D delta = _position - position;
	const double rr = delta.sqrLength();
	const double r = std::sqrt(rr);
	const Vector3D L = delta / r;

	if (_shadow) {
		const Ray3D shadowRay(position, L);
		const auto shadowResult = scene.intersect(shadowRay);

		if (shadowResult.getGeometry() && shadowResult.getDistance() <= r) {
			return LightSample::zero;
		}
	}

	const double attenuation = 1/rr;

	return LightSample(L, _intensity * attenuation);
}
