#include "String.h"


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