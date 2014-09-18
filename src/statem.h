#ifndef _STATEM_H_
#define _STATEM_H_

#include <ClanLib/core.h>
#include <ClanLib/display.h>

#include "map.h"

class Statem
{
public:
	Statem();
	~Statem();
		
	void run();
	void quit();
	static void delay(int milis);
		
private:
	bool bquit;
	Map* map;
	
	static unsigned int delayt;
	static unsigned int startt;
};

#endif
