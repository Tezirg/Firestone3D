#include <f3d/f3d.h>

int main() {
	f3d::Firestone	*engine = f3d::getF3D();

	engine->settings->windowTitle.assign("Sample viewer");
	engine->settings->fpsCap = 60;
	engine->settings->windowWidth = 1280;
	engine->settings->windowHeight = 720;
	engine->settings->fullScreen = true;
	engine->execute();

	return 0;
}