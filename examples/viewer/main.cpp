#include <f3d/f3d.h>

void		loadScene(f3d::Firestone& f3d, void * arg) {
	std::string path((char *)arg);
	std::string file((char *)arg);

	path = path.substr(0, path.find_last_of("/\\") + 1);
	file = file.substr(file.find_last_of("/\\") + 1);

	std::cout << path << std::endl;
	std::cout << file << std::endl;
	f3d.scene->loadFromFile(path, file);

	for (auto it = f3d.scene->getObjects().begin(); it != f3d.scene->getObjects().end(); ++it) {
		//(*it)->rotate(90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
		//(*it)->translate(glm::vec3(0.0f, 0.0f, 0.0f));
		(*it)->scale(glm::vec3(3.0f));
	}
	f3d.scene->getCamera().get()->setPerspective(45.0f, 1280.0f / 720.0f, 0.1f, 1000.0f);
}

void		updateScene(f3d::Firestone& f3d, void * arg) {
	(void)arg;
	const glm::vec3	rotationAxis(0.0, 1.0, 0.0);
	for (auto it = f3d.scene->getObjects().begin(); it != f3d.scene->getObjects().end(); ++it)
		(*it)->rotate(0.01f, rotationAxis);

}

void		keyCallback(f3d::Firestone& f3d, f3d::utils::KeyInput& keyEvent, void *arg) {
	(void)arg;

	std::cout << keyEvent << std::endl;

	if (keyEvent.keycode == 256 && keyEvent.state == keyEvent.F3D_KEY_STATE_PRESS)
		f3d.stop();

}

void		mouseCallback(f3d::Firestone& f3d, f3d::utils::MouseInput& mouseEvent, void *arg) {
	(void)arg;
	std::cout << mouseEvent << std::endl;
}

void		joystickCallback(f3d::Firestone& f3d, f3d::utils::JoystickInput& joystickEvent, void *arg) {
	(void)arg;
	if (joystickEvent.type == joystickEvent.F3D_JOYSTICK_INPUT_CONNECTED ||
		joystickEvent.type == joystickEvent.F3D_JOYSTICK_INPUT_DISCONNECTED)
		std::cout << joystickEvent << std::endl;
	if (joystickEvent.type == joystickEvent.F3D_JOYSTICK_INPUT_BUTTON &&
		joystickEvent.buttonState == joystickEvent.F3D_JOYSTICK_BUTTON_PRESS)
		std::cout << joystickEvent << std::endl;
}

int main(int ac, char **av) {
	if (ac != 2) {
		F3D_ERROR("No file to load");
		return 1;
	}

	f3d::Firestone	*engine = f3d::getF3D();

	engine->startCallback(loadScene, av[1]);
	engine->drawCallback(updateScene, NULL);
	engine->keybordEventsCallback(keyCallback, NULL);
	engine->mouseEventsCallback(mouseCallback, NULL);
	engine->joystickEventsCallback(joystickCallback, NULL);
	engine->settings->applicationName.assign("Sample viewer");
	engine->settings->fpsCap = 60;
	engine->settings->windowWidth = 1280;
	engine->settings->windowHeight = 720;
	engine->settings->fullScreen = false;
	engine->execute();
	
	return 0;
}