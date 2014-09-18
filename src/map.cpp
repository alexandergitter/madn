#include "statem.h"
#include "map.h"

#define YELLOWSTAT(a) (a & 0xf)
#define REDSTAT(a) ((a & 0xf0) >> 4)
#define BLUESTAT(a) ((a & 0xf00) >> 8)
#define GREENSTAT(a) ((a & 0xf000) >> 12)

#define S_GELBWURFEL 1
#define S_GELBCHOOSE 2
#define S_ROTWURFEL 4
#define S_ROTCHOOSE 5
#define S_BLAUWURFEL 7
#define S_BLAUCHOOSE 8
#define S_GRUENWURFEL 10
#define S_GRUENCHOOSE 11

Map::Map()
{
	k_rot = new CL_Surface("kreis_rot.tga");
	k_gruen = new CL_Surface("kreis_gruen.tga");
	k_gelb = new CL_Surface("kreis_gelb.tga");
	k_blau = new CL_Surface("kreis_blau.tga");
	k_grau = new CL_Surface("kreis_grau.tga");
	k_weiss = new CL_Surface("kreis_weiss.tga");
	
	initMap();
	
	for(int i = 0; i < 4; ++i) {
		gelbfigurs[i] = new Figur(i, Map::YELLOW, this);
		rotfigurs[i] = new Figur(i, Map::RED, this);
		blaufigurs[i] = new Figur(i, Map::BLUE, this);
		gruenfigurs[i] = new Figur(i, Map::GREEN, this);
	}
	
	Figur::init();
	
	wurfel = new Wurfel();
	ma_figur = NULL;
	yellowready = 0;
	redready = 0;
	blueready = 0;
	greenready = 0;

	gamestat = S_GELBWURFEL;
}

Map::~Map() {}

void Map::paint()
{
	CL_Display::clear(CL_Color(230,180,120));

	for(int i = 0; i < 40; ++i) {
		k_weiss->draw(map[i].x *30, map[i].y *30);
	}
	
	for(int i = 0; i < 5; ++i) {
		k_gelb->draw(yellowmap[i].x *30, yellowmap[i].y *30);
		k_rot->draw(redmap[i].x *30, redmap[i].y *30);
		k_blau->draw(bluemap[i].x *30, bluemap[i].y *30);
		k_gruen->draw(greenmap[i].x *30, greenmap[i].y *30);
	}
	
	for(int i = 5; i < 9; ++i) {
		k_grau->draw(yellowmap[i].x *30, yellowmap[i].y *30);
		k_grau->draw(redmap[i].x *30, redmap[i].y *30);
		k_grau->draw(bluemap[i].x *30, bluemap[i].y *30);
		k_grau->draw(greenmap[i].x *30, greenmap[i].y *30);
	}
	
	for(int i = 0; i < 4; ++i) {
		gelbfigurs[i]->draw();
		rotfigurs[i]->draw();
		blaufigurs[i]->draw();
		gruenfigurs[i]->draw();
	}
	
	if(ma_figur != NULL)
		ma_figur->draw();
	
	wurfel->draw();
}

CL_Point Map::getCoords(int field, int color)
{
	if(field < 4) {
		switch(color) {
		case Map::YELLOW:
			return yellowmap[field];
		case Map::RED:
			return redmap[field];
		case Map::BLUE:
			return bluemap[field];
		case Map::GREEN:
			return greenmap[field];
		}
	} else if(field > 43) {
		switch(color) {
		case Map::YELLOW:
			return yellowmap[field-44+5];
		case Map::RED:
			return redmap[field-44+5];
		case Map::BLUE:
			return bluemap[field-44+5];
		case Map::GREEN:
			return greenmap[field-44+5];
		}
	} else {
		return map[field-4];
	}
}

