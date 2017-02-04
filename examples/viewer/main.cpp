#include <f3d/f3d.h>

void		loadScene(f3d::Firestone& f3d, void * arg) {
	f3d.scene->loadFromFile((char *)arg);

	for (auto it = f3d.scene->getObjects().begin(); it != f3d.scene->getObjects().end(); ++it) {
		(*it)->translate(glm::vec3(0.0f, -100.0f, 0.0f));
		(*it)->scale(glm::vec3(1000.0f));
	}
}

void		updateScene(f3d::Firestone& f3d, void * arg) {
	const glm::vec3	rotationAxis(0.0, 1.0, 0.0);
	for (auto it = f3d.scene->getObjects().begin(); it != f3d.scene->getObjects().end(); ++it)
		(*it)->rotate(0.1, rotationAxis);
}

int main(int ac, char **av) {
	if (ac != 2) {
		F3D_ERROR("No file to load");
		return 1;
	}


	f3d::Firestone	*engine = f3d::getF3D();

	engine->startCallback(loadScene, av[1]);
	engine->drawCallback(updateScene, NULL);
	engine->settings->applicationName.assign("Sample viewer");
	engine->settings->fpsCap = 60;
	engine->settings->windowWidth = 1280;
	engine->settings->windowHeight = 720;
	engine->settings->fullScreen = false;
	engine->execute();
	
	return 0;
}