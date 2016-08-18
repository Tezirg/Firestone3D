#include <f3d/f3d.h>

void		loadScene(f3d::Firestone& f3d, void * arg) {
	f3d.scene->loadFromFile("../../../assets/Adventurer-Militia/Militia-Adventurer-RIGGED.blend");
}

int main() {
	f3d::Firestone	*engine = f3d::getF3D();


	engine->startCallback(loadScene, NULL);
	engine->settings->applicationName.assign("Sample viewer");
	engine->settings->fpsCap = 60;
	engine->settings->windowWidth = 1280;
	engine->settings->windowHeight = 720;
	engine->settings->fullScreen = false;
	engine->execute();
	
	return 0;
}