////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (C)  2016-2099, ZJU.
// 
// File name:     Math.h
// 
// Author:        Piu Zhang
// 
// Version:       V1.0
// 
// Date:          2016.10.28
// 
// Description:   Math
// 
// Others: 
//
// Example:      
//
// 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

class Math {
public:

	static const double PI;

	template<typename T>
	static inline T clip(const T& x, const T& low, const T& high) {
		return x < low ? low : (x > high ? high : x);
	}

	template<typename T>
	static inline T max3(const T& a, const T& b, const T& c) {
		return std::max(a, std::max(b, c));
	}
};

const double Math::PI = 3.141592653589793238462643;