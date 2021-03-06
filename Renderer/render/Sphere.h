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

	virtual double calcDistance(const Ray3D& ray) const;

	virtual pair<Vector3D, Vector3D> calcPositionAndNormal(const Ray3D& ray, double distance) const;

private:
	Vector3D _center;
	double _radius, _sqrRadius;

};


IntersectResult Sphere::intersect(const Ray3D& ray) const {
	// Solve t^2*d.d + 2*t*(o-c).d + (o-c).(o-c)-R^2 = 0; o: ray's origin, c: sphere's center
	Vector3D oc =  ray.getOrigin() - _center;
	double b = oc.dot(ray.getDirection());
	double det = b * b - oc.dot(oc) + _sqrRadius; // (b^2 - 4ac) / 4
	const double eps = 1e-6;

	if (det < 0) return IntersectResult::noHit;
	
	double dets = std::sqrt(det);
	double distance = 0;

	if (-b - dets > eps) distance = -b - dets;
	else if (-b + dets > eps) distance = -b + dets;
	else return IntersectResult::noHit;

	Vector3D position = ray.getPoint(distance);
	Vector3D normal = (position - _center).norm();

	return IntersectResult(this, distance, position, normal);
}


double Sphere::calcDistance(const Ray3D& ray) const {
	Vector3D oc = ray.getOrigin() - _center;
	double b = oc.dot(ray.getDirection());
	double det = b * b - oc.dot(oc) + _sqrRadius; // (b^2 - 4ac) / 4
	const double eps = 1e-6;

	if (det < 0) return numeric_limits<double>::max();

	double dets = std::sqrt(det);

	if (-b - dets > eps) return -b - dets;
	else if (-b + dets > eps) return -b + dets;
	else return numeric_limits<double>::max();
}


pair<Vector3D, Vector3D> Sphere::calcPositionAndNormal(const Ray3D& ray, double distance) const {
	assert(distance < numeric_limits<double>::max());

	Vector3D position = ray.getPoint(distance);
	Vector3D normal = (position - _center).norm();

	return{position, normal};
}