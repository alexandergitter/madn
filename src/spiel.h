#ifndef _SPIEL_H_
#define _SPIEL_H_

#include <ClanLib/core.h>
#include <ClanLib/gl.h>
#include <ClanLib/application.h>
#include <ClanLib/display.h>

#include "statem.h"

class Spiel : public CL_ClanApplication
{
public:
	Spiel();
	~Spiel();

	virtual int main(int argc, char **argv);
	
private:
	void on_window_close();
	
	Statem* statem;
	static CL_ConsoleWindow console;
};

#endif
