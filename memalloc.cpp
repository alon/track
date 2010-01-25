// test dALLOCA16 or other memory allocation routines

#include <iostream>
#include <sstream>
#include <ode/ode.h>

using namespace std;

//#define ALLOCA dALLOCA16
#define ALLOCA malloc

int main(int argc, char **argv)
{
	// test memory allocation of 3 MB approx.
	int numreal = 606 * 608;
	cout << argc << "\n";
	if (argc > 1) {
		istringstream i(argv[1]);
		i >> numreal;
		if (numreal > 5e6) {
			cout <<
			    "error: request too large. not allocating.\n";
			exit(-1);
		}
	}
	cout << numreal << "\n";
	dReal *invM = (dReal *) ALLOCA(numreal * sizeof(dReal));
	if (invM == 0) {
		cout << "error: dALLOCA16 returned null.\n";
		exit(-2);
	}
	dSetZero(invM, numreal);
	char dump;
	cin.get(dump);
	return 0;
}
