#include "tree/camera.h"

namespace f3d {
	namespace tree {
		Camera::Camera() {
		}

		Camera::~Camera() {
		}


		glm::mat4&					Camera::getView() {
			return _view;
		}
		const glm::mat4&			Camera::getView() const {
			return _view;
		}
		void						Camera::setView(const glm::mat4& view) {
			_view = view;
			_VP = _perspective * _view;
		}
		void						Camera::lookAt(const glm::vec3& eye, const glm::vec3& center, const glm::vec3& up) {
			_view = glm::lookAt(eye, center, up);
			_VP = _perspective * _view;
		}


		glm::mat4&					Camera::getPerspective() {
			return _perspective;
		}
		const glm::mat4&			Camera::getPerspective() const {
			return _perspective;
		}
		void						Camera::setPerspective(const glm::mat4& perspective) {
			_perspective = perspective;
			_VP = _perspective * _view;
		}
		void						Camera::setPerspective(float fov, float aspect, float nearPlane, float farPlane) {
			_fov = fov;
			_aspect = aspect;
			_near = nearPlane;
			_far = farPlane;
			_perspective = glm::perspective(_fov, _aspect, _near, _far);
			_VP = _perspective * _view;
		}


		glm::mat4&					Camera::getViewPerspective() {
			return _VP;
		}
		const glm::mat4&			Camera::getViewPerspective() const {
			return _VP;
		}

		void				Camera::applyPreset(eCameraPresetType preset) {
			float			fov = 0.0;

			setAspect(g_f3dCameraPresets[preset].sensor_width / g_f3dCameraPresets[preset].sensor_height);
			fov = 2.0f * (atanf(g_f3dCameraPresets[preset].sensor_width / (2.0f * g_f3dCameraPresets[preset].focal_len)));
			setFOV(fov);
		}

		float 				Camera::getAspect() const {
			return _aspect;
		}
		void 				Camera::setAspect(float val) {
			_aspect = val;
			_perspective = glm::perspective(_fov, _aspect, _near, _far);
			_VP = _perspective * _view;
		}

		float 				Camera::getClipPlaneFar() const {
			return _far;
		}

		void 				Camera::setClipPlaneFar(float val) {
			_far = val;
			_perspective = glm::perspective(_fov, _aspect, _near, _far);
			_VP = _perspective * _view;
		}

		float 				Camera::getClipPlaneNear() const {
			return _near;
		}
		void 				Camera::setClipPlaneNear(float val) {
			_near = val;
			_perspective = glm::perspective(_fov, _aspect, _near, _far);
			_VP = _perspective * _view;
		}
		float 				Camera::getFOV() const {
			return _fov;
		}
		void 				Camera::setFOV(float val) {
			_fov = val;
			_perspective = glm::perspective(_fov, _aspect, _near, _far);
			_VP = _perspective * _view;
		}

		std::string 		Camera::getName() const {
			return _name;
		}
		void				Camera::setName(std::string& val) {
			_name = val;
		}

		glm::vec3		 	Camera::getPosition() const {
			return glm::vec3(_view[3][0], _view[3][1], _view[3][2]);
		}
		void				Camera::setPosition(const glm::vec3& val) {
			_view[3][0] = val.x;
			_view[3][1] = val.y;
			_view[3][2] = val.z;
			_VP = _perspective * _view;
		}
	}
}