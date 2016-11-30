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
//
// 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

struct Color {
	static const Color BLACK;

	static const Color WHITE;

	static const Color RED;

	static const Color GREEN;

	static const Color BLUE;

	Color() : r(0), g(0), b(0) {}

	Color(double r, double g, double b) : r(r), g(g), b(b) {}

	// Add
	Color& operator += (const Color& rhs);

	friend Color operator + (const Color& lhs, const Color& rhs);

	// Multiply
	Color& operator *= (double value);

	friend Color operator * (const Color& lhs, double value);

	friend Color operator * (double value, const Color& rhs);

	// Modulate
	Color modulate(const Color& rhs) const;

	friend uint8 convert(double channel, double alpha = 1.0);

	double r, g, b;
};

Color& Color::operator += (const Color& rhs) {
	r += rhs.r;
	g += rhs.g;
	b += rhs.b;
	return *this;
}

Color operator + (const Color& lhs, const Color& rhs) {
	Color ret(lhs);
	ret += rhs;
	return ret;
}

Color& Color::operator *= (double value) {
	r *= value;
	g *= value;
	b *= value;
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
	return Color(r*rhs.r, g*rhs.g, b*rhs.b);
}

uint8 convert(double channel, double alpha /* = 1.0 */) {
	channel = Math::clip(channel, 0.0, 1.0);

	return uint8(std::pow(channel, 1 / alpha) * 255 + .5);
}


const Color Color::BLACK = Color(0,0,0);

const Color Color::WHITE = Color(1,1,1);

const Color Color::RED   = Color(1,0,0);

const Color Color::GREEN = Color(0,1,0);

const Color Color::BLUE  = Color(0,0,1);