int Map::getNextField(int field, int color)
{
	bool* colstat;
	CL_Point* colmap;
	
	if(field >= 47)
		return -1;
	
	if(field < 4) {
		switch(color) {
		case Map::YELLOW:
			if(YELLOWSTAT(mapstat[0]) == 0)
				return 4;
			else
				return -1;
				
		case Map::RED:
			if(REDSTAT(mapstat[10]) == 0)
				return 14;
			else
				return -1;
				
		case Map::BLUE:
			if(BLUESTAT(mapstat[20]) == 0)
				return 24;
			else
				return -1;
				
		case Map::GREEN:
			if(GREENSTAT(mapstat[30]) == 0)
				return 34;
			else
				return -1;
		}
	} else if(field > 43) {
		switch(color) {
		case Map::YELLOW:
			if(yellowstat[field-44+5] == false) {
				return field+1;
			}
			else
				return -1;
				
		case Map::RED:
			if(redstat[field-44+5] == false)
				return field+1;
			else
				return -1;
				
		case Map::BLUE:
			if(bluestat[field-44+5] == false)
				return field+1;
			else
				return -1;
				
		case Map::GREEN:
			if(greenstat[field-44+5] == false)
				return field+1;
			else
				return -1;
		}
	} else {
		switch(color) {
		case Map::YELLOW:
			if((field == 43) && (yellowstat[4] == false))
				return 44;
			else if((field != 43) && (YELLOWSTAT(mapstat[field-4+1]) == 0))
				return field+1;
			else
				return -1;
				
		case Map::RED:
			if((field == 13) && (redstat[4] == false))
				return 44;
			else if(field != 13) {
				if((field == 43) && (REDSTAT(mapstat[0]) == 0)) {
					return 4;
				}
				else if(REDSTAT(mapstat[field-3]) == 0)
					return field+1;
				else
					return -1;
			} else
				return -1;
				
		case Map::BLUE:
			if((field == 23) && (bluestat[4] == false))
				return 44;
			else if(field != 23) {
				if((field == 43) && (BLUESTAT(mapstat[0]) == 0))
					return 4;
				else if(BLUESTAT(mapstat[field-3]) == 0)
					return field+1;
				else
					return -1;
			} else
				return -1;
				
		case Map::GREEN:
			if((field == 33) && (greenstat[4] == false))
				return 44;
			else if(field != 33) {
				if((field == 43) && (GREENSTAT(mapstat[0]) == 0))
					return 4;
				else if(GREENSTAT(mapstat[field-3]) == 0)
					return field+1;
				else
					return -1;
			} else
				return -1;
		}
	}
}

void Map::initMap()
{
	map[0].x = 4;
	map[0].y = 10;

	add(map, 1, 4, Map::UP);
	add(map, 5, 4, Map::LEFT);
	add(map, 9, 2, Map::UP);
	add(map, 11, 4, Map::RIGHT);
	add(map, 15, 4, Map::UP);
	add(map, 19, 2, Map::RIGHT);
	add(map, 21, 4, Map::DOWN);
	add(map, 25, 4, Map::RIGHT);
	add(map, 29, 2, Map::DOWN);
	add(map, 31, 4, Map::LEFT);
	add(map, 35, 4, Map::DOWN);
	add(map, 39, 1, Map::LEFT);
	
	yellowmap[0].x = 0; yellowmap[0].y = 9;
	yellowmap[1].x = 1; yellowmap[1].y = 9;
	yellowmap[2].x = 0; yellowmap[2].y = 10;
	yellowmap[3].x = 1; yellowmap[3].y = 10;
	yellowmap[4].x = 4; yellowmap[4].y = 10;
	yellowmap[5].x = 5; yellowmap[5].y = 9;
	add(yellowmap, 6, 3, Map::UP);
	
	redmap[0].x = 0; redmap[0].y = 0;
	redmap[1].x = 1; redmap[1].y = 0;
	redmap[2].x = 0; redmap[2].y = 1;
	redmap[3].x = 1; redmap[3].y = 1;
	redmap[4].x = 0; redmap[4].y = 4;
	redmap[5].x = 1; redmap[5].y = 5;
	add(redmap, 6, 3, Map::RIGHT);
	
	bluemap[0].x = 9; bluemap[0].y = 0;
	bluemap[1].x = 10; bluemap[1].y = 0;
	bluemap[2].x = 9; bluemap[2].y = 1;
	bluemap[3].x = 10; bluemap[3].y = 1;
	bluemap[4].x = 6; bluemap[4].y = 0;
	bluemap[5].x = 5; bluemap[5].y = 1;
	add(bluemap, 6, 3, Map::DOWN);
	
	greenmap[0].x = 9; greenmap[0].y = 9;
	greenmap[1].x = 10; greenmap[1].y = 9;
	greenmap[2].x = 9; greenmap[2].y = 10;
	greenmap[3].x = 10; greenmap[3].y = 10;
	greenmap[4].x = 10; greenmap[4].y = 6;
	greenmap[5].x = 9; greenmap[5].y = 5;
	add(greenmap, 6, 3, Map::LEFT);
	
	for(int i = 0; i < 40; ++i)
		mapstat[i] = 0;
		
	for(int i = 0; i < 9; ++i) {
		yellowstat[i] = false;
		redstat[i] = false;
		bluestat[i] = false;
		greenstat[i] = false;
	}
}

