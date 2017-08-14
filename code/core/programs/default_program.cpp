#include "default_program.h"

namespace f3d {
	namespace core {
		namespace prog {
			DefaultProgram::DefaultProgram(std::shared_ptr< f3d::core::PhysicalDevice >& phys, std::shared_ptr< f3d::core::Device >& device) : 
				Program::Program(device->vk_device, { 0, 0 }),
				DescriptorContainer::DescriptorContainer(phys, device) 
			{
			}

			DefaultProgram::DefaultProgram(std::shared_ptr< f3d::core::PhysicalDevice >& phys, std::shared_ptr< f3d::core::Device >& device, ProgramMask mask) :
				Program::Program(device->vk_device, mask),
				DescriptorContainer::DescriptorContainer(phys, device) 
			{
			}

			DefaultProgram::~DefaultProgram()
			{
				std::cout << "Destructor: " << __FILE__ << std::endl;
			}

			void									DefaultProgram::initSetsFromMask()
			{
				vk_desc_layout_vec.clear();

				// Vertex shader uniforms
				if (hasFlag(F3D_SHADER_UNIFORM_CAMERA)) {  // layout (set = 0, binding = 0) uniform camera
					DescriptorContainer::addDescriptor(0);
					DescriptorContainer::addDescriptorBinding(0, 0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT);
					vk_desc_layout_vec.push_back(DescriptorContainer::getDescriptorLayout(0));
				}
				if (hasFlag(F3D_SHADER_UNIFORM_MODEL)) { // layout(std140, set = 1, binding = 0) uniform mesh_s
					DescriptorContainer::addDescriptor(1);
					DescriptorContainer::addDescriptorBinding(1, 0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT);
					vk_desc_layout_vec.push_back(DescriptorContainer::getDescriptorLayout(1));
				}


				// Fragment shader uniforms
				if (hasFlag(F3D_SHADER_UNIFORM_MATERIAL)) { // layout(set = 3, binding = 0) uniform material_s
					DescriptorContainer::addDescriptor(3);
					DescriptorContainer::addDescriptorBinding(3, 0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_FRAGMENT_BIT);
					vk_desc_layout_vec.push_back(DescriptorContainer::getDescriptorLayout(3));
				}
				if (hasFlag(F3D_SHADER_UNIFORM_LIGHT)) {
					// layout(std140, set = 2, binding = 0) buffer light_s
					// layout (set = 2, binding = 1) uniform n_light_s
					DescriptorContainer::addDescriptor(2);
					DescriptorContainer::addDescriptorBinding(2, 0, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, VK_SHADER_STAGE_FRAGMENT_BIT);
					DescriptorContainer::addDescriptorBinding(2, 1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_FRAGMENT_BIT);
					vk_desc_layout_vec.push_back(DescriptorContainer::getDescriptorLayout(2));
				}
				if (hasFlag(F3D_SHADER_UNIFORM_SAMPLER_AMBIENT)) {
					// layout(set = 4, binding = 0) uniform sampler2D ambient_sampler
					DescriptorContainer::addDescriptor(4);
					DescriptorContainer::addDescriptorBinding(4, 0, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT);
					vk_desc_layout_vec.push_back(DescriptorContainer::getDescriptorLayout(4));

				}
				if (hasFlag(F3D_SHADER_UNIFORM_SAMPLER_DIFFUSE)) {
					// layout(set = 5, binding = 0) uniform sampler2D diffuse_sampler
					DescriptorContainer::addDescriptor(5);
					DescriptorContainer::addDescriptorBinding(5, 0, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT);
					vk_desc_layout_vec.push_back(DescriptorContainer::getDescriptorLayout(5));
				}
				if (hasFlag(F3D_SHADER_UNIFORM_SAMPLER_SPECULAR)) {
					// layout(set = 6, binding = 0) uniform sampler2D specular_sampler
					DescriptorContainer::addDescriptor(6);
					DescriptorContainer::addDescriptorBinding(6, 0, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT);
					vk_desc_layout_vec.push_back(DescriptorContainer::getDescriptorLayout(6));
				}
				if (hasFlag(F3D_SHADER_UNIFORM_SAMPLER_EMISSIVE)) {
					// layout(set = 7, binding = 0) uniform sampler2D emmissive_sampler
					DescriptorContainer::addDescriptor(7);
					DescriptorContainer::addDescriptorBinding(7, 0, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT);
					vk_desc_layout_vec.push_back(DescriptorContainer::getDescriptorLayout(7));
				}
				if (hasFlag(F3D_SHADER_UNIFORM_SAMPLER_HEIGHT)) {
					// layout(set = 8, binding = 0) uniform sampler2D height_sampler
					DescriptorContainer::addDescriptor(8);
					DescriptorContainer::addDescriptorBinding(8, 0, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT);
					vk_desc_layout_vec.push_back(DescriptorContainer::getDescriptorLayout(8));
				}
				if (hasFlag(F3D_SHADER_UNIFORM_SAMPLER_NORMALS)) {
					// layout(set = 9, binding = 0) uniform sampler2D normals_sampler
					DescriptorContainer::addDescriptor(9);
					DescriptorContainer::addDescriptorBinding(9, 0, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT);
					vk_desc_layout_vec.push_back(DescriptorContainer::getDescriptorLayout(9));
				}
				if (hasFlag(F3D_SHADER_UNIFORM_RESERVED_1)) {
					// layout(set = 10, binding = 0) uniform sampler2D displacement_sampler
					DescriptorContainer::addDescriptor(10);
					DescriptorContainer::addDescriptorBinding(10, 0, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT);
					vk_desc_layout_vec.push_back(DescriptorContainer::getDescriptorLayout(10));
				}
				if (hasFlag(F3D_SHADER_UNIFORM_RESERVED_2)) {
					// layout(set = 11, binding = 0) uniform sampler2D reflection_sampler
					DescriptorContainer::addDescriptor(11);
					DescriptorContainer::addDescriptorBinding(11, 0, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT);
					vk_desc_layout_vec.push_back(DescriptorContainer::getDescriptorLayout(11));
				}
			}

