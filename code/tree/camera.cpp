#include "tree/camera.h"

namespace f3d {
	namespace tree {
		Camera::Camera() {
		}

		Camera::~Camera() {
		}

		void				Camera::applyPreset(eCameraPresetType preset) {
			float			fov = 0.0;

			setAspect(g_f3dCameraPresets[preset].sensor_width / g_f3dCameraPresets[preset].sensor_height);
			fov = 2.0f * (atanf(g_f3dCameraPresets[preset].sensor_width / (2.0f * g_f3dCameraPresets[preset].focal_len)));
			setHorizontalFOV(fov);
		}
	}
}