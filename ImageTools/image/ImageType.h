////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (C)  2016-2099, ZJU.
// 
// File name:     ImageType.h
// 
// Author:        Piu Zhang
// 
// Version:       V1.0
// 
// Date:          2016.10.28
// 
// Description:   Image type enum class.
//
//                文件描述子 	类型 	编码
//                    P1 	    位图 	ASCII
//                    P2 	    灰度图 	ASCII
//                    P3 	    像素图 	ASCII
//                    P4 	    位图 	二进制
//                    P5 	    灰度图 	二进制
//                    P6 	    像素图 	二进制
//
// 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <unordered_map>


enum class ImageType {
	P1 = 1, P2, P3, P4, P5, P6
};

struct EnumClassHash{
	template<typename T>
	size_t operator()(T t) const{
		return static_cast<size_t>(t);
	}
};

static std::unordered_map<std::string, ImageType> name2type = {
	{ "P1", ImageType::P1 },
	{ "P2", ImageType::P2 },
	{ "P3", ImageType::P3 },
	{ "P4", ImageType::P4 },
	{ "P5", ImageType::P5 },
	{ "P6", ImageType::P6 },
};

static std::unordered_map<ImageType, std::string, EnumClassHash> type2name = {
	{ ImageType::P1, "P1" },
	{ ImageType::P2, "P2" },
	{ ImageType::P3, "P3" },
	{ ImageType::P4, "P4" },
	{ ImageType::P5, "P5" },
	{ ImageType::P6, "P6" },
};