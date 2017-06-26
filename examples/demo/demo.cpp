#include "demo.h"

Demo::Demo()
{
	_demo_index = 0;
	_btn_a = false;
	_btn_b = false;
	_btn_x = false;
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
	engine->settings->applicationName.assign("Demo");
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
		auto obj = f3d.scene->getObjects().back();
		_sequence_mesh.push_back(obj);
		obj->scale(glm::vec3(0.5f));
		hideObject(obj);

	}
	//Load sequence_frames objects
	for (auto it = _sequence_frame_str.begin(); it != _sequence_frame_str.end(); ++it) {
		std::string path(*it);
		std::string file(*it);

		path = path.substr(0, path.find_last_of("/\\") + 1);
		file = file.substr(file.find_last_of("/\\") + 1);

		f3d.scene->loadFromFile(path, file);
		auto obj = f3d.scene->getObjects().back();
		_sequence_frame.push_back(obj);
		obj->scale(glm::vec3(0.5f));
		hideObject(obj);
	}
	_sequence_it = _sequence_frame.begin();

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
	_texture_material_it = _texture_material.begin();

	//Attach camera to fly view helper
	fly_camera.attachCamera(engine->scene->getCamera());

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

	// /*
	l.setName("light2");
	//l.setColorDiffuse(glm::vec3(0.0f, 0.0f, 0.2f));
	l.setDirection(glm::vec3(0.0f, 1.0f, 1.0f));
	f3d.scene->addLight(&l);
	// */
}

void		Demo::hideObject(f3d::tree::Object *object) 
{
	object->translate(glm::vec3(0.0, -100000.0, 0.0));
}
void		Demo::showObject(f3d::tree::Object* object) 
{
	object->translate(glm::vec3(0.0, 100000.0, 0.0));
}

void		Demo::hideTextureData()
{
	for (auto it = _texture_mesh.begin(); it != _texture_mesh.end(); ++it) {
		hideObject(*it);
	}
	_texture_material_it = _texture_material.begin();
}
void		Demo::showTextureData() {
	for (auto it = _texture_mesh.begin(); it != _texture_mesh.end(); ++it) {
		showObject(*it);
	}
	_texture_material_it = _texture_material.begin();
}

void		Demo::cycleTextureData()
{
	if (_texture_material_it != _texture_material.end()) {
		_texture_material_it++;
		if (_texture_material_it == _texture_material.end())
			_texture_material_it = _texture_material.begin();
		for (auto it = _texture_mesh.begin(); it != _texture_mesh.end(); ++it) {
			for (auto m = (*it)->getRoot()->getMeshes().begin(); m != (*it)->getRoot()->getMeshes().end(); ++m) {
				(*m)->setMaterialName((*_texture_material_it)->getName());
				std::cout << "root" << (*m)->getMaterialName() << std::endl;
			}
			for (auto c = (*it)->getRoot()->getChildren().begin(); c != (*it)->getRoot()->getChildren().end(); ++c) {
				for (auto m = (*c)->getMeshes().begin(); m != (*c)->getMeshes().end(); ++m) {
					(*m)->setMaterialName((*_texture_material_it)->getName());
					std::cout << "child" << (*m)->getMaterialName() << std::endl;
				}
			}
		}
	}
}

void		Demo::hideSequenceData()
{
	for (auto it = _sequence_mesh.begin(); it != _sequence_mesh.end(); ++it) {
		hideObject(*it);
	}
	if (_sequence_it != _sequence_frame.end())
		hideObject(*_sequence_it);
	_sequence_it = _sequence_frame.begin();
}
void		Demo::showSequenceData()
{
	for (auto it = _sequence_mesh.begin(); it != _sequence_mesh.end(); ++it) {
		showObject(*it);
	}
	_sequence_it = _sequence_frame.begin();
	if (_sequence_it != _sequence_frame.end())
		showObject(*_sequence_it);
}

void		Demo::cycleSequenceData()
{
	if (_sequence_it != _sequence_frame.end()) {
		hideObject(*_sequence_it);
		_sequence_it++;
		if (_sequence_it == _sequence_frame.end())
			_sequence_it = _sequence_frame.begin();
		showObject(*_sequence_it);
	}
}

void		Demo::onButtonB()
{
	cycleSequenceData();
}

void		Demo::onButtonX()
{
	std::cout << "X" << std::endl;
	cycleTextureData();
}

void		Demo::onButtonA()
{
	_demo_index++;
	if (_demo_index % 2 == 0) //Demo texure
	{
		_sequence_timer.stop();
		hideSequenceData();
		showTextureData();
	}
	else { //Demo sequence
		hideTextureData();
		showSequenceData();
		_sequence_timer.restart();
	}
}

void		Demo::updateScene(f3d::Firestone& f3d, void * arg)
{

	(void)arg;

	//Camera controls management
	fly_camera.updateFromKeyboard(keyboard);
	fly_camera.updateFromXbox360(joystick);

	//Switch between demos
	if (joystick.isButtonPressed(joystick.BUTTON_A))
		_btn_a = true;
	if (joystick.isButtonReleased(joystick.BUTTON_A) && _btn_a == true) {
		onButtonA();
		_btn_a = false;
	}

	//Sequence controls management
	if (joystick.isButtonPressed(joystick.BUTTON_B))
		_btn_b = true;
	if (joystick.isButtonReleased(joystick.BUTTON_B) && _btn_b == true) {
		onButtonB();
		_btn_b = false;
	}	

	//Texture controls
	if (joystick.isButtonPressed(joystick.BUTTON_X))
		_btn_x = true;
	if (joystick.isButtonReleased(joystick.BUTTON_X) && _btn_x == true) {
		onButtonX();
		_btn_x = false;
	}

	if (_sequence_timer.seconds() >= 1) {
		_sequence_timer.restart();
		cycleSequenceData();
	}

}

void		Demo::keyCallback(f3d::Firestone& f3d, f3d::utils::KeyInput& keyEvent, void *arg)
{
	(void)arg;
	keyboard.updateFromEvent(keyEvent);
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
	(void)arg;
	joystick.updateFromEvent(joystickEvent);
}