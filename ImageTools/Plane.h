#pragma once

#include "Vector3D.h"
#include "Geometry.h"


class Plane : public Geometry {
public:
	Plane(const Vector3D& normal, double d, const std::shared_ptr<Material>& material = nullptr)
		: Geometry(material)
		,_normal(normal)
		, _position(normal*d)
	{}

	virtual IntersectResult intersect(const Ray3D& ray) const;

private:
	Vector3D _normal, _position;
};


IntersectResult Plane::intersect(const Ray3D& ray) const {
	const double a = ray.getDirection().dot(_normal);
	if (a >= 0) return IntersectResult::noHit;

	const double b = _normal.dot(ray.getOrigin() - _position);

	IntersectResult result;
	double dist = -b/a;

	result.setGeometry(this);
	result.setDistance(dist);
	result.setPosition(ray.getPoint(dist));
	result.setNormal(_normal);

	return result;
}
