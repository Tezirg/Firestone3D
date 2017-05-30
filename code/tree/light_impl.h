#pragma once

#ifndef __F3D_LIGHT_IMPL_H
#define __F3D_LIGHT_IMPL_H

#include <memory>
#include <cstdint>
#include <iostream>

#include <assimp/light.h>

#include "core/platform.h"
#include "core/types.h"
#include "core/physical_device.h"
#include "core/device.h"
#include "tree/light.h"

namespace f3d {
	namespace tree {
		class F3D_API LightImpl : public Light {
		public:
			LightImpl(std::shared_ptr< f3d::core::PhysicalDevice >& phys, std::shared_ptr< f3d::core::Device >& device);
			LightImpl(std::shared_ptr< f3d::core::PhysicalDevice >& phys, std::shared_ptr< f3d::core::Device >& device, aiLight *light);
			~LightImpl();

			float					getAngleInnerCone() const;
			void					setAngleInnerCone(float val);
			float					getAngleOuterCone() const;
			void					setAngleOuterCone(float val);
			float					getAttenuationConstant() const;
			void					setAttenuationConstant(float val);
			float					getAttenuationLinear() const;
			void					setAttenuationLinear(float val);
			float					getAttenuationQuadratic() const;
			void					setAttenuationQuadratic(float val);
			glm::vec3				getColorAmbient() const;
			void					setColorAmbient(const glm::vec3& val);
			glm::vec3				getColorDiffuse() const;
			void					setColorDiffuse(const glm::vec3& val);
			glm::vec3				getColorSpecular() const;
			void					setColorSpecular(const glm::vec3& val);
			glm::vec3				getDirection() const;
			void					setDirection(const glm::vec3& val);
			std::string				getName() const;
			void					setName(const std::string& val);
			glm::vec3				getPosition() const;
			void					setPosition(const glm::vec3& val);
			eLightType				getType() const;
			void			 		setType(eLightType val);

			void					updateAttribute();
			void					createAttribute();
			VkDescriptorSet			getDescriptorSet();
			void					updateDescriptorSet();
		private:
			std::shared_ptr<f3d::core::PhysicalDevice>	_physical;
			std::shared_ptr<f3d::core::Device>			_device;

			VkBuffer									_buffer;
			VkDeviceMemory								_memory;
			VkDescriptorSetLayout						_desc_layout;
			VkDescriptorPool							_desc_pool;
			VkDescriptorSet								_descriptor;

			std::string									_name;
			eLightType									_type;
			glm::vec4									_position;
			glm::vec4									_direction;
			float										_inner_cone;
			float										_outer_cone;
			glm::vec4									_ambient_color;
			glm::vec4									_diffuse_color;
			glm::vec4									_specular_color;
			float										_constant;
			float										_linear;
			float										_quadratic;			
		};
	}
}

#endif