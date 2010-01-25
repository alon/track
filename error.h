#ifndef __ERROR_H__
#define __ERROR_H__

#include <iostream>
#define PEXP(t) (std::cout << "debug: " #t " = " << (t) << "\n")
#define PVEC(t) (std::cout << "debug: " #t " = (" << (t[0]) << "," << (t[1]) << "," << (t[2]) << ")\n")
#define ASSERT(t) if (!(t)) std::cout << "assert: " #t " failed\n"

// TODO: these don't really belong here
#define CONSTRAIN_INT(num, low, high) if (num < low) num = low; else if (num > high) num = high
#define BRACKET(a, lo, high) ((a) > (high) ? (high) : (a) < (lo) ? (lo) : (a))

void error(const char* fmt,...);
void debug(const char* fmt,...);

#endif				// __ERROR_H__
