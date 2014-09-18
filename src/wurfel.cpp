#include "wurfel.h"

Wurfel::Wurfel()
{
	CL_SpriteDescription desc;
	desc.add_gridclipped_frames(new CL_TargaProvider("wurfel.tga"), 0, 0, 60, 60, 6, 1, 0, 0, 0, true);
	
	wurfel = new CL_Sprite(desc);
	zahlen = new CL_Surface(new CL_TargaProvider("wurfel.tga"), true);
	
	wurfel->set_show_on_finish(CL_Sprite::show_last_frame);
	srand(CL_System::get_time());
}

Wurfel::~Wurfel() {}

void Wurfel::werf()
{
	value = rand()%6;
	start_time = CL_System::get_time();
	finished = false;
}

void Wurfel::draw()
{
	if((CL_System::get_time() - start_time) > 1000) {
		finished = true;
		zahlen->draw(CL_Rect(value*60, 0, value*60+60, 60), CL_Rect(pos.x, pos.y, pos.x+60, pos.y+60));
	} else {
		wurfel->update();
		wurfel->draw(pos.x, pos.y);
	}
}
