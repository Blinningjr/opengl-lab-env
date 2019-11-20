//------------------------------------------------------------------------------
// main.cc
// (C) 2015-2017 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "config.h"
#include "triangulation3dapp.h"

int main(int argc, const char** argv) {
	Triangulation3d::Triangulation3dApp app;
	if (app.Open())
	{
		app.Run();
		app.Close();
	}
	app.Exit();
}