void Map::add(CL_Point* way, int wo, int count, int dir)
{
	int startx = (*(way+wo-1)).x;
	int starty = (*(way+wo-1)).y;
	
	for(int i = 0; i < count; ++i) {
		switch(dir) {
		case Map::LEFT:
			(*(way+wo+i)).x = startx-i-1;
			(*(way+wo+i)).y = starty;
			break;
		case Map::UP:
			(*(way+wo+i)).x = startx;
			(*(way+wo+i)).y = starty-1-i;
			break;
		case Map::RIGHT:
			(*(way+wo+i)).x = startx+i+1;
			(*(way+wo+i)).y = starty;
			break;
		case Map::DOWN:
			(*(way+wo+i)).x = startx;
			(*(way+wo+i)).y = starty+i+1;
			break;
		}
	}
}

bool Map::moveImpossible()
{
	for(int i = 0; i < 4; ++i) {
		switch(gamestat) {
		case S_GELBCHOOSE:
			if(gelbfigurs[i]->canMove(wurfel->get_value()))
				return false;
				
			break;
		case S_ROTCHOOSE:
			if(rotfigurs[i]->canMove(wurfel->get_value()))
				return false;
				
			break;
		case S_BLAUCHOOSE:
			if(blaufigurs[i]->canMove(wurfel->get_value()))
				return false;
				
			break;
		case S_GRUENCHOOSE:
			if(gruenfigurs[i]->canMove(wurfel->get_value()))
				return false;
				
			break;
		}		
	}
	
	return true;
}

void Map::mov_wurfel(int x, int y, int nstat)
{
	switch(gamestat) {
	case S_GELBWURFEL:
		if(yellowready == 4) {
			gamestat = S_ROTWURFEL;
			return;
		}
			
		break;
	case S_ROTWURFEL:
		if(redready == 4) {
			gamestat = S_BLAUWURFEL;
			return;
		}
			
		break;
	case S_BLAUWURFEL:
		if(blueready == 4) {
			gamestat = S_GRUENWURFEL;
			return;
		}
			
		break;
	case S_GRUENWURFEL:
		if(greenready == 4) {
			gamestat = S_GELBWURFEL;
			return;
		}
			
		break;
	}
	
	wurfel->setPos(x, y);
	wurfel->werf();
	gamestat = nstat;
}

void Map::mov_choose(int pstat, int tstat)
{
	static int tries = 0;
	
	if(wurfel->is_finished()) {
		++tries;
		
		if(CL_Keyboard::get_keycode(CL_KEY_ESCAPE)) {
			if(CL_Keyboard::get_keycode(CL_KEY_1))
				wurfel->set_value(1);
			else if(CL_Keyboard::get_keycode(CL_KEY_2))
				wurfel->set_value(2);
			else if(CL_Keyboard::get_keycode(CL_KEY_3))
				wurfel->set_value(3);
			else if(CL_Keyboard::get_keycode(CL_KEY_4))
				wurfel->set_value(4);
			else if(CL_Keyboard::get_keycode(CL_KEY_5))
				wurfel->set_value(5);
			else if(CL_Keyboard::get_keycode(CL_KEY_6))
				wurfel->set_value(6);
		}
		
		if(ma_figur != NULL) {
			ma_figur->update();
			
			if(ma_figur->is_finished()) {
				ma_figur = NULL;
				gamestat = tstat;
				tries = 0;
				
				if(wurfel->get_value() == 6)
					gamestat = pstat;
			}
		} else if(moveImpossible()) {
			if(tries >= 3) {
				gamestat = tstat;
				tries = 0;
				Statem::delay(1000);
			} else {
				gamestat = pstat;
				Statem::delay(1000);
			}
		} else if(ma_figur == NULL) {
			if(CL_Mouse::get_keycode(CL_MOUSE_LEFT)) {
				Figur* temp = getFigByCoords(CL_Mouse::get_x(), CL_Mouse::get_y());
				if((temp != NULL) && (temp->canMove(wurfel->get_value()))) {
					if(temp->getField() < 4) {
						temp->move_im(getNextField(temp->getField(), temp->getColor()));
						gamestat = pstat;
						tries = 0;
					} else {
						ma_figur = temp;
						unsetFigure(ma_figur->getField(), ma_figur->getColor());
						ma_figur->move_anim(wurfel->get_value());
					}
				}				
			}
		}
	}
}

