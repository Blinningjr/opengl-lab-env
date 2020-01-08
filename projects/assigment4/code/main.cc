#include "config.h"
#include "SimpleGraphics.h"

int main(int argc, const char** argv) {
	Graphics3D::SimpleGraphics app;
	if (app.Open())
	{
		app.Run();
		app.Close();
	}
	app.Exit();
}
