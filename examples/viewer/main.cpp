#include <f3d/f3d.h>
#include <f3d/helpers/xbox_360.hpp>

void		loadScene(f3d::Firestone& f3d, void * arg) {
	std::string path((char *)arg);
	std::string file((char *)arg);

	path = path.substr(0, path.find_last_of("/\\") + 1);
	file = file.substr(file.find_last_of("/\\") + 1);

	std::cout << path << std::endl;
	std::cout << file << std::endl;
	f3d.scene->loadFromFile(path, file);

	for (auto it = f3d.scene->getObjects().begin(); it != f3d.scene->getObjects().end(); ++it) {
		(*it)->scale(glm::vec3(4.0f));
	}
	f3d.scene->getCamera()->setPerspective2(156.5f, 30720.0f / 4320.0f, 0.1f, 2048.0f);
	f3d.scene->getCamera()->lookAt(glm::vec3(0.0f, 25.0f, 400.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
}

void		updateScene(f3d::Firestone& f3d, void * arg) {
	static float distance = 400.0f;

	static glm::vec3 center;
	static float angleX = 0.0f, angleY = 0.0f;

	/*
		left sitck move regarding to forward vector
		right sitck pan camera left / right
		RL y aix down, RT y axis up
	*/

	f3d::helpers::Xbox360Controller* joystick = (f3d::helpers::Xbox360Controller*)arg;

	center[0] += joystick->axisState(joystick->AXIS_LS_X) * 2.25f;
	center[2] += joystick->axisState(joystick->AXIS_LS_Y) * 2.25f;
	center[1] += joystick->isButtonPressed(joystick->BUTTON_LS) ? 1.0f : 0.0f;
	center[1] += joystick->isButtonPressed(joystick->BUTTON_RS) ? -1.0f : 0.0f;
	
	distance += ( - (joystick->axisState(joystick->AXIS_LT) + 1.0f) * 10.0f); // Zoom out on Left trigger
	distance += (joystick->axisState(joystick->AXIS_RT) + 1.0f) * 10.0f; //Zoom in on right trigger

	glm::vec3 eye(0.0f, 0.0f, distance);

	angleX += joystick->axisState(joystick->AXIS_RS_Y) / 10.0f;
	angleY += joystick->axisState(joystick->AXIS_RS_X) / 10.0f;

	eye = (glm::mat3(glm::rotate(angleX, glm::vec3(1.0, 0.0, 0.0))) * (eye - center)) + center;
	eye = (glm::mat3(glm::rotate(angleY, glm::vec3(0.0, -1.0, 0.0))) * (eye - center)) + center;

	f3d.scene->getCamera()->lookAt(eye, center, glm::vec3(0.0f, -1.0f, 0.0f));
}

void		keyCallback(f3d::Firestone& f3d, f3d::utils::KeyInput& keyEvent, void *arg) {
	(void)arg;

	if (keyEvent.keycode == 256 && keyEvent.state == keyEvent.F3D_KEY_STATE_PRESS)
		f3d.stop();

}

void		mouseCallback(f3d::Firestone& f3d, f3d::utils::MouseInput& mouseEvent, void *arg) {
	(void)arg;
}

void		joystickCallback(f3d::Firestone& f3d, f3d::utils::JoystickInput& joystickEvent, void *arg) {
	f3d::helpers::Xbox360Controller* joystick = (f3d::helpers::Xbox360Controller*)arg;

	joystick->update(joystickEvent);
}

int main(int ac, char **av) {
	if (ac != 2) {
		F3D_ERROR("No file to load");
		return 1;
	}

	f3d::helpers::Xbox360Controller joystick;
	f3d::Firestone	*engine = f3d::getF3D();

	engine->startCallback(loadScene, av[1]);
	engine->drawCallback(updateScene, &joystick);
	engine->keybordEventsCallback(keyCallback, NULL);
	engine->mouseEventsCallback(mouseCallback, NULL);
	engine->joystickEventsCallback(joystickCallback, &joystick);
	engine->settings->applicationName.assign("Sample viewer");
	engine->settings->fpsCap = 45;
	engine->settings->windowWidth = 1280;
	engine->settings->windowHeight = 720;
	engine->settings->fullScreen = false;
	engine->execute();
	
	return 0;
}