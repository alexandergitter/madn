#include "statem.h"

unsigned int Statem::delayt = 0;
unsigned int Statem::startt = 0;

Statem::Statem()
{
	bquit = false;
	map = new Map();
}

Statem::~Statem() {}

void Statem::run()
{
	while(!bquit)
	{
		if(CL_Keyboard::get_keycode(CL_KEY_F12))
			map->debuginfo();
	
		if((CL_System::get_time() - startt) > delayt)
			map->update();
			
		map->paint();
	
		CL_Display::flip();
		CL_System::sleep(5);
		CL_System::keep_alive(5);
	}
}

void Statem::quit()
{
	bquit = true;
}

void Statem::delay(int millis)
{
	startt = CL_System::get_time();
	delayt = millis;
}
