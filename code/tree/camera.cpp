#include "tree/camera.h"

namespace f3d {
	namespace tree {
		Camera::Camera() : _ai_camera(new aiCamera) {
			applyPreset(F3D_CAMERA_PRESET_DEFAULT);
			setClipPlaneNear(1.0f);
			setClipPlaneFar(500.0f);
			setUpDirection(aiVector3D(0.0f, -1.0f, 0.0f));
			setName(std::string("DefaultCamera"));
			setLookAt(aiVector3D(0.0f, 0.0f, 1.0f));
		}

		Camera::Camera(aiCamera *camera) : _ai_camera(camera) {
		}

		Camera::~Camera() {
		}

		void				Camera::applyPreset(eCameraPresetType preset) {
			float			fov = 0.0;

			setAspect(g_f3dCameraPresets[preset].sensor_width / g_f3dCameraPresets[preset].sensor_height);
			fov = 2.0f * (atanf(g_f3dCameraPresets[preset].sensor_width / (2.0f * g_f3dCameraPresets[preset].focal_len)));
			setHorizontalFOV(fov);
		}

		//Underlying Assimp attributes
		float 				Camera::getAspect() const { return _ai_camera->mAspect; }
		void 				Camera::setAspect(float val) { _ai_camera->mAspect = val; }
		float 				Camera::getClipPlaneFar() const { return _ai_camera->mClipPlaneFar; }
		void 				Camera::setClipPlaneFar(float val) { _ai_camera->mClipPlaneFar = val;  }
		float 				Camera::getClipPlaneNear() const { return _ai_camera->mClipPlaneNear;  }
		void 				Camera::setClipPlaneNear(float val) { _ai_camera->mClipPlaneNear = val; }
		float 				Camera::getHorizontalFOV() const { return _ai_camera->mHorizontalFOV; }
		void 				Camera::setHorizontalFOV(float val) { _ai_camera->mHorizontalFOV = val; }
		const aiVector3D& 	Camera::getLookAt() const { return _ai_camera->mLookAt; }
		void				Camera::setLookAt(const aiVector3D& val) { _ai_camera->mLookAt = val; }
		std::string 		Camera::getName() const { return _ai_camera->mName.data; }
		void				Camera::setName(std::string& val) { _ai_camera->mName.Set(val); }
		const aiVector3D& 	Camera::getPosition() const { return _ai_camera->mPosition;  }
		void				Camera::setPosition(const aiVector3D& val) { _ai_camera->mPosition = val;  }
		const aiVector3D& 	Camera::getUpDirection() { return _ai_camera->mUp; }
		void				Camera::setUpDirection(const aiVector3D& val) { _ai_camera->mUp = val; }
	}
}