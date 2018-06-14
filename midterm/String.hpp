#ifndef CS540_STRING_HPP
#define CS540_STRING_HPP

#include <iostream>
#include <string.h>

class String {
	public:
		friend std::ostream &operator<<(std::ostream & os, const String &s) {
			return os << s.buf;
		}
		friend String operator+(const String &s1, const String &s2) {
			char * concatBuf = (char *)(malloc(strlen(s1.buf) + strlen(s2.buf) + 1));
			strcpy(concatBuf, s1.buf);
			strcat(concatBuf, s2.buf);
			String s(concatBuf);
			free(concatBuf);
			return s;
		}
		String(const char *s) : buf(strdup(s)) {}
		String(const String &s) : String(s.buf) {}
		String& operator=(const String &s) {
			char *p = strdup(s.buf);
			free(buf);
			buf = p;
			return *this;
		}
		~String() {
			free(buf);
			buf = 0;
		}
	private:
		char * buf;

};


#endif