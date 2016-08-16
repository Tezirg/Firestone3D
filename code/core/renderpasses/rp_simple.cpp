#include "rp_simple.h"

namespace f3d {
	namespace core {
		namespace renderpass {

			SimpleRenderPass::SimpleRenderPass(std::shared_ptr<f3d::core::Device>& device, 
											   std::shared_ptr<f3d::core::PhysicalDevice>& physical, 
											   std::shared_ptr<f3d::core::Window>& window):
				RenderPass::RenderPass(F3D_RENDERPASS_SIMPLE, device, physical, window), 
				_color_format(VK_FORMAT_B8G8R8A8_UNORM),
				_prog(new f3d::core::prog::FlatProgram(device->vk_device)) {

				WindowImpl *w = dynamic_cast<WindowImpl *>(window.get());

				_color_format = w->vk_format;
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
				
				initDepth();
				//initViews();
				//initFramebuffers();
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

				r = vkCreateRenderPass(device->vk_device, &rp_info, NULL, &vk_renderpass);
				F3D_ASSERT_VK(r, VK_SUCCESS, "Render pass creation failed");
			}

			void						SimpleRenderPass::initDepth() {
				VkResult				r;
				bool					r_bool;
				VkMemoryRequirements	mem_reqs;
				VkImageCreateInfo		image_info;
				VkImageViewCreateInfo	view_info;
				VkMemoryAllocateInfo	mem_info;


				std::memset(&image_info, 0, sizeof(VkImageCreateInfo));
				image_info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
				image_info.pNext = NULL;
				image_info.imageType = VK_IMAGE_TYPE_2D;
				image_info.format = (dynamic_cast<WindowImpl *>(window.get()))->vk_format;
				image_info.extent.width = window->width();
				image_info.extent.height = window->height();
				image_info.extent.depth = 1;
				image_info.mipLevels = 1;
				image_info.arrayLayers = 1;
				image_info.samples = VK_SAMPLE_COUNT_1_BIT;
				image_info.tiling = VK_IMAGE_TILING_OPTIMAL;
				image_info.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
				image_info.flags = 0;

				std::memset(&view_info, 0, sizeof(VkImageViewCreateInfo));
				view_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
				view_info.pNext = NULL;
				view_info.image = VK_NULL_HANDLE;
				view_info.format = (dynamic_cast<WindowImpl *>(window.get()))->vk_format;
				view_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
				view_info.subresourceRange.baseMipLevel = 0;
				view_info.subresourceRange.levelCount = 1;
				view_info.subresourceRange.baseArrayLayer = 0;
				view_info.subresourceRange.layerCount = 1;
				view_info.flags = 0;
				view_info.viewType = VK_IMAGE_VIEW_TYPE_2D;

				r = vkCreateImage(device->vk_device, &image_info, NULL, &depth_vk_image);
				F3D_ASSERT_VK(r, VK_SUCCESS, "Cannot create depth image");

				vkGetImageMemoryRequirements(device->vk_device, depth_vk_image, &mem_reqs);

				mem_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
				mem_info.pNext = NULL;
				mem_info.allocationSize = mem_reqs.size;
				mem_info.memoryTypeIndex = physical->getMemoryIndex(mem_reqs.memoryTypeBits, 0);
				F3D_ASSERT(mem_info.memoryTypeIndex != -1, "Cant find depth memory type");

				r = vkAllocateMemory(device->vk_device, &mem_info, NULL, &depth_vk_memory);
				F3D_ASSERT_VK(r, VK_SUCCESS, "Allocating depth buffer memory failed");

				r = vkBindImageMemory(device->vk_device, depth_vk_image, depth_vk_memory, 0);
				F3D_ASSERT_VK(r, VK_SUCCESS, "Cant vind depth image to depth buffer");

				view_info.image = depth_vk_image;
				r = vkCreateImageView(device->vk_device, &view_info, NULL, &depth_vk_view);
				F3D_ASSERT_VK(r, VK_SUCCESS, "Create depth image view failed");

				r_bool = device->initImageLayout(depth_vk_image, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL, 0, VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT, VK_IMAGE_ASPECT_DEPTH_BIT);
				F3D_ASSERT(r_bool == true, "Init depth buffer image layout failed");
			}

			void		SimpleRenderPass::render(VkCommandBuffer cmd, VkFramebuffer frame, std::shared_ptr< f3d::tree::Scene> scene) {
				{
					VkRenderPassBeginInfo	rp_begin_info;
					std::memset(&rp_begin_info, 0, sizeof(VkRenderPassBeginInfo));
					rp_begin_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
					rp_begin_info.renderPass = vk_renderpass;
					rp_begin_info.framebuffer = frame;
					rp_begin_info.renderArea.offset.x = 0;
					rp_begin_info.renderArea.offset.y = 0;
					rp_begin_info.renderArea.extent.width = window->width();
					rp_begin_info.renderArea.extent.height = window->height();
					rp_begin_info.clearValueCount = 2;
					rp_begin_info.pClearValues = _clear;
					vkCmdBeginRenderPass(cmd, &rp_begin_info, VkSubpassContents::VK_SUBPASS_CONTENTS_INLINE);
				}
				{
					_prog->bind(cmd);

					vkCmdBindDescriptorSets(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, _prog->vk_pipeline_layout, 0, 1, &_set, 0, VK_NULL_HANDLE);

					vkCmdSetViewport(cmd, 0, 1, &vk_viewport);
					vkCmdSetScissor(cmd, 0, 1, &vk_scissor);

					//vkCmdBindVertexBuffers(cmd, 0, mesh.vk_buffer_count(), mesh.vk_buffers(), mesh.vk_offsets());
					//vkCmdBindIndexBuffer(cmd, mesh.vk_indices(), 0, VK_INDEX_TYPE_UINT32);
					//vkCmdDrawIndexed(cmd, mesh.vk_indices_count(), 1, 0, 0, 0);

				}
				{
					vkCmdEndRenderPass(cmd);
				}
			}
			
			void		SimpleRenderPass::updateDescriptorSet(f3d::tree::Camera& cam) {
			}
		}
	}
}