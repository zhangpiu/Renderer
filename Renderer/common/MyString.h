////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (C)  2016-2099, ZJU.
// 
// File name:     MyString.h
// 
// Author:        Piu Zhang
// 
// Version:       V1.0
// 
// Date:          2016.10.28
// 
// Description:   String format tools.   
//
// 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <cstdarg>   // For va_start, etc.
#include <memory>    // For std::unique_ptr
#include <string>


class String{
public:
	String(const char* str)
		: _str(str)
	{}

	String(const String& rhs){
		this->_str = rhs._str;
	}

	String(const std::string& rhs){
		this->_str = rhs;
	}

	operator std::string() { return _str; }

	friend std::ostream& operator << (std::ostream& out, const String& str);

	static String format(const std::string formatString, ...);

private:
	std::string _str;
};

/*
 * The 'formatString' is passed by value to conform with the requirements of va_start.
 * 
 * stack overflow: http://stackoverflow.com/questions/2342162/stdstring-formatting-like-sprintf
**/
String String::format(const std::string formatString, ...) {
	int final_n, n = ((int)formatString.size()) * 2; /* Reserve two times as much as the length of the formatString */
	std::string str;
	std::unique_ptr<char[]> formatted;
	va_list ap;

	while (1) {
		formatted.reset(new char[n]);                /* Wrap the plain char array into the unique_ptr */
		strcpy(formatted.get(), formatString.c_str());

		va_start(ap, formatString);
		final_n = vsnprintf(formatted.get(), n, formatString.c_str(), ap);
		va_end(ap);

		if (final_n < 0 || final_n >= n)
			n += abs(final_n - n + 1);
		else
			break;
	}

	return String(formatted.get());
}

std::ostream& operator << (std::ostream& out, const String& str) {
	return out << str._str;
}