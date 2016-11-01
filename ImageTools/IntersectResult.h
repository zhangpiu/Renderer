////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (C)  2016-2099, ZJU.
// 
// File name:     IntersectResult.h
// 
// Author:        Piu Zhang
// 
// Version:       V1.0
// 
// Date:          2016.10.28
// 
// Description:   Data structure of intersection detected.
// 
// Others: 
//
// Example:      
//
// 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include "Vector3D.h"

class Geometry;

class IntersectResult {
public:
	IntersectResult()
		: _distance(0)
		, _position(Vector3D::zero)
		, _normal(Vector3D::zero)
	{}

	IntersectResult(const shared_ptr<const Geometry>& geometry, double distance, const Vector3D& position, const Vector3D& normal)
		: _geometry(geometry)
		, _distance(distance)
		, _position(position)
		, _normal(normal)
	{}

	static const IntersectResult noHit;

	shared_ptr<const Geometry> getGeometry() const { return _geometry; }

	void setGeometry(const shared_ptr<const Geometry>& geometry) { _geometry = geometry; }

	double getDistance() const { return _distance; }

	void setDistance(double distance) { _distance = distance; }

	const Vector3D& getPosition() const { return _position; }

	void setPosition(const Vector3D& position) { _position = position; }

	const Vector3D& getNormal() const { return _normal; }

	void setNormal(const Vector3D& normal) { _normal = normal; }

private:
	shared_ptr<const Geometry> _geometry;
	double _distance;
	Vector3D _position, _normal;
};

const IntersectResult IntersectResult::noHit = IntersectResult();