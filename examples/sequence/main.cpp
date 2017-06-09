#include <f3d/f3d.h>
#include <f3d/helpers/xbox_360.hpp>

static float distance = 400.0f;
static glm::vec3 center;
static float angleX = 0.0f, angleY = 0.0f;

static glm::mat4 r;
static glm::mat4 r2;
static glm::mat4 t;

static uint32_t									sequence_frequency = 0;
static std::list<f3d::tree::Object*>::iterator	sequence_it;

void		loadScene(f3d::Firestone& f3d, void * arg) {
	std::list<std::string>*	sequence = static_cast<std::list<std::string>*>(arg);
	f3d::tree::Light		l;


	for (auto it = sequence->begin(); it != sequence->end(); ++it) {
		std::string path(*it);
		std::string file(*it);

		path = path.substr(0, path.find_last_of("/\\") + 1);
		file = file.substr(file.find_last_of("/\\") + 1);

		f3d.scene->loadFromFile(path, file);
	}

	for (auto it = f3d.scene->getObjects().begin(); it != f3d.scene->getObjects().end(); ++it) {
		//(*it)->rotate(90.0f * 3.14f / 180.0f, glm::vec3(-1.0f, 0.0f, 0.0f));
		(*it)->scale(glm::vec3(4.0f));
		(*it)->translate(glm::vec3(0.0, -100000.0, 0.0));
	}
	sequence_it = f3d.scene->getObjects().begin();
	(*sequence_it)->translate(glm::vec3(0.0, 100000.0, 0.0));
	sequence_frequency = 0;

	//f3d.scene->getCamera()->setPerspective(30.0f, 1280.0f / 720.0f, 0.1f, 2048.0f);
	f3d.scene->getCamera()->setPerspective2(156.5f, 30720.0f / 4320.0f, 0.1f, 2048.0f);
	f3d.scene->getCamera()->lookAt(glm::vec3(0.0f, 25.0f, 400.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));

	l.setName("Test light1");
	l.setColorAmbient(glm::vec3(0.0f));
	l.setColorDiffuse(glm::vec3(-1.0, 1.0, 1.0));
	l.setDirection(glm::vec3(1.0, 1.0, 0.0));
	l.setPosition(glm::vec3(0.0, 0.0, -42.0));
	f3d.scene->addLight(&l);

	l.setName("Test light2");
	l.setColorAmbient(glm::vec3(0.1f));
	l.setColorDiffuse(glm::vec3(0.0, 0.0, 1.0));
	l.setDirection(glm::vec3(1.0, 1.0, 0.0));
	l.setPosition(glm::vec3(0.0, 0.0, -44.0));
	f3d.scene->addLight(&l);
}

void				updateScene(f3d::Firestone& f3d, void * arg) {
	glm::mat4		r;
	glm::mat4		r2;
	glm::mat4		t;
	/*
		left stick move regarding to forward vector
		right stick pan camera left / right
		RL y aix down, RT y axis up
	*/

	sequence_frequency++;
	if (sequence_frequency % 10 == 0) { // Every 20 frames
		(*sequence_it)->translate(glm::vec3(0.0, -100000.0, 0.0));
		sequence_it++;
		if (sequence_it == f3d.scene->getObjects().end())
			sequence_it = f3d.scene->getObjects().begin();
		(*sequence_it)->translate(glm::vec3(0.0, 100000.0, 0.0));
		sequence_frequency = 0;
	}


	f3d::helpers::Xbox360Controller* joystick = (f3d::helpers::Xbox360Controller*)arg;
	
	float z = 0.0f;
	z += joystick->axisState(joystick->AXIS_LS_Y) * 2.0f;
	float x = 0.0f;
	x += joystick->axisState(joystick->AXIS_LS_X) * 2.0f;
	float y = 0.0f;
	y += (-(joystick->axisState(joystick->AXIS_LT) + 1.0f) * 2.0f); // Y up Left trigger
	y += (joystick->axisState(joystick->AXIS_RT) + 1.0f) * 2.0f; //Y down on right trigger
	t = glm::translate(t, glm::vec3(x, y, z));

	float ay = 0.0f;
	ay += joystick->axisState(joystick->AXIS_RS_X) / 50.0f;
	float ax = 0.0f;
	ax += joystick->axisState(joystick->AXIS_RS_Y) / 90.0f;

	if (ay != 0.0f)
		r = glm::rotate(ay, glm::vec3(0.0f, -1.0f, 0.0f));
	if (ax != 0.0f)
		r2 = glm::rotate(ax, glm::vec3(1.0f, 0.0f, 0.0f));

	f3d.scene->getCamera()->setView(t * r2 * r * f3d.scene->getCamera()->getView());
}

