#include <f3d/f3d.h>

int main() {
	f3d::Firestone	*engine = f3d::getF3D();
	f3d::tree::Scene	*scene = f3d::tree::Scene::loadFromFile("../../../assets/Adventurer-Militia/Militia-Adventurer-RIGGED.blend");


	engine->settings->applicationName.assign("Sample viewer");
	engine->settings->fpsCap = 60;
	engine->settings->windowWidth = 1280;
	engine->settings->windowHeight = 720;
	engine->settings->fullScreen = false;
	engine->execute();
	
	return 0;
}