#pragma once

#ifndef _F3D_INPUTS_H
#define _F3D_INPUTS_H

#include <iostream>
#include <ostream>
#include <cstdint>

#include "core/platform.h"

namespace f3d {
	namespace utils {
		class F3D_API KeyInput {
		public:
			enum eKeyState {
				F3D_KEY_STATE_UNDEFINED = 0,
				F3D_KEY_STATE_PRESS = 1,
				F3D_KEY_STATE_REPEAT = 2,
				F3D_KEY_STATE_RELEASE = 3,
				F3D_KEY_STATE_BEGIN_RANGE = F3D_KEY_STATE_UNDEFINED,
				F3D_KEY_STATE_END_RANGE = F3D_KEY_STATE_RELEASE,
				F3D_KEY_STATE_RANGE_SIZE = (F3D_KEY_STATE_END_RANGE - F3D_KEY_STATE_BEGIN_RANGE + 1),
				F3D_KEY_STATE_MAX_ENUM = 0x7FFFFFFF
			};
			enum eKeyModifiersFlagsBits {
				F3D_KEY_MOD_UNDEFINED = 0,
				F3D_KEY_MOD_SHIFT = 1,
				F3D_KEY_MOD_CTRL = 2,
				F3D_KEY_MOD_ALT = 3,
				F3D_KEY_MOD_SUPER = 4,
				F3D_KEY_MOD_BEGIN_RANGE = F3D_KEY_MOD_UNDEFINED,
				F3D_KEY_MOD_END_RANGE = F3D_KEY_MOD_SUPER,
				F3D_KEY_MOD_RANGE_SIZE = (F3D_KEY_MOD_END_RANGE - F3D_KEY_MOD_BEGIN_RANGE + 1),
				F3D_KEY_MOD_MAX_ENUM = 0x7FFFFFFF
			};
			typedef uint32_t keyModiferFlags;

			KeyInput(int keycode, eKeyState state, keyModiferFlags flags);
			virtual ~KeyInput();

		public:
			int					keycode;
			eKeyState			state;
			keyModiferFlags		mods;
		};
		F3D_API std::ostream&	operator<<(std::ostream& os, const KeyInput& keyEvent);

		class F3D_API MouseInput {
		public:
			enum eMouseInputType {
				F3D_MOUSE_INPUT_UNDEFINED = 0,
				F3D_MOUSE_INPUT_CURSOR = 1,
				F3D_MOUSE_INPUT_BUTTON = 2,
				F3D_MOUSE_INPUT_SCROLL = 3,
				F3D_MOUSE_INPUT_FOCUS = 4,
				F3D_MOUSE_INPUT_BEGIN_RANGE = F3D_MOUSE_INPUT_UNDEFINED,
				F3D_MOUSE_INPUT_END_RANGE = F3D_MOUSE_INPUT_FOCUS,
				F3D_MOUSE_INPUT_RANGE_SIZE = (F3D_MOUSE_INPUT_END_RANGE - F3D_MOUSE_INPUT_BEGIN_RANGE + 1),
				F3D_MOUSE_INPUT_MAX_ENUM = 0x7FFFFFFF
			};

			enum eMouseButtonState {
				F3D_MOUSE_BUTTON_UNDEFINED = 0,
				F3D_MOUSE_BUTTON_PRESS = 1,
				F3D_MOUSE_BUTTON_RELEASE = 2,
				F3D_MOUSE_BUTTON_REPEAT = 3,
				F3D_MOUSE_BUTTON_BEGIN_RANGE = F3D_MOUSE_BUTTON_UNDEFINED,
				F3D_MOUSE_BUTTON_END_RANGE = F3D_MOUSE_BUTTON_REPEAT,
				F3D_MOUSE_BUTTON_RANGE_SIZE = (F3D_MOUSE_BUTTON_END_RANGE - F3D_MOUSE_BUTTON_BEGIN_RANGE + 1),
				F3D_MOUSE_BUTTON_MAX_ENUM = 0x7FFFFFFF
			};

			MouseInput(int button, eMouseButtonState state);
			MouseInput(bool hasFocus);
			MouseInput(double offsetX, double offsetY);
			MouseInput(double scrollY);
			virtual ~MouseInput();

		public:
			eMouseInputType		type;
			int					button;
			eMouseButtonState	buttonState;
			bool				focus;
			double				xoffset;
			double				yoffset;
		};
		F3D_API std::ostream&	operator<<(std::ostream& os, const MouseInput& mouseEvent);

		class F3D_API JoystickInput {
		public:
			enum eJoystickInputType {
				F3D_JOYSTICK_INPUT_UNDEFINED = 0,
				F3D_JOYSTICK_INPUT_AXIS = 1,
				F3D_JOYSTICK_INPUT_BUTTON = 2,
				F3D_JOYSTICK_INPUT_CONNECTED = 3,
				F3D_JOYSTICK_INPUT_DISCONNECTED = 4,
				F3D_JOYSTICK_INPUT_BEGIN_RANGE = F3D_JOYSTICK_INPUT_UNDEFINED,
				F3D_JOYSTICK_INPUT_END_RANGE = F3D_JOYSTICK_INPUT_DISCONNECTED,
				F3D_JOYSTICK_INPUT_RANGE_SIZE = (F3D_JOYSTICK_INPUT_END_RANGE - F3D_JOYSTICK_INPUT_BEGIN_RANGE + 1),
				F3D_JOYSTICK_INPUT_MAX_ENUM = 0x7FFFFFFF
			};
			enum eJoystickButtonState {
				F3D_JOYSTICK_BUTTON_UNDEFINED = 0,
				F3D_JOYSTICK_BUTTON_PRESS = 1,
				F3D_JOYSTICK_BUTTON_RELEASE = 2,
				F3D_JOYSTICK_BUTTON_BEGIN_RANGE = F3D_JOYSTICK_BUTTON_UNDEFINED,
				F3D_JOYSTICK_BUTTON_END_RANGE = F3D_JOYSTICK_BUTTON_RELEASE,
				F3D_JOYSTICK_BUTTON_RANGE_SIZE = (F3D_JOYSTICK_BUTTON_END_RANGE - F3D_JOYSTICK_BUTTON_BEGIN_RANGE + 1),
				F3D_JOYSTICK_BUTTON_MAX_ENUM = 0x7FFFFFFF
			};

			JoystickInput(uint32_t id, uint32_t buttonIndex, eJoystickButtonState buttonState);
			JoystickInput(uint32_t id, uint32_t axisIndex, float axisState);
			JoystickInput(uint32_t id, bool connected);
			virtual ~JoystickInput();

		public:
			uint32_t				id;
			eJoystickInputType		type;
			uint32_t				inputIndex;
			eJoystickButtonState	buttonState;
			float					axisState;
		};
		F3D_API std::ostream&		operator<<(std::ostream& os, const JoystickInput& joystickEvent);

	} // utils::
} //f3d::

#endif