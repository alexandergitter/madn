#include "spiel.h"

Spiel applicationInstance;

//CL_ConsoleWindow Spiel::console = CL_ConsoleWindow("Console");
Spiel::Spiel() {}
Spiel::~Spiel() {}

int Spiel::main(int argc, char **argv)
{
	try {
		CL_SetupCore::init();
		CL_SetupDisplay::init();
		CL_SetupGL::init();
	
		//console.redirect_stdio();
		
		CL_DisplayWindow window("Spiel", 330, 330);
		CL_Slot bla = window.sig_window_close().connect(this, &Spiel::on_window_close);
		
		statem = new Statem();
		statem->run();
		
		CL_SetupGL::deinit();
		CL_SetupDisplay::deinit();
		CL_SetupCore::deinit();
	} catch (CL_Error err) {
	}
	
	return 0;
}

void Spiel::on_window_close()
{
	statem->quit();
}
