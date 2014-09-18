#include "map.h"

CL_Surface* Figur::f_gelb = NULL;
CL_Surface* Figur::f_rot = NULL;
CL_Surface* Figur::f_blau = NULL;
CL_Surface* Figur::f_gruen = NULL;

void Figur::init()
{
	Figur::f_rot = new CL_Surface("figur_rot.tga");
	Figur::f_gruen = new CL_Surface("figur_gruen.tga");
	Figur::f_gelb = new CL_Surface("figur_gelb.tga");
	Figur::f_blau = new CL_Surface("figur_blau.tga");
}

Figur::Figur(int tfield, int col, Map* tmap)
{
	field = tfield;
	color = col;
	map = tmap;
	finish = true;
	
	map->setFigure(field, color);
}

Figur::~Figur() {}

void Figur::draw()
{
	if(finish) {
		CL_Point pos;
		pos = map->getCoords(field, color);
	
		switch(color) {
		case Map::YELLOW:
			f_gelb->draw(pos.x*30+9, pos.y*30);
			break;
		case Map::RED:
			f_rot->draw(pos.x*30+9, pos.y*30);
			break;
		case Map::BLUE:
			f_blau->draw(pos.x*30+9, pos.y*30);
			break;
		case Map::GREEN:
			f_gruen->draw(pos.x*30+9, pos.y*30);
			break;
		}
	} else {
		switch(color) {
		case Map::YELLOW:
			f_gelb->draw(ma_pos.x, ma_pos.y);
			break;
		case Map::RED:
			f_rot->draw(ma_pos.x, ma_pos.y);
			break;
		case Map::BLUE:
			f_blau->draw(ma_pos.x, ma_pos.y);
			break;
		case Map::GREEN:
			f_gruen->draw(ma_pos.x, ma_pos.y);
			break;
		}
	}
}

bool Figur::canMove(int count)
{
	int hackcolor;
	int tempf = field;
	int temp;
	
	if((field < 4) && (count != 6))
		return false;
	else if((field > 43) && (count+field > 47))
		return false;

	if(field < 4)
		count = 1;

	if(color == 1)
		hackcolor = 2;
	else
		hackcolor = 1;
		
	for(int i = 0; i < count-1; ++i) {
		if((color == Map::YELLOW) && (tempf == 43)) {
			tempf = 42+count-i;
			break;
		} else if((color == Map::RED) && (tempf == 13)) {
			tempf = 42+count-i;
			break;
		} else if((color == Map::BLUE) && (tempf == 23)) {
			tempf = 42+count-i;
			break;
		} else if((color == Map::GREEN) && (tempf == 33)) {
			tempf = 42+count-i;
			break;
		}
			
		temp = map->getNextField(tempf, color);
		
		if(temp == -1)
			tempf = map->getNextField(tempf, hackcolor);
		else
			tempf = temp;
			
		if(tempf == -1) {
			return false;
		}
	}
	
	tempf = map->getNextField(tempf, color);
	
	if(tempf == -1) {
		return false;
	}
	else
		return true;
}

void Figur::move_im(int tfield)
{
	map->unsetFigure(field, color);
	map->setFigure(tfield, color);
	field = tfield;
}

void Figur::move_anim(int count)
{
	int hackcolor;
	
	if(color == 1)
		hackcolor = 2;
	else
		hackcolor = 1;

	if(field >= 44)
		ma_act_target = field+1;
	else if((color == Map::YELLOW) && (field == 43)) {
		ma_act_target = 44;
	} else if((color == Map::RED) && (field == 13)) {
		ma_act_target = 44;
	} else if((color == Map::BLUE) && (field == 23)) {
		ma_act_target = 44;
	} else if((color == Map::GREEN) && (field == 33)) {
		ma_act_target = 44;
	} else {
		ma_act_target = map->getNextField(field, color);
	
		if(ma_act_target == -1)
			ma_act_target = map->getNextField(field, hackcolor);
	}
	
	ma_target_pos.x = map->getCoords(ma_act_target, color).x*30+9;
	ma_target_pos.y = map->getCoords(ma_act_target, color).y*30;
	ma_pos.x = map->getCoords(field, color).x*30+9;
	ma_pos.y = map->getCoords(field, color).y*30;
	ma_count = count;
	ma_start_time = CL_System::get_time();
	finish = false;	
	
	if(ma_target_pos.x > ma_pos.x)
		ma_wtm = Map::RIGHT;
	else if(ma_target_pos.x < ma_pos.x)
		ma_wtm = Map::LEFT;
	else if(ma_target_pos.y < ma_pos.y)
		ma_wtm = Map::UP;
	else if(ma_target_pos.y > ma_pos.y)
		ma_wtm = Map::DOWN;
}

bool Figur::hasCoords(int x, int y)
{
	CL_Point pos = map->getCoords(field, color);
	if((x >= pos.x*30) && (x <= pos.x*30+30) && (y >= pos.y*30) && (y <= pos.y*30+30))
		return true;
	else
		return false;
}

void Figur::ma_finish()
{
	if(ma_count == 1) {
		map->setFigure(ma_act_target, color);
		finish = true;
		field = ma_act_target;
	} else {
		field = ma_act_target;
		move_anim(ma_count-1);
	}
}

void Figur::update()
{
	if(!finish) {
		int time_diff = CL_System::get_time() - ma_start_time;
	
		if((ma_wtm == Map::RIGHT) && (ma_pos.x >= ma_target_pos.x)) {
			ma_finish();
		} else if ((ma_wtm == Map::LEFT) && (ma_pos.x <= ma_target_pos.x)) {
			ma_finish();
		} else if ((ma_wtm == Map::UP) && (ma_pos.y <= ma_target_pos.y)) {
			ma_finish();
		} else if ((ma_wtm == Map::DOWN) && (ma_pos.y >= ma_target_pos.y)) {
			ma_finish();
		} else if (ma_wtm == Map::RIGHT) {
			ma_pos.x += (int)(time_diff+11)*0.1f;
		}  else if (ma_wtm == Map::LEFT) {
			ma_pos.x -= (int)time_diff*0.1f;
		}  else if (ma_wtm == Map::UP) {
			ma_pos.y -= (int)time_diff*0.1f;
		}  else if (ma_wtm == Map::DOWN) {
			ma_pos.y += (int)(time_diff+11)*0.1f;
		}
		
		ma_start_time = CL_System::get_time();
	}
	
	
}
