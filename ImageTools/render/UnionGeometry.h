#pragma once

#include "Geometry.h"
#include <vector>
#include <limits>

using std::vector;
using std::shared_ptr;

class UnionGeometry : public Geometry {
public:

	UnionGeometry(const vector<shared_ptr<Geometry>>& geometries)
		: _geometries(geometries)
	{}

	virtual IntersectResult intersect(const Ray3D& ray) const;

private:
	vector<shared_ptr<Geometry>> _geometries;
};


IntersectResult UnionGeometry::intersect(const Ray3D& ray) const {
	double minDistance = std::numeric_limits<double>::max();
	IntersectResult minResult = IntersectResult::noHit;

	for (auto& geometry : _geometries) {
		auto result = geometry->intersect(ray);

		if (result.getGeometry() && result.getDistance() < minDistance) {
			minDistance = result.getDistance();
			minResult = result;
		}
	}

	return minResult;
}
