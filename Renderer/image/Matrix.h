////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (C)  2016-2099, ZJU.
// 
// File name:     Matrix.h
// 
// Author:        Piu Zhang
// 
// Version:       V1.0
// 
// Date:          2016.10.28
// 
// Description:   Matrix class.
//
// 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <cassert>
#include <tuple>

using uint8 = unsigned char;
using uint16 = unsigned short;
using uint32 = unsigned int;
using uint64 = unsigned long long;


class Size {
public:
	Size() : _size{0, 0, 0} {}

	Size(int height, int width, int channel = 1)
		: _size{height, width, channel}
	{}

	bool operator == (const Size& rhs) const { return _size == rhs._size; }

	int height()  const { return std::get<0>(_size); }

	int width()   const { return std::get<1>(_size); }

	int channel() const { return std::get<2>(_size); }

private:
	// height, width, channel
	std::tuple<int,int,int> _size;
};

template<typename T>
class Matrix {
public:
	// Prevent heap allocation
	void* operator new  (size_t) = delete;
	void* operator new[](size_t) = delete;
	void  operator delete(void*) = delete;
	void  operator delete[](void*) = delete;

	void create(int height, int width, int channel);

	Matrix()
		: _data(nullptr)
		, _height(0)
		, _width(0)
		, _channel(0)
		, _width_x_channel(0)
		, _length(0)
	{}

	Matrix(int height, int width, int channel = 1) : Matrix() { 
		create(height, width, channel);
	}

	Matrix(const Size& size) : Matrix() {
		create(size.height(), size.width(), size.channel());
	}

	~Matrix() {
		destory();
	}

	Matrix(const Matrix<T>& rhs) {
		create(rhs.height(), rhs.width(), rhs.channel());
		memcpy(_data, rhs._data, sizeof(T)*_length);
	}

	Matrix<T>& operator = (const Matrix<T>& rhs) {
		create(rhs.height(), rhs.width(), rhs.channel());
		memcpy(_data, rhs._data, sizeof(T)*_length);
		return *this;
	}

	Matrix(Matrix<T>&& rhs) : Matrix() {
		swap(rhs);
	}

	Matrix<T>& operator = (Matrix&& rhs) {
		swap(rhs);
		return *this;
	}

	T& operator() (int row, int col, int channel = 0) {
		return _data[row*_width_x_channel + col*_channel + channel];
	}

	const T& operator() (int row, int col, int channel = 0) const {
		return _data[row*_width_x_channel + col*_channel + channel];
	}

	int height()  const { return _height;          }

	int width()   const { return _width;           }

	int channel() const { return _channel;         }

	bool empty()  const { return _data == nullptr; }

	int length()  const { return _length;          }

	int bytes()   const { return _length*sizeof(T);}

	T* data() const { return _data; }

	friend Size size(const Matrix<T>& m);

	bool identical(const Matrix<T>& rhs) const { return this == &rhs; }

	bool equalSize(const Matrix<T>& rhs) const { 
		return _height == rhs._height && _width == rhs._width && _channel == rhs._channel;
	}

private:
	void destory();
	void swap(Matrix<T>& rhs);

private:
	T* _data;

	int _height, _width, _channel;
	int _width_x_channel, _length;
};

template<typename T>
void Matrix<T>::create(int height, int width, int channel) {
	assert(height > 0 && width > 0 && channel > 0);

	destory();
	
	_height  = height;
	_width   = width;
	_channel = channel;
	_width_x_channel = width*channel;
	_length = height*width*channel;

	_data = new T[_length];
	memset(_data, 0, sizeof(T)*_length);
}

template<typename T>
void Matrix<T>::destory() {
	delete[] _data;
}

template<typename T>
void Matrix<T>::swap(Matrix<T>& rhs) {
	std::swap(_data, rhs._data);
	std::swap(_height, rhs._height);
	std::swap(_width, rhs._width);
	std::swap(_channel, rhs._channel);
	std::swap(_width_x_channel, rhs._width_x_channel);
	std::swap(_length, rhs._length);
}

template<typename T>
Size size(const Matrix<T>& m) { 
	return Size(m.height(), m.width(), m.channel); 
}