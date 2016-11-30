////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (C)  2016-2099, ZJU.
// 
// File name:     DirectionalLight.h
// 
// Author:        Piu Zhang
// 
// Version:       V1.0
// 
// Date:          2016.10.28
// 
// Description:   平行光源
//
// 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Color.h"
#include "Vector3D.h"
#include "LightSample.h"
#include "Geometry.h"
#include "Ray3D.h"
#include "IntersectResult.h"
#include "Light.h"

class DirectionalLight : public Light {
public:
	DirectionalLight(const Color& irradiance, const Vector3D& direction)
		: _irradiance(irradiance)
		, _direction(direction)
		, _shadow(true) {
		_l = _direction.norm().negate();
	}

	virtual LightSample sample(const Geometry& scene, const Vector3D& position) const;

private:
	Color _irradiance;
	Vector3D _direction, _l;
	bool _shadow;
};

LightSample DirectionalLight::sample(const Geometry& scene, const Vector3D& position) const {
	if (_shadow) {
		const Ray3D shadowRay(position, _l);
		const auto shadowResult = scene.intersect(shadowRay);

		if (shadowResult.getGeometry()) {
			return LightSample::zero;
		}
	}

	return LightSample(_l, _irradiance);
}
