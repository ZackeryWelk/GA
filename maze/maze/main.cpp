#include "mazeApp.h"

int main() {
	
	// allocation
	auto app = new mazeApp();

	// initialise and loop
	app->run("AIE", app->getScreenWidth(), app->getScreenHeight(), false);

	// deallocation
	delete app;

	return 0;
}