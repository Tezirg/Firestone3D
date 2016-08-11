#pragma once

#ifndef __F3D_CAMERA_H
#define __F3D_CAMERA_H

#include <memory>
#include <cstdint>

#include <assimp/camera.h>

namespace f3d {
	namespace tree {
		
		enum eCameraPresetType {
			F3D_CAMERA_PRESET_DEFAULT = 0,
			F3D_CAMERA_PRESET_IPHONE4 = 1,
			F3D_CAMERA_PRESET_IPHONE4S = 2,
			F3D_CAMERA_PRESET_IPHONE5 = 3,
			F3D_CAMERA_PRESET_NEXUS5 = 4,
			F3D_CAMERA_PRESET_GOPROHERO3_BLACK = 5,
			F3D_CAMERA_PRESET_GALAXYS3 = 6,
			F3D_CAMERA_PRESET_GALAXYS4 = 7,
			F3D_CAMERA_PRESET_BEGIN_RANGE = F3D_CAMERA_PRESET_DEFAULT,
			F3D_CAMERA_PRESET_END_RANGE = F3D_CAMERA_PRESET_GALAXYS4,
			F3D_CAMERA_PRESET_RANGE_SIZE = (F3D_CAMERA_PRESET_GALAXYS4 - F3D_CAMERA_PRESET_DEFAULT + 1),
			F3D_CAMERA_PRESET_MAX_ENUM = 0x7FFFFFFF
		};
		struct CameraPreset {
			eCameraPresetType	type;
			float				sensor_width; //mm
			float				sensor_height; //mm
			float				focal_len; //mm

			bool				operator==(eCameraPresetType oth_type) { return type == oth_type; }
			bool				operator==(CameraPreset& oth) { return type == oth.type; }
		};
		static struct CameraPreset g_f3dCameraPresets[] = {
			{ F3D_CAMERA_PRESET_DEFAULT, 32.0f, 18.0f, 22.9f },
			{ F3D_CAMERA_PRESET_IPHONE4, 4.54f, 3.42f, 3.85f },
			{ F3D_CAMERA_PRESET_IPHONE4S, 4.54f, 3.42f, 4.28f },
			{ F3D_CAMERA_PRESET_IPHONE5, 4.54f, 3.42f, 4.10f },
			{ F3D_CAMERA_PRESET_NEXUS5, 4.50f, 3.37f, 3.91f },
			{ F3D_CAMERA_PRESET_GOPROHERO3_BLACK, 6.16f, 4.62f, 2.77f },
			{ F3D_CAMERA_PRESET_GALAXYS3, 4.80f, 3.60f, 3.70f },
			{ F3D_CAMERA_PRESET_GALAXYS4, 4.80f, 3.60f, 4.20f }
		};

		class Camera {
		public:
			Camera();
			Camera(aiCamera *);
			~Camera();

			void				applyPreset(eCameraPresetType preset);

			//Underlying Assimp attributes
			float 				getAspect() const;
			void 				setAspect(float val);
			float 				getClipPlaneFar() const;
			void 				setClipPlaneFar(float val);
			float 				getClipPlaneNear() const;
			void 				setClipPlaneNear(float val);
			float 				getHorizontalFOV() const;
			void 				setHorizontalFOV(float val);
			const aiVector3D& 	getLookAt() const;
			void				setLookAt(const aiVector3D& val);
			std::string 		getName() const;
			void				setName(std::string& val);
			const aiVector3D& 	getPosition() const;
			void				setPosition(const aiVector3D& val);
			const aiVector3D& 	getUpDirection();
			void				setUpDirection(const aiVector3D& val);
		private:
			std::shared_ptr<aiCamera>		_ai_camera;
		};
	}
}

#endif