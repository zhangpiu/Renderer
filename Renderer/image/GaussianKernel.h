////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (C)  2016-2099, ZJU.
// 
// File name:     GaussianKernel.h
// 
// Author:        Piu Zhang
// 
// Version:       V1.0
// 
// Date:          2016.10.28
// 
// Description:   Gaussian kernel generator.
//
// 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once


#include <cmath>
#include <vector>
#include <cassert>

using std::vector;

class GaussianKernel1D {
public:
	GaussianKernel1D(int radius, double sigma)
		: _radius(radius)
		, _sigma(sigma) {
		assert(radius > 0);
		assert(sigma > 0);

		const int size = 2 * radius + 1;
		const double pi = std::atan(1) * 4;
		const double a = 2 * sigma * sigma;
		const double b = std::sqrt(1 / (a * pi));
		double sum = 0;

		_data.resize(size);

		for (int i = 0; i < size; ++i) {
			const int x = i - radius;

			_data[i] = b * std::exp(-(x*x) / a);
			sum += _data[i];
		}

		for (int i = 0; i < size; ++i) {
			_data[i] /= sum;
		}
	}

	const double operator()(int i) const {
		assert(i >= -_radius && i <= _radius);

		return _data[i + _radius];
	}

	int radius() const {
		return _radius;
	}

private:
	int _radius;
	double _sigma;
	vector<double> _data;
};

class GaussianKernel2D {
public:
	GaussianKernel2D(int radius, double sigma)
		: _radius(radius)
		, _sigma(sigma) {
		assert(radius > 0);
		assert(sigma > 0);

		const int size = 2 * radius + 1;
		const double pi = std::atan(1) * 4;
		const double a = 2 * sigma * sigma;
		const double b = 1 / (a * pi);
		double sum = 0;

		_data.resize(size, vector<double>(size));

		for (int i = 0; i < size; ++i) {
			for (int j = 0; j < size; ++j) {
				const int x = j - radius;
				const int y = radius - i;

				_data[i][j] = b * std::exp(-(x*x + y*y) / a);
				sum += _data[i][j];
			}
		}

		for (int i = 0; i < size; ++i) {
			for (int j = 0; j < size; ++j) {
				_data[i][j] /= sum;
			}
		}
	}

	const double operator()(int i, int j) const {
		assert(i >= -_radius && i <= _radius);
		assert(j >= -_radius && j <= _radius);

		return _data[i + _radius][j + _radius];
	}

	int radius() const {
		return _radius;
	}

private:
	int _radius;
	double _sigma;
	vector<vector<double>> _data;
};