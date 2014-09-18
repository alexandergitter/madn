#ifndef _MAP_H_
#define _MAP_H_

#include <ClanLib/display.h>
#include <ClanLib/core.h>
#include "figur.h"
#include "wurfel.h"

class Map
{
public:
	Map();
	~Map();
	static const int RED = 1;
	static const int BLUE = 2;
	static const int YELLOW = 3;
	static const int GREEN = 4;
	
	static const int UP = 8;
	static const int DOWN = 2;
	static const int LEFT = 4;
	static const int RIGHT = 6;
	
	void paint();
	void update();
	CL_Point getCoords(int field, int color);
	int getNextField(int field, int color);
	void setFigure(int field, int col);
	void unsetFigure(int field, int color);
	
	void debuginfo();
	
private:
	CL_Surface* k_rot;
	CL_Surface* k_gruen;
	CL_Surface* k_blau;
	CL_Surface* k_gelb;
	CL_Surface* k_grau;
	CL_Surface* k_weiss;
	
	CL_Point map[40];
	CL_Point yellowmap[9];
	CL_Point redmap[9];
	CL_Point bluemap[9];
	CL_Point greenmap[9];
	
	Figur* gelbfigurs[4];
	Figur* rotfigurs[4];
	Figur* blaufigurs[4];
	Figur* gruenfigurs[4];
	
	Wurfel* wurfel;
	
	short mapstat[40];
	bool yellowstat[9];
	bool redstat[9];
	bool bluestat[9];
	bool greenstat[9];
	
	int yellowready;
	int redready;
	int blueready;
	int greenready;
	
	int gamestat;
	
	void add(CL_Point* way, int wo, int count, int dir);
	void initMap();
	bool moveImpossible();
	
	void mov_wurfel(int x, int y, int nstat);
	void mov_choose(int pstat, int tstat);
	
	Figur* getFigByField(int field);
	Figur* getFigByCoords(int x, int y);
	
	Figur* ma_figur;
};

#endif