void		keyCallback(f3d::Firestone& f3d, f3d::utils::KeyInput& keyEvent, void *arg) {
	(void)arg;

	float z = 0.0f;
	float ay = 0.0f;

	if (keyEvent.keycode == 256 && keyEvent.state == keyEvent.F3D_KEY_STATE_PRESS)
		f3d.stop();

	std::cout << keyEvent.keycode << std::endl;
	if (keyEvent.keycode == 331 && (keyEvent.state == keyEvent.F3D_KEY_STATE_PRESS || keyEvent.state == keyEvent.F3D_KEY_STATE_REPEAT)) //Left arrow
		ay += -0.1f;
	if (keyEvent.keycode == 333 && (keyEvent.state == keyEvent.F3D_KEY_STATE_PRESS || keyEvent.state == keyEvent.F3D_KEY_STATE_REPEAT)) //Right arrow
		ay += 0.1f;
	if (keyEvent.keycode == 328 && (keyEvent.state == keyEvent.F3D_KEY_STATE_PRESS || keyEvent.state == keyEvent.F3D_KEY_STATE_REPEAT)) //Top arrow
		z = 5.0f;
	if (keyEvent.keycode == 336 && (keyEvent.state == keyEvent.F3D_KEY_STATE_PRESS || keyEvent.state == keyEvent.F3D_KEY_STATE_REPEAT)) //Down arrow
		z = -5.0f;

	r = glm::rotate(ay, glm::vec3(0.0, -1.0, 0.0));
	t = glm::translate(glm::mat4(), glm::vec3(0.0f, 0.0f, z));
	f3d.scene->getCamera()->setView(t * r * f3d.scene->getCamera()->getView());
}

void		mouseCallback(f3d::Firestone& f3d, f3d::utils::MouseInput& mouseEvent, void *arg) {
	(void)arg;
	if (mouseEvent.type == mouseEvent.F3D_MOUSE_INPUT_BUTTON && \
		mouseEvent.buttonState == mouseEvent.F3D_MOUSE_BUTTON_PRESS) {
		std::cout << mouseEvent << std::endl;
	}
}

void		joystickCallback(f3d::Firestone& f3d, f3d::utils::JoystickInput& joystickEvent, void *arg) {
	f3d::helpers::Xbox360Controller* joystick = (f3d::helpers::Xbox360Controller*)arg;

	joystick->update(joystickEvent);
}

int main(int ac, char **av) {

	std::list<std::string>			sequence;

	for (uint32_t i = 1; i < ac; i++) {
		sequence.push_back(av[i]);
	}

	f3d::helpers::Xbox360Controller joystick;
	f3d::Firestone	*engine = f3d::getF3D();

	engine->startCallback(loadScene, &sequence);
	engine->drawCallback(updateScene, &joystick);
	engine->keybordEventsCallback(keyCallback, NULL);
	engine->mouseEventsCallback(mouseCallback, NULL);
	engine->joystickEventsCallback(joystickCallback, &joystick);
	engine->settings->applicationName.assign("Sample viewer");
	engine->settings->fpsCap = 60;
	engine->settings->windowWidth = 1280;
	engine->settings->windowHeight = 720;
	engine->settings->fullScreen = false;
	engine->execute();
	
	return 0;
}