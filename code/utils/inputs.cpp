#include "utils/inputs.h"

namespace f3d {
	namespace utils {
		KeyInput::KeyInput() :
			keycode(0),
			state(eKeyState::F3D_KEY_STATE_UNDEFINED),
			mods(eKeyModifiersFlagsBits::F3D_KEY_MOD_UNDEFINED)
		{
		}

		KeyInput::KeyInput(uint32_t keycode, eKeyState state, keyModiferFlags flags) :
			keycode(keycode), 
			state(state), 
			mods(flags) 
		{
		}

		KeyInput::~KeyInput() {}

		std::ostream&		operator<<(std::ostream& os, const KeyInput& keyEvent) {
			os	<< "F3D KeyInput: "
				<< "keycode[" << keyEvent.keycode << "] "
				<< "pressed[" << (keyEvent.state == keyEvent.F3D_KEY_STATE_PRESS ? "yes" : "no") << "] "
				<< "repeat[" << (keyEvent.state == keyEvent.F3D_KEY_STATE_REPEAT ? "yes" : "no") << "] "
				<< "shift[" << (keyEvent.mods & keyEvent.F3D_KEY_MOD_SHIFT ? "yes" : "no") << "] "
				<< "ctrl[" << (keyEvent.mods & keyEvent.F3D_KEY_MOD_CTRL ? "yes" : "no") << "] "
				<< "alt[" << (keyEvent.mods & keyEvent.F3D_KEY_MOD_ALT ? "yes" : "no") << "] "
				<< "super[" << (keyEvent.mods & keyEvent.F3D_KEY_MOD_SUPER ? "yes" : "no") << "] ";
			return os;
		}

		MouseInput::MouseInput(int button, eMouseButtonState state) :
			type(eMouseInputType::F3D_MOUSE_INPUT_BUTTON),
			button(button),
			buttonState(state),
			focus(true),
			xoffset(0.0),
			yoffset(0.0)
		{ }

		MouseInput::MouseInput(bool hasFocus) :
			type(eMouseInputType::F3D_MOUSE_INPUT_FOCUS),
			button(0),
			buttonState(eMouseButtonState::F3D_MOUSE_BUTTON_UNDEFINED),
			focus(hasFocus),
			xoffset(0.0),
			yoffset(0.0)
		{ }

		MouseInput::MouseInput(double offsetX, double offsetY) :
			type(eMouseInputType::F3D_MOUSE_INPUT_CURSOR),
			button(0),
			buttonState(eMouseButtonState::F3D_MOUSE_BUTTON_UNDEFINED),
			focus(true),
			xoffset(offsetX),
			yoffset(offsetY)
		{ }

		MouseInput::MouseInput(double scrollY) :
			type(eMouseInputType::F3D_MOUSE_INPUT_SCROLL),
			button(0),
			buttonState(eMouseButtonState::F3D_MOUSE_BUTTON_UNDEFINED),
			focus(true),
			xoffset(0.0),
			yoffset(scrollY)
		{ }

		MouseInput::~MouseInput() {}

		std::ostream&		operator<<(std::ostream& os, const MouseInput& mouseEvent) {
			os  << "F3D MouseInput: "
				<< "type[" << 
							(mouseEvent.type == mouseEvent.F3D_MOUSE_INPUT_CURSOR ? "cursor" : 
								(mouseEvent.type == mouseEvent.F3D_MOUSE_INPUT_FOCUS ? "focus" : 
									(mouseEvent.type == mouseEvent.F3D_MOUSE_INPUT_BUTTON ? "button" : 
										(mouseEvent.type == mouseEvent.F3D_MOUSE_INPUT_SCROLL ? "scroll" : "undefined")
									)
								)
							) 
				<< "] button[" << mouseEvent.button << "] "
				<< "button state[" <<
							(mouseEvent.buttonState == mouseEvent.F3D_MOUSE_BUTTON_PRESS ? "pressed" :
								(mouseEvent.buttonState == mouseEvent.F3D_MOUSE_BUTTON_RELEASE ? "release" :
									(mouseEvent.buttonState == mouseEvent.F3D_MOUSE_BUTTON_REPEAT ? "repeat" : "undefined")
								)
							)
				<< "] focus[" << (mouseEvent.focus ? "yes" : "no") << "] "
				<< "offset x[" << mouseEvent.xoffset << "]" 
				<< "offset y[" << mouseEvent.yoffset << "]";
			return os;
		}

		JoystickInput::JoystickInput(uint32_t id, uint32_t buttonIndex, eJoystickButtonState buttonState) :
			id(id),
			type(eJoystickInputType::F3D_JOYSTICK_INPUT_BUTTON),
			inputIndex(buttonIndex),
			buttonState(buttonState),
			axisState(0.0f)
		{ }

		JoystickInput::JoystickInput(uint32_t id, uint32_t axisIndex, float axisState) :
			id(id),
			type(eJoystickInputType::F3D_JOYSTICK_INPUT_AXIS),
			inputIndex(axisIndex),
			buttonState(eJoystickButtonState::F3D_JOYSTICK_BUTTON_UNDEFINED),
			axisState(axisState)
		{ }

		JoystickInput::JoystickInput(uint32_t id, bool connected) :
			id(id),
			type(connected ? eJoystickInputType::F3D_JOYSTICK_INPUT_CONNECTED : eJoystickInputType::F3D_JOYSTICK_INPUT_DISCONNECTED),
			inputIndex(0),
			buttonState(eJoystickButtonState::F3D_JOYSTICK_BUTTON_UNDEFINED),
			axisState(0.0f)
		{ }

		JoystickInput::~JoystickInput() {}

		std::ostream&		operator<<(std::ostream& os, const JoystickInput& joystickEvent) {
			os  << "F3D JoystickInput: "
				<< "[" << joystickEvent.id << "]"
				<< "type[" << 
						(joystickEvent.type == joystickEvent.F3D_JOYSTICK_INPUT_AXIS ? "axis" :
							(joystickEvent.type == joystickEvent.F3D_JOYSTICK_INPUT_BUTTON ? "button" :
								(joystickEvent.type == joystickEvent.F3D_JOYSTICK_INPUT_CONNECTED ? "connect" :
									(joystickEvent.type == joystickEvent.F3D_JOYSTICK_INPUT_DISCONNECTED ? "disconnect" : "undefined")
								)
							)
						)
				<< "] button or axis index[" << joystickEvent.inputIndex << "] "
				<< "axis state[" << joystickEvent.axisState << "] "
				<< "button state[" << 
						(joystickEvent.buttonState == joystickEvent.F3D_JOYSTICK_BUTTON_PRESS ? "pressed" : 
							(joystickEvent.buttonState == joystickEvent.F3D_JOYSTICK_BUTTON_RELEASE ? "released" : "undefined")
						)
				<< "]";
			return os;
		}

	} // utils::
} //f3d::