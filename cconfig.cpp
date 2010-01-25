#include <iostream>
#include <string>
#include "cconfig.h"
#include "error.h"

cConfig::cConfig(const char *filename) : ok(0)
{
	ifile.open(filename, std::fstream::in);
	linenum = 0;		// number of lines read

	if (!ifile.is_open()) {
		printf("error: can not open %s\n", filename);
	};
	ok = 1;
	BadPrevRead=0;
}

dReal cConfig::get_dReal()
{
	std::string str;
	*iss >> str;
	if (str == "dInfinity")
		return dInfinity;
	std::istringstream* iss = new std::istringstream(str.data());
	dReal t;
	*iss >> t;
	delete iss;
	return t;
}

void cConfig::read(const char *astring, int optional)
{

	//PEXP(BadPrevRead);
	// first check if we have a string already (from a bad / non existant previous read)
	if (!BadPrevRead) { // no bad previous read
		line[0] = '#';

		// skip comment lines
		while (line[0] == '#') {
			ifile.getline(line, sizeof(line));
			linenum++;	// advance line count
		}
	
		iss = new std::istringstream(line);
	
		// read name
		*iss >> name;
	}
	
	// any way (bad previous read / not bad) test the name

	if (name != astring) {
		if (optional) {
			BadPrevRead = 1;
			return;
		}
		std::cout << "error: line number " << linenum <<
		    " must begin with *" << astring << "* in param.dat\n";
		std::cout << "error: offending line: *" << line << "*\n";
		std::cout << "debug: name value: " << name << "\n";
		std::cout << "debug: astring size, name size " <<
		    strlen(astring) << ", " << name.length() << "\n";
		exit(-1);
	}
	BadPrevRead = 0;
	//printf("debug: %s\n", name.data());
}


