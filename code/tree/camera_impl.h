#pragma once

#ifndef __F3D_CAMERA_IMPL_H
#define __F3D_CAMERA_IMPL_H

#include <memory>
#include <cstdint>
#include <assimp/camera.h>

#include "f3d.h"

namespace f3d {
	namespace tree {
		
		class CameraImpl : public Camera {
		public:
			CameraImpl();
			CameraImpl(aiCamera *);
			~CameraImpl();

			//Underlying Assimp attributes
			float 				getAspect() const;
			void 				setAspect(float val);
			float 				getClipPlaneFar() const;
			void 				setClipPlaneFar(float val);
			float 				getClipPlaneNear() const;
			void 				setClipPlaneNear(float val);
			float 				getHorizontalFOV() const;
			void 				setHorizontalFOV(float val);
			aiVector3D			getLookAt() const;
			void				setLookAt(const aiVector3D& val);
			std::string 		getName() const;
			void				setName(std::string& val);
			aiVector3D			getPosition() const;
			void				setPosition(const aiVector3D& val);
			aiVector3D			getUpDirection();
			void				setUpDirection(const aiVector3D& val);
		private:
			std::shared_ptr<aiCamera>		_ai_camera;
		};
	}
}

#endif