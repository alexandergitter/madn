#ifndef _WURFEL_H_
#define _WURFEL_H_

#include <ClanLib/display.h>
#include <ClanLib/core.h>

class Wurfel
{
public:
	Wurfel();
	~Wurfel();
	
	void draw();
	void werf();
	void setPos(int x, int y) { pos.x = x; pos.y =y; }
	inline bool is_finished() { return finished; }
	inline int get_value() { return value+1; }
	inline void set_value(int val) { value = val-1; }

private:
	CL_Sprite* wurfel;
	CL_Surface* zahlen;
	CL_Point pos;
	
	int value;
	unsigned int start_time;
	bool finished;
};

#endif
