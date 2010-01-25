#ifndef __CCONFIG_H__
#define __CCONFIG_H__

#include <sstream>
#include <cstring>
#include <fstream>
#include <ode/ode.h>

class cConfig {
private:
	char line[300];		// hold last read line
	int linenum;		// number of current line (look at READ macro)
	std::ifstream ifile;
	std::string name;	// first word in line previously read with read
	int ok;			// was file opened succesfully
	int BadPrevRead;	// was the previous read any good
public:
	std::istringstream* iss;// parse line. buggy under visual c++ 6.0
	void read(const char *astring, int optional = 0);
	dReal get_dReal();
	cConfig(const char *filename);
	int isOk() { return ok; };
};

#endif // __CCONFIG_H__
