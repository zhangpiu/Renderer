#pragma once

#include "Material.h"

enum class IdealType {
	DIFFUSE, SPECULAR, REFRACTIVE
};

class IdealMaterial : public Material {
public:
	IdealMaterial(const Color& color, const Color& emission, const IdealType& idealType = IdealType::DIFFUSE)
		: Material()
		, _color(color)
		, _emission(emission)
		, _type(idealType)
	{}

	virtual Color sample(const Ray3D& ray, const LightSample& lightSample, const Vector3D& position, const Vector3D& normal) const { return Color::BLACK; }
	
	virtual const IdealType& getIdealType() const { return _type; }

	virtual const Color& getColor() const { return _color; }

	virtual const Color& getEmission() const { return _emission; }

private:
	Color _color, _emission;
	IdealType _type;
};