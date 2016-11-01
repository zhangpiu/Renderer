////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (C)  2016-2099, ZJU.
// 
// File name:     Sphere.h
// 
// Author:        Piu Zhang
// 
// Version:       V1.0
// 
// Date:          2016.10.28
// 
// Description:   Sphere
// 
// Others: 
//
// Example:      
//
// 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include "Vector3D.h"
#include "IntersectResult.h"
#include "Ray3D.h"
#include "Geometry.h"

class Sphere : public Geometry {
public:
	Sphere(const Vector3D& center, double radius, const shared_ptr<Material>& material = nullptr)
		: Geometry(material)
		,_center(center)
		, _radius(radius)
		, _sqrRadius(radius*radius)
	{}

	virtual IntersectResult intersect(const Ray3D& ray) const;

private:
	Vector3D _center;
	double _radius, _sqrRadius;

};


IntersectResult Sphere::intersect(const Ray3D& ray) const {
	const Vector3D v = ray.getOrigin() - _center;
	const double a = v.sqrLength() - _sqrRadius;
	const double d_dot_v = ray.getDirection().dot(v);

	if (d_dot_v <= 0) {
		const double discr = d_dot_v*d_dot_v - a;

		if (discr >= 0) {
			const double distance = -d_dot_v-std::sqrt(discr);
			const Vector3D position = ray.getPoint(distance);
			const Vector3D normal = (position - _center).normalize();

			return IntersectResult(shared_from_this(), distance, position, normal);
		}
	}

	return IntersectResult::noHit;
}
