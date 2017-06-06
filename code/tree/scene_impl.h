#pragma once 

#ifndef _F3D_SCENE_IMPL_H
#define _F3D_SCENE_IMPL_H

#include <vector>
#include <stack>
#include <assimp/scene.h>
#include <assimp/mesh.h>
#include <assimp/light.h>
#include <assimp/camera.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <Magick++.h> 

#include "core/platform.h"
#include "core/physical_device.h"
#include "core/device.h"
#include "core/attribute_container.h"
#include "core/descriptor_container.h"
#include "tree/scene.h"
#include "tree/camera_impl.h"
#include "tree/light_impl.h"
#include "tree/mesh_impl.h"
#include "tree/material_impl.h"
#include "tree/texture_impl.h"
#include "utils/assimp_to_f3d.h"

namespace f3d {
	namespace tree {
		class F3D_API SceneImpl : 
			public Scene,
			protected f3d::core::AttributeContainer,
			protected f3d::core::DescriptorContainer {
		public:
			SceneImpl(std::shared_ptr<f3d::core::PhysicalDevice>& physical, std::shared_ptr<f3d::core::Device>& device);
			~SceneImpl();

			void						addLight(f3d::tree::Light* light);

			void						writeDescriptorSet();
			void						writeAttribute();
			VkDescriptorSet				getWorldDescriptorSet();
			VkDescriptorSet				getLightsDescriptorSet();

			void						loadFromFile(const std::string& path, const std::string& file);
			void						recursive_uniformUpdate(f3d::tree::Node* f3d_node);
		private:
			void						recurs_aiNodeToF3d(const aiScene* scene, aiNode *ainode, f3d::tree::Node* f3d_node);
			f3d::tree::Material*		_aiMaterialToF3D(const std::string& path, const aiMaterial* aiMat);
			f3d::tree::TextureImpl*		_aiTextureToF3D(const std::string& path, const aiMaterial *aiMaterial, aiTextureType aiType, uint32_t aiIndex);
		private:
			std::stack<glm::mat4>						_matrix;
			std::shared_ptr<f3d::core::PhysicalDevice>	_physical;
			std::shared_ptr<f3d::core::Device>			_device;
		};
	}
}

#endif