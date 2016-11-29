#pragma once

#include "Geometry.h"
#include "LightSample.h"

class Light {
public:
	virtual LightSample sample(const Geometry& scene, const Vector3D& position) const = 0;
private:
};