void Map::update()
{
	if((yellowready == 4) && (redready == 4) && (blueready == 4) && (greenready == 4))
		return;
		
	switch(gamestat) {
	case S_GELBWURFEL:
		mov_wurfel(60, 210, S_GELBCHOOSE);
		break;
		
	case S_GELBCHOOSE:
		mov_choose(S_GELBWURFEL, S_ROTWURFEL);
		break;
		
	case S_ROTWURFEL:
		mov_wurfel(60, 60, S_ROTCHOOSE);
		break;
		
	case S_ROTCHOOSE:
		mov_choose(S_ROTWURFEL, S_BLAUWURFEL);
		break;
		
	case S_BLAUWURFEL:
		mov_wurfel(210, 60, S_BLAUCHOOSE);
		break;
		
	case S_BLAUCHOOSE:
		mov_choose(S_BLAUWURFEL, S_GRUENWURFEL);
		break;	

	case S_GRUENWURFEL:
		mov_wurfel(210, 210, S_GRUENCHOOSE);
		break;
		
	case S_GRUENCHOOSE:
		mov_choose(S_GRUENWURFEL, S_GELBWURFEL);
		break;
	}
}

void Map::setFigure(int field, int color)
{
	if(field < 4) {
		switch(color) {
		case Map::YELLOW:
			yellowstat[field] = true;
			break;
		case Map::RED:
			redstat[field] = true;
			break;
		case Map::BLUE:
			bluestat[field] = true;
			break;
		case Map::GREEN:
			greenstat[field] = true;
			break;
		}
	} else if(field > 43) {
		switch(color) {
		case Map::YELLOW:
			++yellowready;
			yellowstat[field-44+4] = true;
			std::cout << "gelb: " << yellowready << "\n";
			break;
		case Map::RED:
			++redready;
			std::cout << "rot: " << redready << "\n";
			redstat[field-44+4] = true;
			break;
		case Map::BLUE:
			++blueready;
			std::cout << "blau: " << blueready << "\n";
			bluestat[field-44+4] = true;
			break;
		case Map::GREEN:
			++greenready;
			std::cout << "gruen: " << greenready << "\n";
			greenstat[field-44+4] = true;
			break;
		}
	} else {
		if(YELLOWSTAT(mapstat[field-4]) != 0) {
			if(yellowstat[0] == false) {
				getFigByField(field)->move_im(0);
			} else if(yellowstat[1] == false) {
				getFigByField(field)->move_im(1);
			} else if(yellowstat[2] == false) {
				getFigByField(field)->move_im(2);
			} else if(yellowstat[3] == false) {
				getFigByField(field)->move_im(3);
			}
		} else if(REDSTAT(mapstat[field-4]) != 0) {
			if(redstat[0] == false)
				getFigByField(field)->move_im(0);
			else if(redstat[1] == false)
				getFigByField(field)->move_im(1);
			else if(redstat[2] == false)
				getFigByField(field)->move_im(2);
			else if(redstat[3] == false)
				getFigByField(field)->move_im(3);
		} else if(BLUESTAT(mapstat[field-4]) != 0) {
			if(bluestat[0] == false)
				getFigByField(field)->move_im(0);
			else if(bluestat[1] == false)
				getFigByField(field)->move_im(1);
			else if(bluestat[2] == false)
				getFigByField(field)->move_im(2);
			else if(bluestat[3] == false)
				getFigByField(field)->move_im(3);
		} else if(GREENSTAT(mapstat[field-4]) != 0) {
			if(greenstat[0] == false)
				getFigByField(field)->move_im(0);
			else if(greenstat[1] == false)
				getFigByField(field)->move_im(1);
			else if(greenstat[2] == false)
				getFigByField(field)->move_im(2);
			else if(greenstat[3] == false)
				getFigByField(field)->move_im(3);
		}
		
		mapstat[field-4] = 0;
		
		switch(color) {
		case Map::YELLOW:
			mapstat[field-4] = 0xf;
			break;
		case Map::RED:
			mapstat[field-4] = 0xf0;
			break;
		case Map::BLUE:
			mapstat[field-4] = 0xf00;
			break;
		case Map::GREEN:
			mapstat[field-4] = 0xf000;
			break;
		}
	}
}

