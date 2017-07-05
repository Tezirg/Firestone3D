#ifndef _F3D_HELPER_KEYBOARD_HPP
#define _F3D_HELPER_KEYBOARD_HPP

#include <memory>
#include <functional>
#include <map>
#include <f3d/tree/camera.h>
#include <f3d/utils/inputs.h>


namespace f3d {
	namespace helpers {
		class Keyboard {
		public:
			Keyboard() {

			}

			~Keyboard() {
				_keys.clear();
			}

			Keyboard(const Keyboard& oth) = default; //Use default to copy members
			Keyboard(Keyboard&& mov) = delete; //No move constructor
			Keyboard&	operator=(const Keyboard& oth) = default; //Use default to copy assign members
			Keyboard&	operator=(Keyboard&& mov) = delete; //No move assignment

			void													setKeyState(const char key, f3d::utils::KeyInput::eKeyState state) {
				uint32_t	int_code = static_cast<uint32_t>(key);
				_keys[int_code] = state;
			}

			const f3d::utils::KeyInput::eKeyState					getKeyState(const char key) {
				uint32_t	int_code = static_cast<uint32_t>(key);
				return reinterpret_cast<f3d::utils::KeyInput::eKeyState&>(_keys[int_code]);
			}

			void													setKeyState(const uint32_t key, f3d::utils::KeyInput::eKeyState state) {
				_keys[key] = state;
			}

			const f3d::utils::KeyInput::eKeyState&					getKeyState(const uint32_t key) {
				uint32_t	int_code = static_cast<uint32_t>(key);
				return reinterpret_cast<f3d::utils::KeyInput::eKeyState&>(_keys[int_code]);
			}

			bool													isKeyPressed(const char key) {
				uint32_t	int_code = static_cast<uint32_t>(key);

				if (_keys[int_code] == f3d::utils::KeyInput::eKeyState::F3D_KEY_STATE_PRESS ||
					_keys[int_code] == f3d::utils::KeyInput::eKeyState::F3D_KEY_STATE_REPEAT) {
					return true;
				}
				return false;
			}

			bool													isKeyPressed(const uint32_t key) {
				uint32_t	int_code = static_cast<uint32_t>(key);

				if (_keys[int_code] == f3d::utils::KeyInput::eKeyState::F3D_KEY_STATE_PRESS ||
					_keys[int_code] == f3d::utils::KeyInput::eKeyState::F3D_KEY_STATE_REPEAT) {
					return true;
				}
				return false;
			}

			void													setModifier(const f3d::utils::KeyInput::keyModiferFlags mod) {
				_modifier = mod;
			}

			const f3d::utils::KeyInput::keyModiferFlags&			getModififer() const {
				return _modifier;
			}

			void													updateFromEvent(const f3d::utils::KeyInput& e) {
				_keys[e.keycode] = e.state;
				_modifier = e.mods;
			}

		private:
			std::map<uint32_t, uint32_t>						_keys;
			f3d::utils::KeyInput::keyModiferFlags				_modifier;
		};
	}// helpers::
}// f3d::

#endif