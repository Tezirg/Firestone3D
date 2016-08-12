#include "camera_impl.h"

namespace f3d {
	namespace tree {
		CameraImpl::CameraImpl() : _ai_camera(new aiCamera) {
			applyPreset(F3D_CAMERA_PRESET_DEFAULT);
			setClipPlaneNear(1.0f);
			setClipPlaneFar(500.0f);
			setUpDirection(aiVector3D(0.0f, -1.0f, 0.0f));
			setName(std::string("DefaultCamera"));
			setLookAt(aiVector3D(0.0f, 0.0f, 1.0f));
		}

		CameraImpl::CameraImpl(aiCamera *camera) : _ai_camera(camera) {
		}

		CameraImpl::~CameraImpl() {
		}

		//Underlying Assimp attributes
		float 				CameraImpl::getAspect() const { return _ai_camera->mAspect; }
		void 				CameraImpl::setAspect(float val) { _ai_camera->mAspect = val; }
		float 				CameraImpl::getClipPlaneFar() const { return _ai_camera->mClipPlaneFar; }
		void 				CameraImpl::setClipPlaneFar(float val) { _ai_camera->mClipPlaneFar = val;  }
		float 				CameraImpl::getClipPlaneNear() const { return _ai_camera->mClipPlaneNear;  }
		void 				CameraImpl::setClipPlaneNear(float val) { _ai_camera->mClipPlaneNear = val; }
		float 				CameraImpl::getHorizontalFOV() const { return _ai_camera->mHorizontalFOV; }
		void 				CameraImpl::setHorizontalFOV(float val) { _ai_camera->mHorizontalFOV = val; }
		aiVector3D 			CameraImpl::getLookAt() const { return _ai_camera->mLookAt; }
		void				CameraImpl::setLookAt(const aiVector3D& val) { _ai_camera->mLookAt = val; }
		std::string 		CameraImpl::getName() const { return _ai_camera->mName.data; }
		void				CameraImpl::setName(std::string& val) { _ai_camera->mName.Set(val); }
		aiVector3D 			CameraImpl::getPosition() const { return _ai_camera->mPosition;  }
		void				CameraImpl::setPosition(const aiVector3D& val) { _ai_camera->mPosition = val;  }
		aiVector3D 			CameraImpl::getUpDirection() { return _ai_camera->mUp; }
		void				CameraImpl::setUpDirection(const aiVector3D& val) { _ai_camera->mUp = val; }
	}
}