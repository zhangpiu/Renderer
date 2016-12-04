////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (C)  2016-2099, ZJU.
// 
// File name:     UnionGeometry.h
// 
// Author:        Piu Zhang
// 
// Version:       V1.0
// 
// Date:          2016.10.29
// 
// Description:   
//    
//
// 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Geometry.h"
#include <vector>
#include <limits>

using std::vector;
using std::shared_ptr;

class UnionGeometry : public Geometry {
public:

	UnionGeometry(const vector<shared_ptr<Geometry>>& geometries)
		: _geometries(geometries)
	{}

	virtual IntersectResult intersect(const Ray3D& ray) const;

	virtual double calcDistance(const Ray3D& ray) const { throw Exception("Illegal function call: 'UnionGeometry' is an abstract class!"); }

	virtual pair<Vector3D, Vector3D> calcPositionAndNormal(const Ray3D& ray, double distance) const { throw Exception("Illegal function call: 'UnionGeometry' is an abstract class!"); }

	void add(const shared_ptr<Geometry>& geometry) { _geometries.push_back(geometry); }

	vector<shared_ptr<Geometry>> getAll() const { return _geometries; }

private:
	vector<shared_ptr<Geometry>> _geometries;
};


IntersectResult UnionGeometry::intersect(const Ray3D& ray) const {
// 	double minDistance = std::numeric_limits<double>::max();
// 	IntersectResult minResult = IntersectResult::noHit;
// 
// 	for (auto& geometry : _geometries) {
// 		IntersectResult&& result = geometry->intersect(ray);
// 
// 		if (result.getGeometry() && result.getDistance() < minDistance) {
// 			minDistance = result.getDistance();
// 			minResult = result;
// 		}
// 	}
// 
// 	return minResult;

	double minDist = std::numeric_limits<double>::max();
	int idx = -1;

	for (size_t i = 0; i < _geometries.size(); ++i) {
		double dist = _geometries[i]->calcDistance(ray);
		if (dist < minDist) {
			minDist = dist;
			idx = (int)i;
		}
	}

	if (idx == -1) return IntersectResult::noHit;
	else {
		auto&& r = _geometries[idx]->calcPositionAndNormal(ray, minDist);
		return IntersectResult(_geometries[idx].get(), minDist, r.first, r.second);
	}
}
