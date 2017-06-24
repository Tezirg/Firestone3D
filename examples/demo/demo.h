#pragma once
#ifndef DEMO_H_
#define DEMO_H_

#include <f3d/f3d.h>
#include <f3d/helpers/xbox_360.hpp>

#include <list>
#include <memory>
#include <functional>

class Demo {
public:
	Demo();
	~Demo();

	Demo(const Demo&) = delete; //No copy constructor
	Demo(const Demo&&) = delete;//No move contructor
	Demo& operator=(const Demo&) = delete; //No copy assignement
	Demo& operator=(const Demo&&) = delete; // No move assignement

	bool		init(const std::list<std::string>& texture_mesh, const std::list<std::string>& texture_material, \
					 const std::list<std::string>& sequence_mesh, const std::list<std::string>& sequence_frame);
	bool		run();
	bool		clean();

	void		loadScene(f3d::Firestone& f3d, void * arg);
	void		updateScene(f3d::Firestone& f3d, void * arg);
	void		keyCallback(f3d::Firestone& f3d, f3d::utils::KeyInput& keyEvent, void *arg);
	void		mouseCallback(f3d::Firestone& f3d, f3d::utils::MouseInput& mouseEvent, void *arg);
	void		joystickCallback(f3d::Firestone& f3d, f3d::utils::JoystickInput& joystickEvent, void *arg);
private:
	f3d::helpers::Xbox360Controller			joystick;
	f3d::Firestone*							engine;

	std::list<std::string>						_texture_mesh_str;
	std::list<std::string>						_texture_material_str;
	std::list<std::string>						_sequence_mesh_str;
	std::list<std::string>						_sequence_frame_str;

	float									distance;
	glm::vec3								center;
	float									angleX;
	float									angleY;

	glm::mat4								r;
	glm::mat4								r2;
	glm::mat4								t;
	float									speed;

	bool										_texture_btn;
	std::list<f3d::tree::Object*>				_texture_mesh;
	std::list<f3d::tree::Material*>				_texture_material;
	std::list<f3d::tree::Material*>::iterator	_texture_material_it;

	bool										_sequence_btn;
	bool										_sequence_playing;
	f3d::utils::Timer							_sequence_timer;
	std::list<f3d::tree::Object*>				_sequence_mesh;
	std::list<f3d::tree::Object*>				_sequence_frame;
	std::list<f3d::tree::Object*>::iterator		_sequence_it;
};

#endif