#ifndef _FIGUR_H_
#define _FIGUR_H_

#include <ClanLib/display.h>
#include <ClanLib/core.h>

class Map;

class Figur
{
public:
	Figur(int tfield, int col, Map* tmap);
	~Figur();
	
	void draw();
	static void init();
	int getField() { return field; }
	int getColor() { return color; }
	bool canMove(int count);
	void move_im(int tfield);
	void move_anim(int count);
	void update();
	bool is_finished() { return finish; }
	bool hasCoords(int x, int y);
	
private:
	static CL_Surface* f_rot;
	static CL_Surface* f_gruen;
	static CL_Surface* f_blau;
	static CL_Surface* f_gelb;
	
	int field;
	int color;
	Map* map;
	bool finish;
	
	int ma_act_target;
	unsigned int ma_start_time;
	int ma_count;
	CL_Point ma_pos;
	CL_Point ma_target_pos;
	int ma_wtm;
	
	void ma_finish();
};

#endif
