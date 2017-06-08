#pragma once

#ifndef __F3D_CAMERA_H
#define __F3D_CAMERA_H

#include "core/platform.h"

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
		struct F3D_API CameraPreset {
			eCameraPresetType	type;
			float				sensor_width; //mm
			float				sensor_height; //mm
			float				focal_len; //mm

			bool				operator==(eCameraPresetType oth_type) { return type == oth_type; }
			bool				operator==(CameraPreset& oth) { return type == oth.type; }
		};
		static struct F3D_API CameraPreset g_f3dCameraPresets[] = {
			{ F3D_CAMERA_PRESET_DEFAULT, 32.0f, 18.0f, 22.9f },
			{ F3D_CAMERA_PRESET_IPHONE4, 4.54f, 3.42f, 3.85f },
			{ F3D_CAMERA_PRESET_IPHONE4S, 4.54f, 3.42f, 4.28f },
			{ F3D_CAMERA_PRESET_IPHONE5, 4.54f, 3.42f, 4.10f },
			{ F3D_CAMERA_PRESET_NEXUS5, 4.50f, 3.37f, 3.91f },
			{ F3D_CAMERA_PRESET_GOPROHERO3_BLACK, 6.16f, 4.62f, 2.77f },
			{ F3D_CAMERA_PRESET_GALAXYS3, 4.80f, 3.60f, 3.70f },
			{ F3D_CAMERA_PRESET_GALAXYS4, 4.80f, 3.60f, 4.20f }
		};

		class F3D_API Camera {
		public:
			Camera();
			virtual ~Camera();

			glm::mat4&					getView();
			const glm::mat4&			getView() const;
			void						setView(const glm::mat4& view);
			void						lookAt(const glm::vec3& eye, const glm::vec3& center, const glm::vec3& up);

			glm::mat4&					getPerspective();
			const glm::mat4&			getPerspective() const;
			void						setPerspective(const glm::mat4& perspective);
			void						setPerspective(float verticalFOV, float aspect, float nearPlane, float farPlane);
			//Perpective with Horizontal field of view
			void						setPerspective2(float horizontalFOV, float aspect, float nearPlane, float farPlane);

			glm::mat4&					getViewPerspective();
			const glm::mat4&			getViewPerspective() const;


			virtual void				applyPreset(eCameraPresetType preset);

			virtual float 				getAspect() const;
			virtual void 				setAspect(float val);
			virtual float 				getClipPlaneFar() const;
			virtual void 				setClipPlaneFar(float val);
			virtual float 				getClipPlaneNear() const;
			virtual void 				setClipPlaneNear(float val);
			virtual float 				getFOV() const;
			virtual void 				setFOV(float val);
			virtual std::string 		getName() const;
			virtual void				setName(std::string& val);
			virtual glm::vec3		 	getPosition() const;
			virtual void				setPosition(const glm::vec3& val);
			void						rotate(const glm::vec3& angles);
		protected:
			std::string					_name;

			glm::mat4					_VP;
			glm::mat4					_view;
			glm::mat4					_perspective;

			float						_far;
			float						_near;
			float						_fov;
			float						_aspect;
		};
	}
}

#endif