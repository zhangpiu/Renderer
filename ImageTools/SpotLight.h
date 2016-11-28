#pragma once

#include "Light.h"
#include "Math.h"

class SpotLight : public Light {
public:

	SpotLight(const Color& intensity, const Vector3D& position, const Vector3D& direction, double theta, double phi, double falloff)
		: _intensity(intensity)
		, _position(position)
		, _direction(direction)
		, _theta(theta)
		, _phi(phi)
		, _falloff(falloff)
		, _shadow(true) {
		_S = _direction.norm().negate();
		_cosTheta = std::cos(_theta * Math::PI * 0.5 / 180);
		_cosPhi = std::cos(_phi * Math::PI * 0.5 / 180);
		_baseMultiplier = 1 / (_cosTheta - _cosPhi);
	}

	virtual LightSample sample(const Geometry& scene, const Vector3D& position) const;


private:
	Color _intensity;
	Vector3D _position, _direction, _S;
	double _theta, _phi, _falloff, _cosTheta, _cosPhi, _baseMultiplier;
	bool _shadow;
};


LightSample SpotLight::sample(const Geometry& scene, const Vector3D& position) const {
	const Vector3D delta = _position - position;
	const double rr = delta.sqrLength();
	const double r = std::sqrt(rr);
	const Vector3D L = delta / r;

	const double SdotL = _S.dot(L);
	double spot = 0;

	if (SdotL >= _cosTheta) spot = 1;
	else if (SdotL < _cosPhi) spot = 0;
	else spot = std::pow((SdotL - _cosPhi)*_baseMultiplier, _falloff);

	if (_shadow) {
		const Ray3D shadowRay(position, L);
		const auto shadownResult = scene.intersect(shadowRay);

		if (shadownResult.getGeometry() && shadownResult.getDistance() <= r) {
			return LightSample::zero;
		}
	}

	const double attenuation = 1 / rr;
	return LightSample(L, _intensity*(attenuation*spot));
}
