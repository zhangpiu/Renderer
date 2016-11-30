////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (C)  2016-2099, ZJU.
// 
// File name:     PXMImage.h
// 
// Author:        Piu Zhang
// 
// Version:       V1.0
// 
// Date:          2016.10.28
// 
// Description:   A toolkit to read/write PPM / PGM / PBM image files.
//
// 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <fstream>

#include "ImageType.h"
#include "MyString.h"
#include "MyException.h"
#include "Matrix.h"

using namespace std;

class PXMImage {
public:

	static Matrix<uint8> open(const string& filepath);

	static void save(const Matrix<uint8>& mat, const string& filepath, const ImageType& type = ImageType::P6, bool ascii = false);

private:
	struct Header {
		Header(){}

		Header(ImageType type, int width, int height, int mmax)
			: _type(type), _width(width), _height(height), _mmax(mmax)
		{}

		ImageType _type;
		int _width, _height, _mmax;
	};

	static Header parseHeader(ifstream& file);

	void readPPM(const string& filepath);
};



PXMImage::Header PXMImage::parseHeader(ifstream& file) {
	string line, name;
	int result[3] = { 0 }, idx = 0, varNum = 3;
	bool first = true;

	while (true){
		char ch = file.peek();

		if (ch == ' ' || ch == '\t' || ch == '\n'){
			file.ignore();
			continue;
		}
		else if (ch == '#'){
			getline(file, line);
			continue;
		}

		if (first) {
			file >> name;
			first = false;
			if (name2type.find(name) == name2type.end()) throw Exception("File header is wrong!");
			if (name2type[name] == ImageType::P1 || name2type[name] == ImageType::P4) varNum = 2;
		}
		else {
			file >> result[idx++];
		}

		if (idx == varNum) {
			getline(file, line);
			break;
		}
	}

	return PXMImage::Header(name2type[name], result[0], result[1], result[2]);
}

Matrix<uint8> PXMImage::open(const string& filepath){
	ifstream file(filepath, ios::in | ios::binary);
	Matrix<uint8> mat;

	try{
		auto header = parseHeader(file);

		const int width = header._width;
		const int height = header._height;
		const int channel = 3;
		const bool ascii = (int)(header._type) <= 3;

		mat.create(height, width, channel);

		if (ascii){
			for (int i = 0; i < height; ++i)
				for (int j = 0; j < width; ++j)
					for (int k = 0; k < channel; ++k)
						file >> mat(i, j, k);
		}
		else {
			const int len = width * height * channel;
			auto curr = file.tellg();
			file.seekg(0, ios::end);
			auto tail = file.tellg();
			file.seekg(curr);

			if (len != tail - curr) throw Exception("Image data is corrupted!");

			unique_ptr<uint8_t[]> buf(new uint8_t[len]);
			file.read((char*)buf.get(), len*sizeof(uint8_t));

			for (int i = 0; i < height; ++i)
				for (int j = 0; j < width; ++j)
					for (int k = 0; k < channel; ++k)
						mat(i, j, k) = buf[k + j*channel + i*width*channel];
		}
	}
	catch (exception& e){
		cerr << e.what() << endl;
		file.close();
	}

	file.close();
	return std::move(mat);
}


void PXMImage::save(const Matrix<uint8>& mat, const string& filepath, const ImageType& type, bool ascii){
	ofstream file(filepath.c_str(), ios::out | ios::binary);
	const int width = mat.width();
	const int height = mat.height();
	const int channel = mat.channel();

	file << String::format("%s\n%d %d\n%d\n", type2name[type].c_str(), width, height, 255);

	if (ascii){
		for (int i = 0; i < height; ++i)
			for (int j = 0; j < width; ++j)
				for (int k = 0; k < channel; ++k)
					file << (int)mat(i, j, k) << " ";
	}
	else {
		const int len = width*height*channel;
		int idx = 0;
		unique_ptr<uint8[]> buf(new uint8[len]);

		for (int i = 0; i < height; ++i)
			for (int j = 0; j < width; ++j)
				for (int k = 0; k < channel; ++k)
					buf[idx++] = mat(i, j, k);

		file.write((char*)buf.get(), len*sizeof(uint8));
	}

	file.close();
}