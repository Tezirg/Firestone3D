#include "prog_flat.h"

namespace f3d {
	namespace core {
		namespace prog {
			FlatProgram::FlatProgram(VkDevice device) : Program::Program(device) {
			}

			FlatProgram::~FlatProgram() {
			}

			void									FlatProgram::initVkPipeline(VkRenderPass& renderpass, uint32_t subpass) {
				VkResult							r;
				VkShaderModule						vert_shader;
				VkShaderModule						frag_shader;
				VkPipelineShaderStageCreateInfo		shaderStages[2];
				VkPipelineCacheCreateInfo			cache_infos;

				initVkLayout();
				initVkPipelineInfos();
				initVkDecriptorPool();

				_create_infos.stageCount = 2;

				std::memset(&shaderStages, 0, 2 * sizeof(VkPipelineShaderStageCreateInfo));

				F3D_ASSERT(createSpvShader(flat_vert_spv, &vert_shader) != false, "Flat program vertex shader");

				shaderStages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
				shaderStages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
				shaderStages[0].module = vert_shader;
				shaderStages[0].pName = "main";

				F3D_ASSERT(createSpvShader(flat_frag_spv, &frag_shader) != false, "Flat program fragment shader");

				shaderStages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
				shaderStages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
				shaderStages[1].module = frag_shader;
				shaderStages[1].pName = "main";

				std::memset(&cache_infos, 0, sizeof(cache_infos));
				cache_infos.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;

				r = vkCreatePipelineCache(vk_device, &cache_infos, NULL, &vk_pipeline_cache);
				F3D_ASSERT_VK(r, VK_SUCCESS, "Creating pipeline cache failed");

				_create_infos.pVertexInputState = &_vi;
				_create_infos.pInputAssemblyState = &_ia;
				_create_infos.pRasterizationState = &_rs;
				_create_infos.pColorBlendState = &_cb;
				_create_infos.pMultisampleState = &_ms;
				_create_infos.pViewportState = &_vp;
				_create_infos.pDepthStencilState = &_ds;
				_create_infos.pStages = shaderStages;
				_create_infos.renderPass = renderpass;
				_create_infos.pDynamicState = &_dynamicState;

				r = vkCreateGraphicsPipelines(vk_device, vk_pipeline_cache, 1, &_create_infos, NULL, &vk_pipeline);
				F3D_ASSERT_VK(r, VK_SUCCESS, "Creating graphics pipeline failed");

				vkDestroyShaderModule(vk_device, vert_shader, NULL);
				vkDestroyShaderModule(vk_device, frag_shader, NULL);
			}

			void				FlatProgram::initVkPipelineInfos() {
				Program::initVkPipelineInfos();

				//layout (location = 0) in vec4 position
				std::memset(&_vi_bind, 0, sizeof(_vi_bind));
				_vi_bind[0].binding = 0;
				_vi_bind[0].stride = sizeof(float) * 4;
				_vi_bind[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
				std::memset(&_vi_attr, 0, sizeof(VkVertexInputAttributeDescription));
				_vi_attr[0].format = VkFormat::VK_FORMAT_R32G32B32A32_SFLOAT;
				_vi_attr[0].binding = 0;
				_vi_attr[0].location = 0;
				_vi_attr[0].offset = 0;

				//layout (location = 1) in vec4 normal
				_vi_bind[1].binding = 1;
				_vi_bind[1].stride = sizeof(float) * 4;
				_vi_bind[1].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
				_vi_attr[1].format = VkFormat::VK_FORMAT_R32G32B32A32_SFLOAT;
				_vi_attr[1].binding = 1;
				_vi_attr[1].location = 1;
				_vi_attr[1].offset = 0;

				//Setup vkGraphicsPipelineCreateInfos.vertexInputStateCreateInfos struct
				std::memset(&_vi, 0, sizeof(_vi));
				_vi.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
				_vi.vertexAttributeDescriptionCount = 2;
				_vi.pVertexAttributeDescriptions = _vi_attr;
				_vi.vertexBindingDescriptionCount = 2;
				_vi.pVertexBindingDescriptions = _vi_bind;
			}

			void									FlatProgram::initVkLayout() {
				VkResult							r;
				VkDescriptorSetLayoutBinding		layout_bindings[2];
				VkDescriptorSetLayoutCreateInfo		desc_layout_info;
				VkPipelineLayoutCreateInfo			pipe_layout_info;


				std::memset(layout_bindings, 0, sizeof(VkDescriptorSetLayoutBinding) * 1);
				layout_bindings[0].binding = 0;
				layout_bindings[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
				layout_bindings[0].descriptorCount = 1;
				layout_bindings[0].stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
				layout_bindings[0].pImmutableSamplers = NULL;


				std::memset(&desc_layout_info, 0, sizeof(VkDescriptorSetLayoutCreateInfo));
				desc_layout_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
				desc_layout_info.bindingCount = 1;
				desc_layout_info.pBindings = layout_bindings;
				r = vkCreateDescriptorSetLayout(vk_device, &desc_layout_info, NULL, &vk_desc_layout);
				F3D_ASSERT_VK(r, VK_SUCCESS, "FlatProgram Create descriptor set layout failed");

				std::memset(&pipe_layout_info, 0, sizeof(VkPipelineLayoutCreateInfo));
				pipe_layout_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
				pipe_layout_info.pNext = NULL;
				pipe_layout_info.setLayoutCount = 1;
				pipe_layout_info.pSetLayouts = &vk_desc_layout;
				r = vkCreatePipelineLayout(vk_device, &pipe_layout_info, NULL, &vk_pipeline_layout);
				F3D_ASSERT_VK(r, VK_SUCCESS, "FlatProgram Create pipeline layout failed");
			}

			void							FlatProgram::initVkDecriptorPool() {
				VkResult					r;
				VkDescriptorPoolSize		pool_types[1];
				VkDescriptorPoolCreateInfo	pool_info;

				pool_types[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
				pool_types[0].descriptorCount = 1;

				std::memset(&pool_info, 0, sizeof(VkDescriptorPoolCreateInfo));
				pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
				pool_info.poolSizeCount = 1;
				pool_info.maxSets = 1;
				pool_info.pPoolSizes = pool_types;
				r = vkCreateDescriptorPool(vk_device, &pool_info, NULL, &vk_desc_pool);
				F3D_ASSERT_VK(r, VK_SUCCESS, "FlatProgram Descriptor pool creation failed");
			}
		}
	}
}