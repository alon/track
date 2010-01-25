#ifndef __KEY_FUNCTOR_H__
#define __KEY_FUNCTOR_H__

#include "callback.h"

class KeyMap {
private:
	char keymap[256];
	CBFunctor0 func[256];
public:
	keyFunctor() {
		for ( int i = 0 ; i < 256 ; ++i ) {
			key[i] = 0;
			func[i] = makeFunctor((CBFunctor0*)0, this, notAssigned);
		}
	}
	void notAssigned() {};
};

#endif // __KEY_FUNCTOR_H__
