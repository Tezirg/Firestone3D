#ifndef _F3D_HELPER_FLY_CAMERA_HPP
#define _F3D_HELPER_FLY_CAMERA_HPP

#include <memory>
#include <functional>
#include <map>
#include <f3d/core/platform.h> //to get mat4
#include <f3d/tree/camera.h>
#include <f3d/utils/inputs.h>
#include <f3d/helpers/keyboard.hpp>
//#include <f3d/helpers/mouse.hpp>
#include <f3d/helpers/xbox_360.hpp>


namespace f3d {
	namespace helpers {
		class FlyCamera {
		private:
			const uint32_t		LEFT_ARROW = 331;
			const uint32_t		RIGHT_ARROW = 333;
			const uint32_t		UP_ARROW = 328;
			const uint32_t		DOWN_ARROW = 336;
		public:
			FlyCamera() :
				_camera(nullptr), _speed(1.0), _fov_tweak(false) {
			}

			~FlyCamera() {

			}

			FlyCamera(const FlyCamera& oth) = delete; //No copy constructor
			FlyCamera(FlyCamera&& mov) = delete; // No move constructor
			FlyCamera&	operator=(const FlyCamera& oth) = delete; //No copy assignement
			FlyCamera&	operator=(FlyCamera&& mov) = delete; //No mov assignement

			void		attachCamera(std::shared_ptr<f3d::tree::Camera>& camera) {
				_camera = camera;
				//Default perspective
				_camera->setPerspective(30.0f, 1280.0f / 720.0f, 0.1f, 2048.0f);
				//Default the camera to look a center
				_camera->lookAt(glm::vec3(0.0f, 100.0f, -500.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
			}

			//void		updateFromMouse(f3d::helpers::Mouse& mouse);
			void		updateFromKeyboard(f3d::helpers::Keyboard& keyboard) {
				float	z = 0.0f;
				float	 ay = 0.0f;
				glm::mat4	r;
				glm::mat4	t;

				if (keyboard.isKeyPressed(LEFT_ARROW)) //Left arrow
					ay += -0.1f;
				if (keyboard.isKeyPressed(RIGHT_ARROW)) //Right arrow
					ay += 0.1f;
				if (keyboard.isKeyPressed(UP_ARROW)) //Top arrow
					z = 2.0f * _speed;
				if (keyboard.isKeyPressed(DOWN_ARROW)) {//Down arrow
					z = -2.0f * _speed;
				}

				r = glm::rotate(ay, glm::vec3(0.0, -1.0, 0.0));
				t = glm::translate(glm::mat4(), glm::vec3(0.0f, 0.0f, z));
				_camera->setView(t * r * _camera->getView());
			}

			void						updateFromXbox360(f3d::helpers::Xbox360Controller& joystick) {
				glm::mat4				r;
				glm::mat4				r2;
				glm::mat4				t;
				float					x = 0.0f;
				float					y = 0.0f;
				float					z = 0.0f;
				float					ax = 0.0f;
				float					ay = 0.0f;

				//Update fov
				if (joystick.isButtonPressed(joystick.BUTTON_BACK)) {
					_fov_tweak = !_fov_tweak;
					if (_fov_tweak) {
						_camera->setPerspective(30.0f, 1280.0f / 720.0f, 0.1f, 2048.0f);
					}
					else {
						_camera->setPerspective2(156.5f, 30720.0f / 4320.0f, 0.1f, 2048.0f);
					}
				}

				//Update speed
				if (joystick.isButtonPressed(joystick.BUTTON_LB))
					_speed *= 1.1f;
				if (joystick.isButtonPressed(joystick.BUTTON_RB))
					_speed *= 0.92f;


				z += joystick.axisState(joystick.AXIS_LS_Y) * 2.0f * _speed;
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
				_camera->setView(t * r2 * r * _camera->getView());
				//_camera->lookAt(_camera->getPosition(), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
			}
	
		private:
			std::shared_ptr<f3d::tree::Camera>	_camera;
			float								_speed;
			bool								_fov_tweak;
		};
	}// helpers::
}// f3d::

#endif