			void									DefaultProgram::initAttributesFromMask() 
			{
				_vi_attrs.clear();
				_vi_binds.clear();

				// Vertex shader attributes mask
				if (hasFlag(F3D_SHADER_ATTR_POSITION)) {
					// layout(location = 0) in vec4 in_vertex_position_local_space;
					_vi_binds.push_back({ 0, sizeof(float) * 4, VK_VERTEX_INPUT_RATE_VERTEX });
					_vi_attrs.push_back({ 0, 0, VkFormat::VK_FORMAT_R32G32B32A32_SFLOAT, 0 });
				}
				if (hasFlag(F3D_SHADER_ATTR_NORMAL)) {
					// layout(location = 1) in vec4 in_vertex_normal_model_space
					_vi_binds.push_back({ 1, sizeof(float) * 4, VK_VERTEX_INPUT_RATE_VERTEX });
					_vi_attrs.push_back({ 1, 0, VkFormat::VK_FORMAT_R32G32B32A32_SFLOAT, 0 });
				}
				if (hasFlag(F3D_SHADER_ATTR_COLOR)) {
					// layout (location = 2) in vec4 in_vertex_color
					_vi_binds.push_back({ 2, sizeof(float) * 4, VK_VERTEX_INPUT_RATE_VERTEX });
					_vi_attrs.push_back({ 2, 0, VkFormat::VK_FORMAT_R32G32B32A32_SFLOAT, 0 });
				}
				if (hasFlag(F3D_SHADER_ATTR_UV)) {
					// layout(location = 3) in vec2 in_vertex_UV
					_vi_binds.push_back({ 3, sizeof(float) * 2, VK_VERTEX_INPUT_RATE_VERTEX });
					_vi_attrs.push_back({ 3, 0, VkFormat::VK_FORMAT_R32G32_SFLOAT, 0 });
				}
				if (hasFlag(F3D_SHADER_ATTR_RESERVED_1)) {
					// layout(location = 4) in vec4 in_custom_1;
					_vi_binds.push_back({ 4, sizeof(float) * 4, VK_VERTEX_INPUT_RATE_VERTEX });
					_vi_attrs.push_back({ 4, 0, VkFormat::VK_FORMAT_R32G32B32A32_SFLOAT, 0 });
				}
				if (hasFlag(F3D_SHADER_ATTR_RESERVED_2)) {
					// layout(location = 5) in vec4 in_custom_2 
					_vi_binds.push_back({ 5, sizeof(float) * 4, VK_VERTEX_INPUT_RATE_VERTEX });
					_vi_attrs.push_back({ 5, 0, VkFormat::VK_FORMAT_R32G32B32A32_SFLOAT, 0 });
				}
			}
			void									DefaultProgram::bindDescriptorSets() {}
			void									DefaultProgram::bindAttributes() {}

