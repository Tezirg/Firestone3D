#pragma once
#ifndef DEMO_H_
#define DEMO_H_

#include <f3d/f3d.h>
#include <f3d/helpers/xbox_360.hpp>
#include <f3d/helpers/fly_camera.hpp>
#include <f3d/helpers/keyboard.hpp>

#include <list>
#include <memory>
#include <functional>
#include <iostream>

class Demo {
public:
	Demo();
	~Demo();

	Demo(const Demo&) = delete; //No copy constructor
	Demo(Demo&&) = delete;//No move contructor
	Demo& operator=(const Demo&) = delete; //No copy assignement
	Demo& operator=(Demo&&) = delete; // No move assignement

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
	void		onButtonB();
	void		onButtonX();
	void		onButtonA();
	void		hideObject(f3d::tree::Object *object);
	void		showObject(f3d::tree::Object* object);
	void		hideTextureData();
	void		showTextureData();
	void		cycleTextureData();
	void		hideSequenceData();
	void		showSequenceData();
	void		cycleSequenceData();

private:
	f3d::Firestone*							engine;
	f3d::helpers::FlyCamera					fly_camera;
	f3d::helpers::Keyboard					keyboard;
	f3d::helpers::Xbox360Controller			joystick;

	uint32_t								_demo_index;
	bool									_btn_b;
	bool									_btn_a;
	bool									_btn_x;
	bool									_btn_start;

	std::list<std::string>						_texture_mesh_str;
	std::list<std::string>						_texture_material_str;
	std::list<std::string>						_sequence_mesh_str;
	std::list<std::string>						_sequence_frame_str;

	std::list<f3d::tree::Object*>				_texture_mesh;
	std::list<f3d::tree::Material*>				_texture_material;
	std::list<f3d::tree::Material*>::iterator	_texture_material_it;

	f3d::utils::Timer							_sequence_timer;
	std::list<f3d::tree::Object*>				_sequence_mesh;
	std::list<f3d::tree::Object*>				_sequence_frame;
	std::list<f3d::tree::Object*>::iterator		_sequence_it;
};

#endif