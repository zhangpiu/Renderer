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