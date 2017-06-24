#include "demo.h"

Demo::Demo()
{
	distance = 400.0f;
	center;
	angleX = 0.0f;
	angleY = 0.0f;

	speed = 1.0;
}

Demo::~Demo() 
{

}

bool		Demo::init(const std::list<std::string>& texture_mesh, const std::list<std::string>& texture_material, \
					   const std::list<std::string>& sequence_mesh, const std::list<std::string>& sequence_frame) 
{
	//Get f3d engine pointer once
	engine = f3d::getF3D();
	if (engine == nullptr)
		return false;

	//Copy args
	_texture_mesh_str.assign(texture_mesh.begin(), texture_mesh.end());
	_texture_material_str.assign(texture_material.begin(), texture_material.end());
	_sequence_mesh_str.assign(sequence_mesh.begin(), sequence_mesh.end());
	_sequence_frame_str.assign(sequence_frame.begin(), sequence_frame.end());

	return true;
}

typedef std::function<void(f3d::Firestone& f3d, void * arg)> start_handle_t;

bool		Demo::run() 
{
	auto start_fn = std::bind(&Demo::loadScene, this, std::placeholders::_1, std::placeholders::_2);
	auto draw_fn = std::bind(&Demo::updateScene, this, std::placeholders::_1, std::placeholders::_2);
	auto keyboard_fn = std::bind(&Demo::keyCallback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
	auto mouse_fn = std::bind(&Demo::mouseCallback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
	auto joystick_fn = std::bind(&Demo::joystickCallback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

	engine->startCallback(start_fn, nullptr);
	engine->drawCallback(draw_fn, nullptr);
	engine->keybordEventsCallback(keyboard_fn, nullptr);
	engine->mouseEventsCallback(mouse_fn, nullptr);
	engine->joystickEventsCallback(joystick_fn, nullptr);
	engine->settings->applicationName.assign("Sample viewer");
	engine->settings->fpsCap = 40;
	engine->settings->windowWidth = 1280;
	engine->settings->windowHeight = 720;
	engine->settings->fullScreen = false;
	engine->settings->windowClearColor = glm::vec4(0.6, 0.6, 0.6, 1.0);
	engine->execute();

	return true;
}

bool		Demo::clean()
{
	return false;
}

void		Demo::loadScene(f3d::Firestone& f3d, void * arg)
{
	f3d::tree::Light		l;

	//Load sequence_mesh objects
	for (auto it = _sequence_mesh_str.begin(); it != _sequence_mesh_str.end(); ++it) {
		std::string path(*it);
		std::string file(*it);

		path = path.substr(0, path.find_last_of("/\\") + 1);
		file = file.substr(file.find_last_of("/\\") + 1);

		f3d.scene->loadFromFile(path, file);
		_sequence_mesh.push_back(f3d.scene->getObjects().back());
	}
	//Load sequence_frames objects
	for (auto it = _sequence_frame_str.begin(); it != _sequence_frame_str.end(); ++it) {
		std::string path(*it);
		std::string file(*it);

		path = path.substr(0, path.find_last_of("/\\") + 1);
		file = file.substr(file.find_last_of("/\\") + 1);

		f3d.scene->loadFromFile(path, file);
		_sequence_frame.push_back(f3d.scene->getObjects().back());
	}
	//Load texture_mesh objects
	for (auto it = _texture_mesh_str.begin(); it != _texture_mesh_str.end(); ++it) {
		std::string path(*it);
		std::string file(*it);

		path = path.substr(0, path.find_last_of("/\\") + 1);
		file = file.substr(file.find_last_of("/\\") + 1);

		f3d.scene->loadFromFile(path, file);
		_texture_mesh.push_back(f3d.scene->getObjects().back());
	}
	//Load texture_material objects
	for (auto it = _texture_material_str.begin(); it != _texture_material_str.end(); ++it) {
		f3d::tree::Material *mat = engine->scene->getMaterialByName(*it);
		_texture_material.push_back(mat);
	}


	f3d.scene->getCamera()->setPerspective(30.0f, 1280.0f / 720.0f, 0.1f, 2048.0f);
	//f3d.scene->getCamera()->setPerspective2(156.5f, 30720.0f / 4320.0f, 0.1f, 2048.0f);
	f3d.scene->getCamera()->lookAt(glm::vec3(0.0f, 120.0f, -800.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));

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

void		Demo::updateScene(f3d::Firestone& f3d, void * arg)
{
	glm::mat4							r;
	glm::mat4							r2;
	glm::mat4							t;
	float								x = 0.0f;
	float								y = 0.0f;
	float								z = 0.0f;
	float								ax = 0.0f;
	float								ay = 0.0f;

	(void)arg;

	//Camera controls management
	if (joystick.isButtonPressed(joystick.BUTTON_LB))
		speed *= 1.1f;
	if (joystick.isButtonPressed(joystick.BUTTON_RB))
		speed *= 0.92f;

	z += joystick.axisState(joystick.AXIS_LS_Y) * 2.0f * speed;
	x += joystick.axisState(joystick.AXIS_LS_X) * 2.0f;
	y += (-(joystick.axisState(joystick.AXIS_LT) + 1.0f) * 2.0f); // Y up Left trigger
	y += (joystick.axisState(joystick.AXIS_RT) + 1.0f) * 2.0f; //Y down on right trigger
	ay += joystick.axisState(joystick.AXIS_RS_X) / 50.0f;
	ax += joystick.axisState(joystick.AXIS_RS_Y) / 90.0f;

	t = glm::translate(t, glm::vec3(x, y, z));
	if (ay != 0.0f)
		r = glm::rotate(ay, glm::vec3(0.0f, -1.0f, 0.0f));
	if (ax != 0.0f)
		r2 = glm::rotate(ax, glm::vec3(1.0f, 0.0f, 0.0f));
	f3d.scene->getCamera()->setView(t * r2 * r * f3d.scene->getCamera()->getView());

	/*
	//Sequence controls management
	if (joystick->isButtonPressed(joystick->BUTTON_B))
		sequence_btn = true;
	if (joystick->isButtonReleased(joystick->BUTTON_B) && sequence_btn == true && sequence.empty() == false) {
		sequence_playing = !sequence_playing;
		sequence_btn = false;
		if (sequence_playing) { // Start playing
			(*sequence_it)->translate(glm::vec3(0.0, 100000.0, 0.0));
			sequence_timer.restart();
		}
		else {
			(*sequence_it)->translate(glm::vec3(0.0, -100000.0, 0.0));
			sequence_it = sequence.begin();
			sequence_frequency = 0;
			sequence_timer.stop();
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
	if (sequence_playing == true && sequence.empty() == false && sequence_timer.seconds() >= 1) {
		sequence_frequency++;
		(*sequence_it)->translate(glm::vec3(0.0, -100000.0, 0.0));
		sequence_it++;
		if (sequence_it == sequence.end())
			sequence_it = sequence.begin();
		(*sequence_it)->translate(glm::vec3(0.0, 100000.0, 0.0));
		sequence_frequency = 0;
		sequence_timer.restart();
	}
	*/
}

void		Demo::keyCallback(f3d::Firestone& f3d, f3d::utils::KeyInput& keyEvent, void *arg)
{
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
	/*
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
	*/
	r = glm::rotate(ay, glm::vec3(0.0, -1.0, 0.0));
	t = glm::translate(glm::mat4(), glm::vec3(0.0f, 0.0f, z));
	f3d.scene->getCamera()->setView(t * r * f3d.scene->getCamera()->getView());
}

void		Demo::mouseCallback(f3d::Firestone& f3d, f3d::utils::MouseInput& mouseEvent, void *arg)
{
	(void)arg;
	if (mouseEvent.type == mouseEvent.F3D_MOUSE_INPUT_BUTTON && \
		mouseEvent.buttonState == mouseEvent.F3D_MOUSE_BUTTON_PRESS) {
		std::cout << mouseEvent << std::endl;
	}
}

void		Demo::joystickCallback(f3d::Firestone& f3d, f3d::utils::JoystickInput& joystickEvent, void *arg)
{
	joystick.update(joystickEvent);
}