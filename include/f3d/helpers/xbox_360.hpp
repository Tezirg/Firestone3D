#ifndef _F3D_HELPER_XBOX_360_CONTROLLER_HPP
#define _F3D_HELPER_XBOX_360_CONTROLLER_HPP

#include <memory>
#include <map>
#include <f3d/utils/inputs.h>

namespace f3d {
	namespace helpers {
		class Xbox360Controller {
		public:
			enum eButton {
				BUTTON_A = 0,
				BUTTON_B = 1,
				BUTTON_X = 2,
				BUTTON_Y = 3,
				BUTTON_LB = 4,
				BUTTON_RB = 5,
				BUTTON_BACK = 6,
				BUTTON_START = 7,
				BUTTON_LS = 8,
				BUTTON_RS = 9
			};

			enum eAxis {
				AXIS_LS_X = 0,
				AXIS_LS_Y = 1,
				AXIS_RS_X = 2,
				AXIS_RS_Y = 3,
				AXIS_LT = 4,
				AXIS_RT = 5
			};

			Xbox360Controller() {
				_button_state.emplace(BUTTON_A, false);
				_button_state.emplace(BUTTON_B, false);
				_button_state.emplace(BUTTON_X, false);
				_button_state.emplace(BUTTON_Y, false);
				_button_state.emplace(BUTTON_LB, false);
				_button_state.emplace(BUTTON_RB, false);
				_button_state.emplace(BUTTON_BACK, false);
				_button_state.emplace(BUTTON_START, false);
				_button_state.emplace(BUTTON_LS, false);
				_button_state.emplace(BUTTON_RS, false);

				_axis_state.emplace(AXIS_LS_X, 0.0f);
				_axis_state.emplace(AXIS_LS_Y, 0.0f);
				_axis_state.emplace(AXIS_RS_X, 0.0f);
				_axis_state.emplace(AXIS_RS_Y, 0.0f);
				_axis_state.emplace(AXIS_LT, -1.0f);
				_axis_state.emplace(AXIS_RT, -1.0f);
			}
			
			virtual ~Xbox360Controller() {
			}

			Xbox360Controller(const Xbox360Controller& oth_cpy) = default;
			Xbox360Controller(Xbox360Controller&& oth_move) = default;
			Xbox360Controller&	operator=(const Xbox360Controller& oth_assign_cpy) = default;
			Xbox360Controller&	operator=(Xbox360Controller&& oth_assign_move) = default;

			void	update(const f3d::utils::JoystickInput& input) {
				if (input.type == f3d::utils::JoystickInput::eJoystickInputType::F3D_JOYSTICK_INPUT_BUTTON) {
					eButton b = static_cast<eButton>(input.inputIndex);
					_button_state[b] = input.buttonState == f3d::utils::JoystickInput::eJoystickButtonState::F3D_JOYSTICK_BUTTON_PRESS;
				}
				else if (input.type == f3d::utils::JoystickInput::eJoystickInputType::F3D_JOYSTICK_INPUT_AXIS) {
					eAxis a = static_cast<eAxis>(input.inputIndex);
					_axis_state[a] = input.axisState;
				}
				else {
					//Nothing
				}
			}

			bool	isButtonPressed(const eButton button) const {
				return _button_state.at(button);
			}

			bool	isButtonReleased(const eButton button) const {
				return !_button_state.at(button);
			}

			float	axisState(eAxis axis) const {
				return _axis_state.at(axis);
			}

		private:
			std::map<eButton, bool>		_button_state;
			std::map<eAxis, float>		_axis_state;

		};
	}// helpers::
}// f3d::

#endif