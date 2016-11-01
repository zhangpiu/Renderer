////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (C)  2016-2099, ZJU.
// 
// File name:     Color.h
// 
// Author:        Piu Zhang
// 
// Version:       V1.0
// 
// Date:          2016.10.28
// 
// Description:   Color
// 
// Others: 
//
// Example:      
//
// 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

class Color {
public:

	static const Color BLACK;

	static const Color WHITE;

	static const Color RED;

	static const Color GREEN;

	static const Color BLUE;

	Color() : _r(0), _g(0), _b(0) {}

	Color(double r, double g, double b) : _r(r), _g(g), _b(b) {}

	// Add
	Color& operator += (const Color& rhs);

	friend Color operator + (const Color& lhs, const Color& rhs);

	// Multiply
	Color& operator *= (double value);

	friend Color operator * (const Color& lhs, double value);

	friend Color operator * (double value, const Color& rhs);

	// Modulate
	Color modulate(const Color& rhs) const;

	double r() const { return _r; }

	double g() const { return _g; }

	double b() const { return _b; }

	friend std::tuple<uint8, uint8, uint8> convert2Int(const Color& clr);

private:
	double _r, _g, _b;
};

Color& Color::operator += (const Color& rhs) {
	_r += rhs._r;
	_g += rhs._g;
	_b += rhs._b;
	return *this;
}

Color operator + (const Color& lhs, const Color& rhs) {
	Color ret(lhs);
	ret += rhs;
	return ret;
}

Color& Color::operator *= (double value) {
	_r *= value;
	_g *= value;
	_b *= value;
	return *this;
}


Color operator * (const Color& lhs, double value) {
	Color ret(lhs);
	ret *= value;
	return ret;
}

Color operator * (double value, const Color& rhs) {
	return rhs * value;
}


Color Color::modulate(const Color& rhs) const {
	return Color(_r*rhs._r, _g*rhs._g, _b*rhs._b);
}


std::tuple<uint8, uint8, uint8> convert2Int(const Color& clr) {
	uint8 r = (uint8)Math::clip(clr.r() * 255, 0.0, 255.0);
	uint8 g = (uint8)Math::clip(clr.g() * 255, 0.0, 255.0);
	uint8 b = (uint8)Math::clip(clr.b() * 255, 0.0, 255.0);

	return std::make_tuple(r,g,b);
}


const Color Color::BLACK = Color(0,0,0);

const Color Color::WHITE = Color(1,1,1);

const Color Color::RED   = Color(1,0,0);

const Color Color::GREEN = Color(0,1,0);

const Color Color::BLUE  = Color(0,0,1);