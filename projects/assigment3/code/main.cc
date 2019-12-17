#include "config.h"
#include "Simple3DGraphics.h"

int main(int argc, const char** argv) {
	Simple3DGraphics::Simple3DGraphics app;
	if (app.Open())
	{
		app.Run();
		app.Close();
	}
	app.Exit();
}