			void									DefaultProgram::initVkPipeline(VkRenderPass& renderpass, uint32_t subpass) 
			{
				VkResult							r;
				VkShaderModule						vert_shader;
				VkShaderModule						frag_shader;
				VkPipelineShaderStageCreateInfo		shaderStages[2];
				VkPipelineCacheCreateInfo			cache_infos;
				std::ostringstream					os;

				//Get shader name from bitmask
				os	<< "0x" 
					<< std::uppercase << std::setfill('0') << std::setw(4) << std::hex << mask.fields.shadingMask 
					<< std::setw(0) << "_" 
					<< std::uppercase << std::setfill('0') << std::setw(4) << mask.fields.interfaceMask;

				std::cout << os.str() << std::endl;


				initVkLayout();
				initVkPipelineInfos();

				std::memset(&shaderStages, 0, 2 * sizeof(VkPipelineShaderStageCreateInfo));
				F3D_ASSERT(createSpvShader("0002_0002_0001_0007.vert.spv", &vert_shader) != false, "DefaultProgram vertex shader");

				shaderStages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
				shaderStages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
				shaderStages[0].module = vert_shader;
				shaderStages[0].pName = "main";

				F3D_ASSERT(createSpvShader("0002_0002_0001_0007.frag.spv", &frag_shader) != false, "DefaultProgram fragment shader");

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

			void				DefaultProgram::initVkPipelineInfos() {
				Program::initVkPipelineInfos();

				initAttributesFromMask();

				//Setup vkGraphicsPipelineCreateInfos.vertexInputStateCreateInfos struct
				std::memset(&_vi, 0, sizeof(_vi));
				_vi.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
				_vi.vertexAttributeDescriptionCount = _vi_attrs.size();
				_vi.pVertexAttributeDescriptions = _vi_attrs.data();
				_vi.vertexBindingDescriptionCount = _vi_binds.size();
				_vi.pVertexBindingDescriptions = _vi_binds.data();
			}

			void									DefaultProgram::initVkLayout() 
			{
				VkResult							r;
				VkPipelineLayoutCreateInfo			pipe_layout_info;

				initSetsFromMask();

				std::memset(&pipe_layout_info, 0, sizeof(VkPipelineLayoutCreateInfo));
				pipe_layout_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
				pipe_layout_info.pNext = NULL;
				pipe_layout_info.setLayoutCount = vk_desc_layout_vec.size();
				pipe_layout_info.pSetLayouts = vk_desc_layout_vec.data();
				r = vkCreatePipelineLayout(vk_device, &pipe_layout_info, NULL, &vk_pipeline_layout);
				F3D_ASSERT_VK(r, VK_SUCCESS, "DefaultProgram Create pipeline layout failed");
			}

			bool							DefaultProgram::drawToCommandBuffer(VkCommandBuffer& cmd, f3d::tree::Mesh& mesh, f3d::tree::Scene& scene) 
			{
				f3d::tree::SceneImpl&		sc = dynamic_cast<f3d::tree::SceneImpl&>(scene);
				f3d::tree::MeshImpl&		m = dynamic_cast<f3d::tree::MeshImpl&>(mesh);
				f3d::tree::CameraImpl*		cam = dynamic_cast<f3d::tree::CameraImpl*>(scene.getCamera().get());
				f3d::tree::MaterialImpl*	material = dynamic_cast<f3d::tree::MaterialImpl*>(scene.getMaterialByName(mesh.getMaterialName()));
				f3d::tree::TextureImpl*		texture = dynamic_cast<f3d::tree::TextureImpl *>(material->getTexture(F3D_TEXTURE_DIFFUSE));
				VkBuffer					vertex_bufs[3] = { m.getVertexBuffer(), m.getNormalBuffer(), m.getUvBuffer() };
				VkDeviceSize				vertex_offsets[3] = { 0, 0, 0 };
				VkDescriptorSet				sets[5] = { sc.getWorldDescriptorSet(), m.getDescriptorSet(), sc.getLightsDescriptorSet(), material->getDescriptorSet(), texture->getDescriptorSet() };

				vkCmdBindDescriptorSets(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, vk_pipeline_layout, 0, 5, sets, 0, nullptr);
				Program::bind(cmd);

				vkCmdBindVertexBuffers(cmd, 0, 3, vertex_bufs, vertex_offsets);
				vkCmdBindIndexBuffer(cmd, m.getIndexBuffer(), 0, VK_INDEX_TYPE_UINT32);
				vkCmdDrawIndexed(cmd, m.numIndices(), 1, 0, 0, 0);

				return true;
			}

		}
	}
}