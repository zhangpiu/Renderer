////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (C)  2016-2099, ZJU.
// 
// File name:     Render.h
// 
// Author:        Piu Zhang
// 
// Version:       V1.0
// 
// Date:          2016.10.28
// 
// Description:   Render
// 
// Others: 
//
// Example:      
//
// 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include "Geometry.h"
#include "PerspectiveCamera .h"
#include "Matrix.h"
#include "Color.h"
#include "Math.h"
#include "Light.h"

#include <algorithm>

class Render {
public:
	static Matrix<uint8> renderDepth(const Geometry& scene, const PerspectiveCamera& camera, double maxDepth, const Size& size);

	static Matrix<uint8> renderNormal(const Geometry& scene, const PerspectiveCamera& camera, const Size& size);

	static Matrix<uint8> rayTrace(const Geometry& scene, const PerspectiveCamera& camera, const Size& size);

	static Matrix<uint8> rayTraceReflection(const Geometry& scene, const vector<shared_ptr<Light>>& lights, const PerspectiveCamera& camera, int maxReflect, const Size& size);

	static Matrix<uint8> renderLight(const Geometry& scene, const vector<shared_ptr<Light>>& lights, const PerspectiveCamera& camera, const Size& size);

private:
	static Color rayTraceRecursive(const Geometry& scene, const vector<shared_ptr<Light>>& lights, const Ray3D& ray, int maxReflect);

};


Matrix<uint8> Render::renderDepth(const Geometry& scene, const PerspectiveCamera& camera, double maxDepth, const Size& size) {
	Matrix<uint8> m(size);

	const int height = m.height();
	const int width = m.width();

	for (int i = 0; i < height; ++i) {
		const double sy = i / double(height);

		for (int j = 0; j < width; ++j) {
			const double sx = j / double(width);
			const Ray3D ray = camera.generateRay(sx, sy);
			const IntersectResult result = scene.intersect(ray);

			if (result.getGeometry()) {
				uint8 depth = uint8(255 - std::min(result.getDistance()/maxDepth*255, 255.0));

				m(i,j,0) = depth;
				m(i,j,1) = depth;
				m(i,j,2) = depth;
			}
		}
	}

	return std::move(m);
}


Matrix<uint8> Render::renderNormal(const Geometry& scene, const PerspectiveCamera& camera, const Size& size) {
	Matrix<uint8> m(size);

	const int height = m.height();
	const int width = m.width();

	for (int i = 0; i < height; ++i) {
		const double sy = 1 - i / double(height);

		for (int j = 0; j < width; ++j) {
			const double sx = j / double(width);
			const Ray3D ray = camera.generateRay(sx, sy);
			const IntersectResult result = scene.intersect(ray);

			if (result.getGeometry()) {
				m(i, j, 0) = uint8((result.getNormal().x() + 1) * 128);
				m(i, j, 1) = uint8((result.getNormal().y() + 1) * 128);
				m(i, j, 2) = uint8((result.getNormal().z() + 1) * 128);
			}
		}
	}

	return std::move(m);
}


Matrix<uint8> Render::rayTrace(const Geometry& scene, const PerspectiveCamera& camera, const Size& size) {
	Matrix<uint8> m(size);

	const int height = m.height();
	const int width = m.width();

	for (int i = 0; i < height; ++i) {
		const double sy = 1 - i / double(height);

		for (int j = 0; j < width; ++j) {
			const double sx = j / double(width);
			const Ray3D ray = camera.generateRay(sx, sy);
			const IntersectResult result = scene.intersect(ray);

			if (result.getGeometry()) {
				const Color clr; //= result.getGeometry()->getMaterial()->sample(ray, result.getPosition(), result.getNormal());

				m(i, j, 0) = (uint8)Math::clip(clr.r() * 255, 0.0, 255.0);
				m(i, j, 1) = (uint8)Math::clip(clr.g() * 255, 0.0, 255.0);
				m(i, j, 2) = (uint8)Math::clip(clr.b() * 255, 0.0, 255.0);
			}
		}
	}


	return std::move(m);
}



Color Render::rayTraceRecursive(const Geometry& scene, const vector<shared_ptr<Light>>& lights, const Ray3D& ray, int maxReflect) {
	const auto result = scene.intersect(ray);

	if (result.getGeometry()) {
		const auto material = result.getGeometry()->getMaterial();
		const double reflectiveness = material->getReflectiveness();
		vector<LightSample> lightSamples;

		for (auto& light : lights) {
			lightSamples.emplace_back(light->sample(scene, result.getPosition()));
		}

		Color clr = material->sample(ray, lightSamples, result.getPosition(), result.getNormal());

		clr *= 1 - reflectiveness;

		if (reflectiveness > 0 && maxReflect > 0) {
			const Vector3D& d = ray.getDirection();
			const Vector3D& n = result.getNormal();
			const Vector3D r = d - 2 * (d.dot(n)) * n;
			const Color reflectedClr = rayTraceRecursive(scene, lights, Ray3D(result.getPosition(), r), maxReflect-1);

			clr += reflectedClr * reflectiveness;
		}

		return clr;
	}

	return Color::BLACK;
}


Matrix<uint8> Render::rayTraceReflection(const Geometry& scene, const vector<shared_ptr<Light>>& lights, const PerspectiveCamera& camera, int maxReflect, const Size& size) {
	Matrix<uint8> m(size);

	const int height = m.height();
	const int width = m.width();

	for (int i = 0; i < height; ++i) {
		const double sy = 1 - i / double(height);

		for (int j = 0; j < width; ++j) {
			const double sx = j / double(width);
			const Ray3D ray = camera.generateRay(sx, sy);
			const Color clr = rayTraceRecursive(scene, lights, ray, maxReflect);
			const auto c = convert2Int(clr);

			m(i, j, 0) = std::get<0>(c);
			m(i, j, 1) = std::get<1>(c);
			m(i, j, 2) = std::get<2>(c);
		}
	}


	return std::move(m);
}



Matrix<uint8> Render::renderLight(const Geometry& scene, const vector<shared_ptr<Light>>& lights, const PerspectiveCamera& camera, const Size& size) {
	Matrix<uint8> m(size);

	const int height = m.height();
	const int width = m.width();

	for (int i = 0; i < height; ++i) {
		const double sy = 1 - i / double(height);

		for (int j = 0; j < width; ++j) {
			const double sx = j / double(width);
			const Ray3D ray = camera.generateRay(sx, sy);
			const auto result = scene.intersect(ray);

			if (result.getGeometry()) {
				Color clr = Color::BLACK;

				for (auto& light : lights) {
					auto lightSample = light->sample(scene, result.getPosition());

					if (&lightSample != &LightSample::zero) {
						double NdotL = result.getNormal().dot(lightSample.L());

						if (NdotL >= 0) {
							clr += lightSample.EL() * NdotL;
						}
					}
				}

				const auto c = convert2Int(clr);

				m(i, j, 0) = std::get<0>(c);
				m(i, j, 1) = std::get<1>(c);
				m(i, j, 2) = std::get<2>(c);
			}
		}
	}


	return std::move(m);
}
