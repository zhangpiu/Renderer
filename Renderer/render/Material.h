////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (C)  2016-2099, ZJU.
// 
// File name:     Material.h
// 
// Author:        Piu Zhang
// 
// Version:       V1.0
// 
// Date:          2016.10.29
// 
// Description:   Abstract material class.
//    
//
// 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include "Ray3D.h"
#include "Vector3D.h"
#include "Color.h"
#include "LightSample.h"

enum class IdealType;

class Material {
public:

	Material()
		: _shininess(0)
		, _reflectiveness(0)
	{}

	Material(double shininess, double reflectiveness)
		: _shininess(shininess)
		, _reflectiveness(reflectiveness)
	{}

	virtual ~Material(){}

	virtual Color sample(const Ray3D& ray, const LightSample& lightSample, const Vector3D& position, const Vector3D& normal) const = 0;

	double getShininess() const { return _shininess; }

	double getReflectiveness() const { return _reflectiveness; }

	virtual const IdealType& getIdealType() const { throw Exception("Illegal function call: 'Material' has no ideal type!"); }

	virtual const Color& getColor() const { throw Exception("Illegal function call: 'Material' has no color!"); }

	virtual const Color& getEmission() const { throw Exception("Illegal function call: 'Material' has no emission!"); }

protected:
	double _shininess;
	double _reflectiveness;
};