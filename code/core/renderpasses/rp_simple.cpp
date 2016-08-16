#include "rp_simple.h"

namespace f3d {
	namespace core {
		namespace renderpass {

			SimpleRenderPass::SimpleRenderPass(std::shared_ptr<f3d::core::Device>& device, VkFormat color) : 
				RenderPass::RenderPass(F3D_RENDERPASS_SIMPLE), 
				_device(device), _color_format(color),
				_prog(new f3d::core::prog::FlatProgram(device->vk_device)) {

				vk_subpasses_count = 1;
				vk_subpasses = new VkSubpassDescription[1];
				_clear[0].color.float32[0] = 0.2f;
				_clear[0].color.float32[1] = 0.2f;
				_clear[0].color.float32[2] = 0.2f;
				_clear[0].color.float32[3] = 0.2f;
				_clear[1].depthStencil.depth = 1.0f;
				_clear[1].depthStencil.stencil = 0;

				initRenderPass();

				_prog->initVkPipeline(vk_renderpass, 0);
			}
			
			SimpleRenderPass::~SimpleRenderPass() {
			}

			void						SimpleRenderPass::initRenderPass() {
				VkResult				r;
				VkRenderPassCreateInfo	rp_info;

				std::memset(_attachments, 0, sizeof(VkAttachmentDescription) * 2);
				_attachments[0].format = _color_format;
				_attachments[0].samples = VK_SAMPLE_COUNT_1_BIT;
				_attachments[0].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
				_attachments[0].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
				_attachments[0].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
				_attachments[0].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
				_attachments[0].initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
				_attachments[0].finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
				_attachments[1].format = VK_FORMAT_D16_UNORM;
				_attachments[1].samples = VK_SAMPLE_COUNT_1_BIT;
				_attachments[1].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
				_attachments[1].storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
				_attachments[1].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
				_attachments[1].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
				_attachments[1].initialLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
				_attachments[1].finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

				std::memset(&_color_reference, 0, sizeof(VkAttachmentReference));
				_color_reference.attachment = 0;
				_color_reference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

				std::memset(&_depth_reference, 0, sizeof(VkAttachmentReference));
				_depth_reference.attachment = 1;
				_depth_reference.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

				std::memset(vk_subpasses, 0, sizeof(VkSubpassDescription));
				vk_subpasses[0].pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
				vk_subpasses[0].flags = 0;
				vk_subpasses[0].inputAttachmentCount = 0;
				vk_subpasses[0].pInputAttachments = NULL;
				vk_subpasses[0].colorAttachmentCount = 1;
				vk_subpasses[0].pColorAttachments = &_color_reference;
				vk_subpasses[0].pResolveAttachments = NULL;
				vk_subpasses[0].pDepthStencilAttachment = &_depth_reference;
				vk_subpasses[0].preserveAttachmentCount = 0;
				vk_subpasses[0].pPreserveAttachments = NULL;

				std::memset(&rp_info, 0, sizeof(VkRenderPassCreateInfo));
				rp_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
				rp_info.attachmentCount = 2;
				rp_info.pAttachments = _attachments;
				rp_info.subpassCount = vk_subpasses_count;
				rp_info.pSubpasses = vk_subpasses;

				r = vkCreateRenderPass(_device->vk_device, &rp_info, NULL, &vk_renderpass);
				F3D_ASSERT_VK(r, VK_SUCCESS, "Render pass creation failed");
			}

			void		SimpleRenderPass::render(VkCommandBuffer cmd, VkFramebuffer frame, std::shared_ptr< f3d::tree::Scene> scene) {
			}
			
			void		SimpleRenderPass::updateDescriptorSet(f3d::tree::Camera& cam) {
			}
		}
	}
}