#include "mesh_impl.h"

namespace f3d {
	namespace tree{
		MeshImpl::MeshImpl(std::shared_ptr<f3d::core::PhysicalDevice>& phys, 
							std::shared_ptr<f3d::core::Device>& device) :
			DescriptorContainer::DescriptorContainer(phys, device),
			AttributeContainer::AttributeContainer(phys, device),
			_phys(phys), _device(device) {

			AttributeContainer::addAttribute(0, sizeof(float) * 16, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);
			DescriptorContainer::addDescriptor(5);
			DescriptorContainer::addDescriptorBinding(5, 0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT);


			VkDescriptorBufferInfo	bufinfo;
			bufinfo.buffer = AttributeContainer::getAttributeBuffer(0);
			bufinfo.offset = 0;
			bufinfo.range = VK_WHOLE_SIZE;

			VkWriteDescriptorSet	desc_write;
			std::memset(&desc_write, 0, sizeof(VkWriteDescriptorSet));
			desc_write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			desc_write.descriptorCount = 1;
			desc_write.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			desc_write.dstSet = DescriptorContainer::getDescriptorSet(5);
			desc_write.dstBinding = 0;
			desc_write.pBufferInfo = &bufinfo;
			vkUpdateDescriptorSets(_device->vk_device, 1, &desc_write, 0, NULL);
		}


		MeshImpl::~MeshImpl() {
			std::cout << "Destructor: " << __FILE__ << std::endl;
		}
		
		VkDescriptorSet			MeshImpl::getDescriptorSet() {
			return DescriptorContainer::getDescriptorSet(5);
		}

		bool					MeshImpl::updateDescriptorSet(glm::mat4& model) {
			return AttributeContainer::updateAttribute(0, glm::value_ptr(model), 0, sizeof(float) * 16);
		}

		VkBuffer				MeshImpl::getVertexBuffer() { return AttributeContainer::getAttributeBuffer(1);  }
		VkBuffer				MeshImpl::getNormalBuffer() { return AttributeContainer::getAttributeBuffer(2);  }
		VkBuffer				MeshImpl::getIndexBuffer() { return AttributeContainer::getAttributeBuffer(4); }
		VkBuffer				MeshImpl::getUvBuffer() { return AttributeContainer::getAttributeBuffer(3); }

		bool			MeshImpl::makeRenderReady() {
			AttributeContainer::addAttribute(1, _vertices.size() * sizeof(float), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
			AttributeContainer::updateAttribute(1, _vertices.data(), 0, _vertices.size() * sizeof(float));
			_vertices.clear();

			AttributeContainer::addAttribute(2, _normals.size() * sizeof(float), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
			AttributeContainer::updateAttribute(2, _normals.data(), 0, _normals.size() * sizeof(float));
			_normals.clear();

			if (_uvs.empty() == false) {
				AttributeContainer::addAttribute(3, _uvs.size() * sizeof(float), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
				AttributeContainer::updateAttribute(3, _uvs.data(), 0, _uvs.size() * sizeof(float));
				_uvs.clear();
			}

			AttributeContainer::addAttribute(4, _indices.size() * sizeof(uint32_t), VK_BUFFER_USAGE_INDEX_BUFFER_BIT);
			AttributeContainer::updateAttribute(4, _indices.data(), 0, _indices.size() * sizeof(uint32_t));
			_indices.clear();

			_ready = true;
			return true;
		}
	}
}