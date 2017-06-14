#include <f3d/f3d.h>
#include <f3d/helpers/xbox_360.hpp>
#include <list>

#include "getopt.h"

static float distance = 400.0f;
static glm::vec3 center;
static float angleX = 0.0f, angleY = 0.0f;

static glm::mat4 r;
static glm::mat4 r2;
static glm::mat4 t;

static bool										domain_btn = false;
std::list<std::string>							domain_str;
static std::list<f3d::tree::Object *>			domain;
static std::list<f3d::tree::Object*>::iterator	domain_it;

static bool										sequence_btn = false;
static std::list<std::string>					sequence_str;
static std::list<f3d::tree::Object *>			sequence;
static uint32_t									sequence_frequency = 0;
static std::list<f3d::tree::Object*>::iterator	sequence_it;
static bool										sequence_playing = false;

void		loadScene(f3d::Firestone& f3d, void * arg) {
	f3d::tree::Light		l;

	//Load sequence objects
	for (auto it = sequence_str.begin(); it != sequence_str.end(); ++it) {
		std::string path(*it);
		std::string file(*it);

		path = path.substr(0, path.find_last_of("/\\") + 1);
		file = file.substr(file.find_last_of("/\\") + 1);

		f3d.scene->loadFromFile(path, file);
		sequence.push_back(f3d.scene->getObjects().back());
	}
	//Adjust sequence objects
	for (auto it = sequence.begin(); it != sequence.end(); ++it) {
		for (auto child_it = (*it)->getRoot()->getChildren().begin(); child_it != (*it)->getRoot()->getChildren().end(); ++child_it) {
			for (auto mat_it = (*child_it)->getMeshes().begin(); mat_it != (*child_it)->getMeshes().end(); ++mat_it) {
				auto mat = f3d.scene->getMaterialByName((*mat_it)->getMaterialName());
				std::cout << mat->getName() << std::endl;
				mat->setColor(f3d::F3D_COLOR_DIFFUSE, glm::vec3(0.640f)); // Nice blue color
				mat->setColor(f3d::F3D_COLOR_AMBIENT, glm::vec3(0.000, 0.749f, 1.000f)); // Gray diffuse spectrum
			}
		}
		(*it)->scale(glm::vec3(0.5f));
		(*it)->translate(glm::vec3(0.0, -100000.0, 0.0));
	}
	//Prepare sequence play
	sequence_it = sequence.begin();
	sequence_frequency = 0;


	//Load Domain object
	for (auto it = domain_str.begin(); it != domain_str.end(); ++it) {
		std::string path(*it);
		std::string file(*it);

		path = path.substr(0, path.find_last_of("/\\") + 1);
		file = file.substr(file.find_last_of("/\\") + 1);

		f3d.scene->loadFromFile(path, file);
		domain.push_back(f3d.scene->getObjects().back());
	}
	//Adjust Domain objects
	for (auto it = domain.begin(); it != domain.end(); ++it) {
		(*it)->scale(glm::vec3(0.5f));
		(*it)->translate(glm::vec3(0.0, -100000.0, 0.0));
	}
	//Still show the first one
	domain.front()->translate(glm::vec3(0.0, 100000.0, 0.0));
	domain_it = domain.begin();


	//f3d.scene->getCamera()->setPerspective(30.0f, 1280.0f / 720.0f, 0.1f, 2048.0f);
	f3d.scene->getCamera()->setPerspective2(156.5f, 30720.0f / 4320.0f, 0.1f, 2048.0f);
	f3d.scene->getCamera()->lookAt(glm::vec3(0.0f, 120.0f, -800.0f), glm::vec3(0.0f, 20.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));

	l.setName("light1");
	//l.setType(f3d::F3D_LIGHT_POINT);
	l.setAttenuationConstant(0.1f);
	l.setAttenuationLinear(0.05f);
	l.setColorAmbient(glm::vec3(0.2f));
	l.setColorDiffuse(glm::vec3(1.0f));
	l.setColorSpecular(glm::vec3(0.0f));
	l.setDirection(glm::vec3(0.0f, 1.0f, -1.0f));
	l.setPosition(glm::vec3(0.0f, 20.0f, 0.0f));
	f3d.scene->addLight(&l);

	l.setName("light2");
	//l.setColorDiffuse(glm::vec3(0.0f, 0.0f, 0.2f));
	l.setDirection(glm::vec3(0.0f, 1.0f, 1.0f));
	f3d.scene->addLight(&l);


}

