#ifndef _F3D_HELPER_MOUSE_HPP
#define _F3D_HELPER_MOUSE_HPP

#include <memory>
#include <functional>
#include <map>
#include <f3d/utils/inputs.h>


namespace f3d {
	namespace helpers {
		class Mouse {
		public:
			Mouse() : 
				_focus(true), _posX(0.0), _posY(0.0), _deltaX(0.0), _deltaY(0.0), _scroll(0.0)
			{
			}

			~Mouse()
			{
				_buttons.clear();
			}

			Mouse(const Mouse& oth) = default; //Use default to copy members
			Mouse(Mouse&& mov) = delete; //No move constructor
			Mouse&	operator=(const Mouse& oth) = default; //Use default to copy assign members
			Mouse&	operator=(Mouse&& mov) = delete; //No move assignment

			void													setButtonState(const uint32_t button, f3d::utils::MouseInput::eMouseButtonState state) 
			{
				_buttons[button] = state;
			}

			const f3d::utils::MouseInput::eMouseButtonState&		getButtonState(const uint32_t button) 
			{
				return _buttons[button];
			}

			bool													isButtonPressed(const uint32_t button) 
			{
				if (_buttons[button] == f3d::utils::MouseInput::eMouseButtonState::F3D_MOUSE_BUTTON_PRESS ||
					_buttons[button] == f3d::utils::MouseInput::eMouseButtonState::F3D_MOUSE_BUTTON_REPEAT) {
					return true;
				}
				return false;
			}

			void													setFocus(bool hasFocus) 
			{
				_focus = hasFocus;
			}

			bool													getFocus() const
			{
				return _focus;
			}

			void													setPosition(double x, double y)
			{
				_deltaX = x - _posX;
				_deltaY = y - _posY;

				_posX = x;
				_posY = y;
			}

			double													getPositionX() const
			{
				return _posX;
			}

			double													getPositionY() const
			{
				return _posY;
			}

			double													getDeltaX() const
			{
				return _deltaX;
			}

			double													getDeltaY() const
			{
				return _deltaY;
			}

			void													setScroll(double value)
			{
				_scroll += value;
			}

			double													getScroll() const
			{
				return _scroll;
			}

			f3d::utils::MouseInput::eMouseInputType					getLastEventType() const
			{
				return _last_event_type;
			}

			void													updateFromEvent(const f3d::utils::MouseInput& e) {
				_last_event_type = e.type;
				switch (e.type) 
				{
				case f3d::utils::MouseInput::eMouseInputType::F3D_MOUSE_INPUT_BUTTON:
					setButtonState(e.button, e.buttonState);
					break;
				case f3d::utils::MouseInput::eMouseInputType::F3D_MOUSE_INPUT_FOCUS:
					setFocus(e.focus);
					break;
				case f3d::utils::MouseInput::eMouseInputType::F3D_MOUSE_INPUT_CURSOR:
					setPosition(e.xoffset, e.yoffset);
					break;
				case f3d::utils::MouseInput::eMouseInputType::F3D_MOUSE_INPUT_SCROLL:
					setScroll(e.yoffset);
					break;
				default:
					_last_event_type = f3d::utils::MouseInput::eMouseInputType::F3D_MOUSE_INPUT_UNDEFINED;
					break;
				}
			}

		private:
			f3d::utils::MouseInput::eMouseInputType						_last_event_type;
			std::map<int, f3d::utils::MouseInput::eMouseButtonState >	_buttons;
			bool														_focus;
			double														_posX;
			double														_posY;
			double														_deltaX;
			double														_deltaY;
			double														_scroll;
		};
	}// helpers::
}// f3d::

#endif