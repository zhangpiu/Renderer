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