void Map::unsetFigure(int field, int color)
{
	if(field < 4) {
		switch(color) {
		case Map::YELLOW:
			yellowstat[field] = false;
			break;
		case Map::RED:
			redstat[field] = false;
			break;
		case Map::BLUE:
			bluestat[field] = false;
			break;
		case Map::GREEN:
			greenstat[field] = false;
			break;
		}
	} else if(field > 43) {
		switch(color) {
		case Map::YELLOW:
			--yellowready;
			std::cout << "unset gelb: " << yellowready << "\n";
			yellowstat[field-44+4] = false;
			break;
		case Map::RED:
			--redready;
			redstat[field-44+4] = false;
			std::cout << "unset rot: " << redready << "\n";
			break;
		case Map::BLUE:
			--blueready;
			bluestat[field-44+4] = false;
			std::cout << "unset blau: " << blueready << "\n";
			break;
		case Map::GREEN:
			--greenready;
			greenstat[field-44+4] = false;
			std::cout << "unset gruen: " << greenready << "\n";
			break;
		}
	} else {
		mapstat[field-4] = 0;
	}
}

Figur* Map::getFigByCoords(int x, int y)
{
	for(int i = 0; i < 4; ++i) {
		switch(gamestat) {
		case S_GELBCHOOSE:
			if(gelbfigurs[i]->hasCoords(x, y))
				return gelbfigurs[i];
				
			break;
		case S_ROTCHOOSE:
			if(rotfigurs[i]->hasCoords(x, y))
				return rotfigurs[i];
				
			break;
		case S_BLAUCHOOSE:
			if(blaufigurs[i]->hasCoords(x, y))
				return blaufigurs[i];
				
			break;
		case S_GRUENCHOOSE:
			if(gruenfigurs[i]->hasCoords(x, y))
				return gruenfigurs[i];
				
			break;
		}		
	}
	
	return NULL;
}

Figur* Map::getFigByField(int field)
{
	for(int i = 0; i < 4; ++i) {
		if(field == gelbfigurs[i]->getField()) {
			return gelbfigurs[i];
		}
		else if(field == rotfigurs[i]->getField())
			return rotfigurs[i];
		else if(field == blaufigurs[i]->getField())
			return blaufigurs[i];
		else if(field == gruenfigurs[i]->getField()) {
			return gruenfigurs[i];
		}
	}
	
	return NULL;
}

void Map::debuginfo()
{
	std::cout << "\n--- Debug ---\n";
	std::cout << "gamestate: " << gamestat << "\n";
	
	for(int i = 0; i < 4; ++i) {
		std::cout << "gelbfigurs[" << i << "]->move_im(" << gelbfigurs[i]->getField() << ");\n";
		std::cout << "rotfigurs[" << i << "]->move_im(" << rotfigurs[i]->getField() << ");\n";
		std::cout << "blaufigurs[" << i << "]->move_im(" << blaufigurs[i]->getField() << ");\n";
		std::cout << "gruenfigurs[" << i << "]->move_im(" << gruenfigurs[i]->getField() << ");\n";
	}
	
	std::cout << "--- /Debug --\n";
	
	CL_System::sleep(2000);
}
