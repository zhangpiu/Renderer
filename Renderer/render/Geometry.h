////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (C)  2016-2099, ZJU.
// 
// File name:     Geometry.h
// 
// Author:        Piu Zhang
// 
// Version:       V1.0
// 
// Date:          2016.10.28
// 
// Description:   Abstract geometry class.
//    
//
// 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include "Ray3D.h"
#include "Material.h"
#include "IntersectResult.h"

class Geometry {
public:

	Geometry() {}

	Geometry(const std::shared_ptr<Material>& material) : _material(material) {}

	virtual IntersectResult intersect(const Ray3D& ray) const = 0;

	virtual double calcDistance(const Ray3D& ray) const = 0;

	virtual pair<Vector3D, Vector3D> calcPositionAndNormal(const Ray3D& ray, double distance) const = 0;

	virtual ~Geometry(){}

	const std::shared_ptr<Material>& getMaterial() const { return _material; }

	void setMaterial(const std::shared_ptr<Material>& material) { _material = material; }

private:
	std::shared_ptr<Material> _material;
};