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
// Description:   Geometry class.
// 
// Others: 
//
// Example:      
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

	virtual ~Geometry(){}

	const std::shared_ptr<Material>& getMaterial() const { return _material; }

	void setMaterial(const std::shared_ptr<Material>& material) { _material = material; }

private:
	std::shared_ptr<Material> _material;
};