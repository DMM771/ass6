//324680438
//313306367
// Nitzan Fisher & David Monheit

#ifndef CLI_H_
#define CLI_H_

#include <string.h>
#include "commands.h"
#include <array>
#include "minCircle.h"

using namespace std;

class CLI {
public:
	DefaultIO* defaultInputOutput;
    vector<Command*> cliCmds;
	CLI(DefaultIO* inputOutput);
	void start();
	virtual ~CLI();
};

#endif /* CLI_H_ */
