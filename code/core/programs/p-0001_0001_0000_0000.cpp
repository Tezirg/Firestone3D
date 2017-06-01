#include "p-0001_0001_0000_0000.h"

namespace f3d {
	namespace core {
		namespace prog {
			Program_0001_0001_0000_0000::Program_0001_0001_0000_0000(VkDevice device) : Program::Program(device, 
				{ F3D_COLOR_UNDEFINED, F3D_TEXTURE_UNDEFINED, F3D_LIGHT_DIRECTIONAL, F3D_SHADING_FLAT }
			) {	}

			Program_0001_0001_0000_0000::~Program_0001_0001_0000_0000() {
				std::cout << "Destructor: " << __FILE__ << std::endl;
			}

			void									Program_0001_0001_0000_0000::initVkPipeline(VkRenderPass& renderpass, uint32_t subpass) 
			{
				VkResult							r;
				VkShaderModule						vert_shader;
				VkShaderModule						frag_shader;
				VkPipelineShaderStageCreateInfo		shaderStages[2];
				VkPipelineCacheCreateInfo			cache_infos;

				initVkLayout();
				initVkPipelineInfos();

				std::memset(&shaderStages, 0, 2 * sizeof(VkPipelineShaderStageCreateInfo));
				F3D_ASSERT(createSpvShader("0001_0001_0000_0000.vert.spv", &vert_shader) != false, "Program_0001_0001_0000_0000 vertex shader");

				shaderStages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
				shaderStages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
				shaderStages[0].module = vert_shader;
				shaderStages[0].pName = "main";

				F3D_ASSERT(createSpvShader("0001_0001_0000_0000.frag.spv", &frag_shader) != false, "Program_0001_0001_0000_0000 fragment shader");

				shaderStages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
				shaderStages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
				shaderStages[1].module = frag_shader;
				shaderStages[1].pName = "main";

				std::memset(&cache_infos, 0, sizeof(cache_infos));
				cache_infos.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;

				r = vkCreatePipelineCache(vk_device, &cache_infos, NULL, &vk_pipeline_cache);
				F3D_ASSERT_VK(r, VK_SUCCESS, "Creating pipeline cache failed");

				_create_infos.stageCount = 2;
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

			void				Program_0001_0001_0000_0000::initVkPipelineInfos() {
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

			void									Program_0001_0001_0000_0000::initVkLayout() 
			{
				VkResult							r;
				VkDescriptorSetLayoutBinding		layout_bindings[4];
				VkDescriptorSetLayoutCreateInfo		desc_layout_info;
				VkPipelineLayoutCreateInfo			pipe_layout_info;


				//layout (set = 0, binding = 0) uniform camera;
				std::memset(&layout_bindings[0], 0, sizeof(VkDescriptorSetLayoutBinding));
				layout_bindings[0].binding = 0;
				layout_bindings[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
				layout_bindings[0].descriptorCount = 1;
				layout_bindings[0].stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
				layout_bindings[0].pImmutableSamplers = NULL;

				//layout (set = 1, binding = 0) uniform light;
				layout_bindings[1].binding = 0;
				layout_bindings[1].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
				layout_bindings[1].descriptorCount = 1;
				layout_bindings[1].stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
				layout_bindings[1].pImmutableSamplers = NULL;

				//layout (set = 1, binding = 1) uniform uint n_light;
				layout_bindings[2].binding = 1;
				layout_bindings[2].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
				layout_bindings[2].descriptorCount = 1;
				layout_bindings[2].stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
				layout_bindings[2].pImmutableSamplers = NULL;

				//layout (set = 5, binding = 0) uniform model;
				layout_bindings[3].binding = 0;
				layout_bindings[3].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
				layout_bindings[3].descriptorCount = 1;
				layout_bindings[3].stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
				layout_bindings[3].pImmutableSamplers = NULL;

				vk_desc_layout = new VkDescriptorSetLayout[3];

				std::memset(&desc_layout_info, 0, sizeof(VkDescriptorSetLayoutCreateInfo));
				desc_layout_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
				desc_layout_info.bindingCount = 1;
				desc_layout_info.pBindings = &layout_bindings[0];
				r = vkCreateDescriptorSetLayout(vk_device,& desc_layout_info, NULL, &vk_desc_layout[0]);
				F3D_ASSERT_VK(r, VK_SUCCESS, "Program_0001_0001_0000_0000 Create descriptor set layout[0] failed");
				
				std::memset(&desc_layout_info, 0, sizeof(VkDescriptorSetLayoutCreateInfo));
				desc_layout_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
				desc_layout_info.bindingCount = 2;
				desc_layout_info.pBindings = &layout_bindings[1];
				r = vkCreateDescriptorSetLayout(vk_device, &desc_layout_info, NULL, &vk_desc_layout[1]);
				F3D_ASSERT_VK(r, VK_SUCCESS, "Program_0001_0001_0000_0000 Create descriptor set layout[1] failed");

				std::memset(&desc_layout_info, 0, sizeof(VkDescriptorSetLayoutCreateInfo));
				desc_layout_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
				desc_layout_info.bindingCount = 1;
				desc_layout_info.pBindings = &layout_bindings[3];
				r = vkCreateDescriptorSetLayout(vk_device, &desc_layout_info, NULL, &vk_desc_layout[2]);
				F3D_ASSERT_VK(r, VK_SUCCESS, "Program_0001_0001_0000_0000 Create descriptor set layout[2] failed");

				std::memset(&pipe_layout_info, 0, sizeof(VkPipelineLayoutCreateInfo));
				pipe_layout_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
				pipe_layout_info.pNext = NULL;
				pipe_layout_info.setLayoutCount = 3;
				pipe_layout_info.pSetLayouts = vk_desc_layout;
				r = vkCreatePipelineLayout(vk_device, &pipe_layout_info, NULL, &vk_pipeline_layout);
				F3D_ASSERT_VK(r, VK_SUCCESS, "Program_0001_0001_0000_0000 Create pipeline layout failed");
			}

			bool							Program_0001_0001_0000_0000::drawToCommandBuffer(VkCommandBuffer& cmd, f3d::tree::Mesh& mesh, f3d::tree::Scene& scene) 
			{
				f3d::tree::SceneImpl&		sc = dynamic_cast<f3d::tree::SceneImpl&>(scene);
				f3d::tree::MeshImpl&		m = dynamic_cast<f3d::tree::MeshImpl&>(mesh);
				f3d::tree::CameraImpl&		cam = dynamic_cast<f3d::tree::CameraImpl&>(*scene.getCamera().get());
				f3d::tree::MaterialImpl*	material = dynamic_cast<f3d::tree::MaterialImpl*>(scene.getMaterialByName(mesh.getMaterialName()));
				VkBuffer					vertex_bufs[2] = { m.getVertexBuffer(), m.getNormalBuffer() };
				VkDeviceSize				vertex_offsets[2] = { 0, 0 };
				VkDescriptorSet				sets[3] = { cam.getDescriptorSet() , m.getDescriptorSet(), material->getDescriptorSet() };

				vkCmdBindDescriptorSets(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, vk_pipeline_layout, 0, 3, sets, 0, nullptr);
				Program::bind(cmd);

				vkCmdBindVertexBuffers(cmd, 0, 2, vertex_bufs, vertex_offsets);
				vkCmdBindIndexBuffer(cmd, m.getIndexBuffer(), 0, VK_INDEX_TYPE_UINT32);
				vkCmdDrawIndexed(cmd, m.numIndices(), 1, 0, 0, 0);

				return true;
			}

		}
	}
}