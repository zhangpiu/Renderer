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