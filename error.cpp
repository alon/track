#include <cstdio>
#include <cstdarg>
#include "error.h"

void helper(const char* start, const char* fmt, va_list ap)
{
	//double d=va_arg(ap,double);
	puts(start);
	vprintf(fmt, ap);
	puts("\n");
}

void debug(const char* fmt,...)
{
	va_list ap; /*will point to each unnamed argument in turn*/
	va_start(ap, fmt); /*make ap point to the first arg after fmt*/
	helper("debug: ", fmt, ap);
	va_end(ap); /*cleanup*/ 
}

void error(const char* fmt,...)
{
	va_list ap; /*will point to each unnamed argument in turn*/
	va_start(ap, fmt); /*make ap point to the first arg after fmt*/
	helper("error: ", fmt, ap);
	va_end(ap); /*cleanup*/ 
}

