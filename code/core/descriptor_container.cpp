#include "core/descriptor_container.h"

namespace f3d {
	namespace core {
		DescriptorContainer::DescriptorContainer(std::shared_ptr< f3d::core::PhysicalDevice >& phys, std::shared_ptr< f3d::core::Device >& dev) :
			_physical(phys), _device(dev) {
			_pool = VK_NULL_HANDLE;
			_dirty_pool = true;
		}

		DescriptorContainer::~DescriptorContainer() {
			cleanContainer();
			std::cout << "Destructor: " << __FILE__ << std::endl;
		}

		void									DescriptorContainer::updateContainer() {
			VkResult							r;
			VkDescriptorPoolCreateInfo			pool_info;
			VkDescriptorSetLayoutCreateInfo		layout_info;
			VkDescriptorSetLayout				layout;
			std::vector<VkDescriptorPoolSize>	poolSizes;
			std::vector<VkDescriptorSetLayoutBinding> pBindings;

			resetContainer();
			//Create the descriptor layout
			for (auto l = _sets_layout.begin(); l != _sets_layout.end(); ++l) {
				//Per descriptor set layout bindings declaration
				auto bind_range = _bindings.equal_range(l->first);
				for (auto b = bind_range.first; b != bind_range.second; ++b) {
					pBindings.push_back(b->second);
					//Since we are here, prepare the poolSizes for later
					VkDescriptorPoolSize	ps;
					ps.type = b->second.descriptorType;
					ps.descriptorCount = b->second.descriptorCount;
					poolSizes.push_back(ps);
				}

				//Create descriptor layout
				std::memset(&layout_info, 0, sizeof(VkDescriptorSetLayoutCreateInfo));
				layout_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
				layout_info.bindingCount = pBindings.size();
				layout_info.pBindings = pBindings.data();
				r = vkCreateDescriptorSetLayout(_device->vk_device, &layout_info, NULL, &layout);
				l->second = layout;
				F3D_ASSERT_VK(r, VK_SUCCESS, "Cannot create descriptor set layout");
				pBindings.clear();
			}

			//Create the pool from layout
			std::memset(&pool_info, 0, sizeof(VkDescriptorPoolCreateInfo));
			pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
			pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
			pool_info.maxSets = _sets.size();
			pool_info.pPoolSizes = poolSizes.data();
			pool_info.poolSizeCount = poolSizes.size();
			r = vkCreateDescriptorPool(_device->vk_device, &pool_info, NULL, &_pool);
			F3D_ASSERT_VK(r, VK_SUCCESS, "Creating descriptor pool failed");
			poolSizes.clear(); //Release tmp pool size infos
			_dirty_pool = false; //Pool is valid from now
		}

		void					DescriptorContainer::cleanContainer() {
			if (_pool != VK_NULL_HANDLE) {
				for (auto d = _sets.begin(); d != _sets.end(); ++d) {
					if (d->second != VK_NULL_HANDLE) {
						vkFreeDescriptorSets(_device->vk_device, _pool, 1, &d->second);
					}
				}
				_sets.clear();
				vkDestroyDescriptorPool(_device->vk_device, _pool, NULL);
				_pool = VK_NULL_HANDLE;
			}
			for (auto l = _sets_layout.begin(); l != _sets_layout.end(); ++l) {
				if (l->second != VK_NULL_HANDLE) {
					vkDestroyDescriptorSetLayout(_device->vk_device, l->second, NULL);
				}
			}
			_sets_layout.clear();
		}

		void					DescriptorContainer::resetContainer() {
			if (_pool != VK_NULL_HANDLE) {
				for (auto d = _sets.begin(); d != _sets.end(); ++d) {
					if (d->second != VK_NULL_HANDLE) {
						vkFreeDescriptorSets(_device->vk_device, _pool, 1, &d->second);
						d->second = VK_NULL_HANDLE;
					}
				}
				vkDestroyDescriptorPool(_device->vk_device, _pool, NULL);
				_pool = VK_NULL_HANDLE;
			}
			for (auto l = _sets_layout.begin(); l != _sets_layout.end(); ++l) {
				if (l->second != VK_NULL_HANDLE) {
					vkDestroyDescriptorSetLayout(_device->vk_device, l->second, NULL);
					l->second = VK_NULL_HANDLE;
				}
			}
		}

		bool	DescriptorContainer::addDescriptor(uint32_t setIndex) {
			_dirty_pool = true;
			_sets[setIndex] = VK_NULL_HANDLE;
			_sets_layout[setIndex] = VK_NULL_HANDLE;
			return true;
		}

		bool	DescriptorContainer::removeDescriptor(uint32_t setIndex) {
			auto it = _sets.find(setIndex);
			if (it != _sets.end() && it->second != VK_NULL_HANDLE) {
				_bindings.erase(setIndex);
				vkFreeDescriptorSets(_device->vk_device, _pool, 1, &(it->second));
				return true;
			}
			return false;
		}

		bool	DescriptorContainer::addDescriptorBinding(uint32_t setIndex, uint32_t bindingIndex, VkDescriptorType type, VkShaderStageFlags stage) {
			VkDescriptorSetLayoutBinding bind;

			std::memset(&bind, 0, sizeof(VkDescriptorSetLayoutBinding));
			bind.descriptorCount = 1;
			bind.pImmutableSamplers = NULL;
			bind.descriptorType = type;
			bind.stageFlags = stage;
			bind.binding = bindingIndex;
			_bindings.emplace(setIndex, bind);
			_dirty_pool = true;
			return true;
		}

		bool	DescriptorContainer::removeDescriptorBinding(uint32_t setIndex, uint32_t bindingIndex) {
			auto range = _bindings.equal_range(setIndex);
			for (auto it = range.first; it != range.second; ++it) {
				if ((*it).second.binding == bindingIndex) {
					_bindings.erase(it);
					_dirty_pool = true;
					return true;
				}
			}
			return false;
		}

		VkDescriptorSetLayout	DescriptorContainer::getDescriptorLayout(uint32_t setIndex) {
			if (_dirty_pool == true)
				updateContainer();
			return _sets_layout[setIndex];
		}

		VkDescriptorSet			DescriptorContainer::getDescriptorSet(uint32_t setIndex) {
			if (_dirty_pool == true)
				updateContainer();

			if (_sets[setIndex] == VK_NULL_HANDLE) {
				VkResult						r;
				VkDescriptorSetAllocateInfo		set_info;
				VkDescriptorSet					set;
				//Allocate descriptor sets from the pool
				std::memset(&set_info, 0, sizeof(VkDescriptorSetAllocateInfo));
				set_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
				set_info.descriptorPool = _pool;
				set_info.descriptorSetCount = 1;
				set_info.pSetLayouts = & (_sets_layout[setIndex]); // Using corresponding layout as previously created
				r = vkAllocateDescriptorSets(_device->vk_device, &set_info, &set);
				_sets[setIndex] = set;
				F3D_ASSERT_VK(r, VK_SUCCESS, "Allocating descriptor set failed");
			}
			return _sets[setIndex];
		}
	}// core::
} // f3d::