void									updateScene(f3d::Firestone& f3d, void * arg) {
	f3d::helpers::Xbox360Controller*	joystick = (f3d::helpers::Xbox360Controller*)arg; 
	glm::mat4							r;
	glm::mat4							r2;
	glm::mat4							t;
	float								x = 0.0f;
	float								y = 0.0f;
	float								z = 0.0f;
	float								ax = 0.0f;
	float								ay = 0.0f;

	//Camera contrls management
	z += joystick->axisState(joystick->AXIS_LS_Y) * 2.0f;
	x += joystick->axisState(joystick->AXIS_LS_X) * 2.0f;
	y += (-(joystick->axisState(joystick->AXIS_LT) + 1.0f) * 2.0f); // Y up Left trigger
	y += (joystick->axisState(joystick->AXIS_RT) + 1.0f) * 2.0f; //Y down on right trigger
	ay += joystick->axisState(joystick->AXIS_RS_X) / 50.0f;
	ax += joystick->axisState(joystick->AXIS_RS_Y) / 90.0f;

	t = glm::translate(t, glm::vec3(x, y, z));
	if (ay != 0.0f)
		r = glm::rotate(ay, glm::vec3(0.0f, -1.0f, 0.0f));
	if (ax != 0.0f)
		r2 = glm::rotate(ax, glm::vec3(1.0f, 0.0f, 0.0f));
	f3d.scene->getCamera()->setView(t * r2 * r * f3d.scene->getCamera()->getView());


	//Sequence controls management
	if (joystick->isButtonPressed(joystick->BUTTON_B))
		sequence_btn = true;
	if (joystick->isButtonReleased(joystick->BUTTON_B) && sequence_btn == true && sequence.empty() == false) {
		sequence_playing = !sequence_playing;
		sequence_btn = false;
		if (sequence_playing) // Start playing
			(*sequence_it)->translate(glm::vec3(0.0, 100000.0, 0.0));
		else {
			(*sequence_it)->translate(glm::vec3(0.0, -100000.0, 0.0));
			sequence_it = sequence.begin();
			sequence_frequency = 0;
		}
	}
	//Domain controls management
	if (joystick->isButtonPressed(joystick->BUTTON_A))
		domain_btn = true;
	if (joystick->isButtonReleased(joystick->BUTTON_A) && domain_btn == true) {
		(*domain_it)->translate(glm::vec3(0.0, -100000.0, 0.0));
		++domain_it;
		if (domain_it == domain.end())
			domain_it = domain.begin();
		(*domain_it)->translate(glm::vec3(0.0, 100000.0, 0.0));
		domain_btn = false;
	}

	//Update sequence if playing
	if (sequence_playing == true && sequence.empty() == false) {
		sequence_frequency++;
		(*sequence_it)->translate(glm::vec3(0.0, -100000.0, 0.0));
		sequence_it++;
		if (sequence_it == sequence.end())
			sequence_it = sequence.begin();
		(*sequence_it)->translate(glm::vec3(0.0, 100000.0, 0.0));
		sequence_frequency = 0;
	}

}

void		keyCallback(f3d::Firestone& f3d, f3d::utils::KeyInput& keyEvent, void *arg) {
	(void)arg;

	float z = 0.0f;
	float ay = 0.0f;

	if (keyEvent.keycode == 256 && keyEvent.state == keyEvent.F3D_KEY_STATE_PRESS)
		f3d.stop();

	std::cout << std::dec << keyEvent.keycode << std::endl;
	if (keyEvent.keycode == 331 && (keyEvent.state == keyEvent.F3D_KEY_STATE_PRESS || keyEvent.state == keyEvent.F3D_KEY_STATE_REPEAT)) //Left arrow
		ay += -0.1f;
	if (keyEvent.keycode == 333 && (keyEvent.state == keyEvent.F3D_KEY_STATE_PRESS || keyEvent.state == keyEvent.F3D_KEY_STATE_REPEAT)) //Right arrow
		ay += 0.1f;
	if (keyEvent.keycode == 328 && (keyEvent.state == keyEvent.F3D_KEY_STATE_PRESS || keyEvent.state == keyEvent.F3D_KEY_STATE_REPEAT)) //Top arrow
		z = 5.0f;
	if (keyEvent.keycode == 336 && (keyEvent.state == keyEvent.F3D_KEY_STATE_PRESS || keyEvent.state == keyEvent.F3D_KEY_STATE_REPEAT)) {//Down arrow
		z = -5.0f;
	}

	if (keyEvent.keycode == 57 && keyEvent.state == keyEvent.F3D_KEY_STATE_RELEASE && sequence.empty() == false) { // Space bar
		sequence_playing = !sequence_playing;
		sequence_btn = false;
		if (sequence_playing) // Start playing
			(*sequence_it)->translate(glm::vec3(0.0, 100000.0, 0.0));
		else {
			(*sequence_it)->translate(glm::vec3(0.0, -100000.0, 0.0));
			sequence_it = sequence.begin();
			sequence_frequency = 0;
		}
	}
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

int				main(int ac, char **av) {
	int32_t		flag = -1;
	option		opt[] = {
		{ "domain", 1, 0, 0 },
		{ "sequence", 1, 0, 1 },
		{ 0, 0, 0, 0 }
	};

	while ((flag = getopt_long(ac, av, "", opt, 0)) != -1) {
		switch (flag) {
			case 0:
				domain_str.push_back(optarg);
				std::cout << "Domain:" << optarg << std::endl;
				break;
			case 1:
				sequence_str.push_back(optarg);
				std::cout << "Serquence:" << optarg << std::endl;
				break;
			default:
				break;
		}
	}


	f3d::helpers::Xbox360Controller joystick;
	f3d::Firestone	*engine = f3d::getF3D();

	engine->startCallback(loadScene, &sequence);
	engine->drawCallback(updateScene, &joystick);
	engine->keybordEventsCallback(keyCallback, NULL);
	engine->mouseEventsCallback(mouseCallback, NULL);
	engine->joystickEventsCallback(joystickCallback, &joystick);
	engine->settings->applicationName.assign("Sample viewer");
	engine->settings->fpsCap = 40;
	engine->settings->windowWidth = 1280;
	engine->settings->windowHeight = 720;
	engine->settings->fullScreen = false;
	engine->execute();
	
	return 0;
}