#pragma once

#include "Vector3D.h"
#include "Geometry.h"


class Plane : public Geometry {
public:
	Plane(const Vector3D& normal, double d, const std::shared_ptr<Material>& material = nullptr)
		: Geometry(material)
		,_normal(normal)
		, _position(normal*d)
		, _d(d)
	{}

	virtual IntersectResult intersect(const Ray3D& ray) const;

private:
	Vector3D _normal, _position;
	double _d;
};


IntersectResult Plane::intersect(const Ray3D& ray) const {
	const double a = ray.getDirection().dot(_normal);
	if (a >= 0) return IntersectResult::noHit;

	const double b = _normal.dot(ray.getOrigin() - _position);

	IntersectResult result;
	result.setGeometry(shared_from_this());
	result.setDistance(-b/a);
	result.setPosition(ray.getPoint(result.getDistance()));
	result.setNormal(_normal);

	return result;
}
