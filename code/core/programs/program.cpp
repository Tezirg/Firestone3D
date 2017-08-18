#include "program.h"

namespace f3d {
	namespace core {
		Program::Program(VkDevice device, ProgramMask mask) : vk_device(device), mask(mask) {
		}

		Program::~Program() {
			std::cout << "Destructor: " << __FILE__ << std::endl;
		}

		F3D_Mask			Program::getMask() const
		{
			return mask.mask;
		}

		bool				Program::hasFlag(const f3d::eShaderInterfaceTypeBits& flagBit) const
		{
			return (mask.fields.interfaceMask & flagBit) == flagBit;
		}

		bool				Program::hasFlag(const f3d::eShadingTypeBits& flagBit) const
		{
			return (mask.fields.shadingMask & flagBit) == flagBit;
		}

		void				Program::bind(VkCommandBuffer& cmd) {
			vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, vk_pipeline);
		}

		void				Program::initVkPipelineInfos() {
			std::memset(_dynamicStateEnables, 0, sizeof _dynamicStateEnables);
			std::memset(&_dynamicState, 0, sizeof _dynamicState);
			_dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
			_dynamicState.pDynamicStates = _dynamicStateEnables;

			std::memset(&_create_infos, 0, sizeof(_create_infos));
			_create_infos.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
			_create_infos.layout = vk_pipeline_layout;

			std::memset(&_vi, 0, sizeof(_vi));
			_vi.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

			std::memset(&_ia, 0, sizeof(_ia));
			_ia.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
			_ia.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

			std::memset(&_rs, 0, sizeof(_rs));
			_rs.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
			_rs.polygonMode = VK_POLYGON_MODE_FILL;
			_rs.cullMode = VK_CULL_MODE_BACK_BIT;
			_rs.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
			_rs.depthClampEnable = VK_FALSE;
			_rs.rasterizerDiscardEnable = VK_FALSE;
			_rs.depthBiasEnable = VK_FALSE;
			_rs.lineWidth = 1.0;

			std::memset(&_cb, 0, sizeof(_cb));
			_cb.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
			static VkPipelineColorBlendAttachmentState att_state[1];
			memset(att_state, 0, sizeof(att_state));
			att_state[0].colorWriteMask = 0xf;
			att_state[0].blendEnable = VK_FALSE;
			_cb.attachmentCount = 1;
			_cb.pAttachments = att_state;

			std::memset(&_vp, 0, sizeof(_vp));
			_vp.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
			_vp.viewportCount = 1;
			_dynamicStateEnables[_dynamicState.dynamicStateCount++] = VK_DYNAMIC_STATE_VIEWPORT;
			_vp.scissorCount = 1;
			_dynamicStateEnables[_dynamicState.dynamicStateCount++] = VK_DYNAMIC_STATE_SCISSOR;
			_dynamicState.dynamicStateCount = 2;

			std::memset(&_ds, 0, sizeof(_ds));
			_ds.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
			_ds.depthTestEnable = VK_TRUE;
			_ds.depthWriteEnable = VK_TRUE;
			_ds.depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
			_ds.depthBoundsTestEnable = VK_FALSE;
			_ds.back.failOp = VK_STENCIL_OP_KEEP;
			_ds.back.passOp = VK_STENCIL_OP_KEEP;
			_ds.back.compareOp = VK_COMPARE_OP_ALWAYS;
			_ds.stencilTestEnable = VK_FALSE;
			_ds.front = _ds.back;

			std::memset(&_ms, 0, sizeof(_ms));
			_ms.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
			_ms.pSampleMask = NULL;
			_ms.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
		}

		bool							Program::createSpvShader(const std::string& filename, VkShaderModule *shader) {
			VkShaderModuleCreateInfo	shader_info;
			VkResult					r;
			void						*code = 0;
			int							size = 0;
			size_t						usize = 0;
			std::ifstream				file(filename, std::ifstream::in | std::ifstream::binary);

			if (file.is_open() == false)
				return false;

			file.seekg(0, file.end);
			usize = file.tellg();
			size = file.tellg();
			file.seekg(0, file.beg);
			code = new char[size];
			file.read((char *)code, size);

			std::memset(&shader_info, 0, sizeof(VkShaderModuleCreateInfo));
			shader_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
			shader_info.pCode = (const uint32_t *)code;
			shader_info.codeSize = usize;

			r = vkCreateShaderModule(vk_device, &shader_info, NULL, shader);
			F3D_ASSERT_VK(r, VK_SUCCESS, "Create shader module");

			delete[] code;
			file.close();

			return true;
		}
	}
}