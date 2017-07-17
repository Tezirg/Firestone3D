#include "tree/camera.h"

namespace f3d {
	namespace tree {
		Camera::Camera() {
		}

		Camera::~Camera() {
		}


		glm::mat4&					Camera::getView() {
			return _buffer._view;
		}
		const glm::mat4&			Camera::getView() const {
			return _buffer._view;
		}
		void						Camera::setView(const glm::mat4& view) {
			_buffer._view = view;
			updateCombined();
		}
		void						Camera::lookAt(const glm::vec3& eye, const glm::vec3& center, const glm::vec3& up) {
			_buffer._view = glm::lookAt(eye, center, up);
			updateCombined();
		}

		glm::mat4&					Camera::getPerspective() {
			return _buffer._perspective;
		}
		const glm::mat4&			Camera::getPerspective() const {
			return _buffer._perspective;
		}
		void						Camera::setPerspective(const glm::mat4& perspective) {
			_buffer._perspective = perspective;
			updateCombined();
		}
		void						Camera::setPerspective(float fov, float aspect, float nearPlane, float farPlane) {
			_fov = fov;
			_aspect = aspect;
			_near = nearPlane;
			_far = farPlane;
			_buffer._perspective = glm::perspective(_fov, _aspect, _near, _far);
			updateCombined();
		}

		void						Camera::setPerspective2(float horizontalFOV, float aspect, float nearPlane, float farPlane) {
			_fov = horizontalFOV;
			_aspect = 1.0f / aspect; //Inverting aspect ratio (w/h becomes h/w)
			_near = nearPlane;
			_far = farPlane;
			_buffer._perspective = glm::perspective(_fov, _aspect, _near, _far);
			
			//Swaping scale on w and h
			float tmp = _buffer._perspective[0][0];
			_buffer._perspective[0][0] = _buffer._perspective[1][1];
			_buffer._perspective[1][1] = tmp;

			updateCombined();
		}

		glm::mat4&					Camera::getViewPerspective() {
			return _buffer._VP;
		}
		const glm::mat4&			Camera::getViewPerspective() const {
			return _buffer._VP;
		}

		glm::mat4&					Camera::getNormal() {
			return _buffer._normal;
		}
		const glm::mat4&					Camera::getNormal() const {
			return _buffer._normal;
		}


		void						Camera::applyPreset(eCameraPresetType preset) {
			float					fov = 0.0f;

			setAspect(g_f3dCameraPresets[preset].sensor_width / g_f3dCameraPresets[preset].sensor_height);
			fov = 2.0f * (atanf(g_f3dCameraPresets[preset].sensor_width / (2.0f * g_f3dCameraPresets[preset].focal_len)));
			setFOV(fov);
			setPerspective(_fov, _aspect, 0.1f, 1000.0f);
		}

		float 				Camera::getAspect() const {
			return _aspect;
		}
		void 				Camera::setAspect(float val) {
			_aspect = val;
			_buffer._perspective = glm::perspective(_fov, _aspect, _near, _far);
			updateCombined();
		}

		float 				Camera::getClipPlaneFar() const {
			return _far;
		}

		void 				Camera::setClipPlaneFar(float val) {
			_far = val;
			_buffer._perspective = glm::perspective(_fov, _aspect, _near, _far);
			updateCombined();
		}

		float 				Camera::getClipPlaneNear() const {
			return _near;
		}
		void 				Camera::setClipPlaneNear(float val) {
			_near = val;
			_buffer._perspective = glm::perspective(_fov, _aspect, _near, _far);
			updateCombined();
		}
		float 				Camera::getFOV() const {
			return _fov;
		}
		void 				Camera::setFOV(float val) {
			_fov = val;
			_buffer._perspective = glm::perspective(_fov, _aspect, _near, _far);
			updateCombined();
		}

		std::string 		Camera::getName() const {
			return _name;
		}
		void				Camera::setName(std::string& val) {
			_name = val;
		}

		glm::vec3		 	Camera::getPosition() const {
			return glm::vec3(_buffer._view[3][0], _buffer._view[3][1], _buffer._view[3][2]);
		}
		void				Camera::setPosition(const glm::vec3& val) {
			_buffer._view[3][0] = val.x;
			_buffer._view[3][1] = val.y;
			_buffer._view[3][2] = val.z;
			updateCombined();
		}

		void				Camera::rotate(const glm::vec3& angles) {
			_buffer._view *= glm::rotate(angles.x, glm::vec3(1.0, 0.0, 0.0));
			_buffer._view *= glm::rotate(angles.y, glm::vec3(0.0, 1.0, 0.0));
			_buffer._view *= glm::rotate(angles.z, glm::vec3(0.0, 0.0, 1.0));
			updateCombined();
		}

		void				Camera::updateCombined() {
			_buffer._VP = _buffer._perspective * _buffer._view;
			_buffer._normal = glm::transpose(glm::inverse(_buffer._view));
		}
	}
}