////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (C)  2016-2099, ZJU.
// 
// File name:     Exception.h
// 
// Author:        Piu Zhang
// 
// Version:       V1.0
// 
// Date:          2016.10.28
// 
// Description:   Exception
//
// 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <exception>
#include <string>

class Exception : public std::exception{
public:
	explicit Exception(const std::string& msg = "") throw()
		: _msg(msg)
	{}

	virtual const char* what() const throw() {
		return _msg.c_str();
	}

private:
	std::string _msg;
};