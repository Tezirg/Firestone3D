#include "rp_simple.h"

namespace f3d {
	namespace core {
		namespace renderpass {

			SimpleRenderPass::SimpleRenderPass(std::shared_ptr<f3d::core::Device>& device, 
											   std::shared_ptr<f3d::core::PhysicalDevice>& physical, 
											   std::shared_ptr<f3d::core::Window>& window):
				RenderPass::RenderPass(F3D_RENDERPASS_SIMPLE, device, physical, window), 
				_color_format(VK_FORMAT_B8G8R8A8_UNORM),
				_flat_prog(new f3d::core::prog::FlatProgram(device->vk_device)),
				_texture_prog(new f3d::core::prog::TexturedProgram(device->vk_device)) {


				WindowImpl *w = dynamic_cast<WindowImpl *>(window.get());

				_color_format = w->vk_format;
				vk_subpasses_count = 1;
				vk_subpasses = new VkSubpassDescription[1];
				_clear[0].color.float32[0] = 0.2f;
				_clear[0].color.float32[1] = 0.2f;
				_clear[0].color.float32[2] = 0.2f;
				_clear[0].color.float32[3] = 0.0f;
				_clear[1].depthStencil.depth = 1.0f;
				_clear[1].depthStencil.stencil = 0;

				initRenderPass();
				_flat_prog->initVkPipeline(vk_renderpass, 0);
				_texture_prog->initVkPipeline(vk_renderpass, 0);

				_depth.reset(new f3d::core::Depth(device, physical, window->width(), window->height()));
				initViews();
				initFramebuffers();
			}
			
			SimpleRenderPass::~SimpleRenderPass() {
				std::cout << "Destructor: " << __FILE__ << std::endl;
			}

			void						SimpleRenderPass::initFramebuffers() {
				VkImageView					attachments[2];
				VkFramebufferCreateInfo		fb_info;
				VkResult					r;
				WindowImpl					*win = dynamic_cast<WindowImpl *>(window.get());

				attachments[1] = _depth->vk_view;

				std::memset(&fb_info, 0, sizeof(VkFramebufferCreateInfo));
				fb_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
				fb_info.pNext = NULL;
				fb_info.renderPass = vk_renderpass;
				fb_info.attachmentCount = 2;
				fb_info.pAttachments = attachments;
				fb_info.width = window->width();
				fb_info.height = window->height();
				fb_info.layers = 1;

				vk_framebuffers = new VkFramebuffer[win->vk_image_count];

				for (uint32_t i = 0; i < win->vk_image_count; i++) {
					attachments[0] = vk_views[i];
					r = vkCreateFramebuffer(device->vk_device, &fb_info, NULL, &(vk_framebuffers[i]));
					F3D_ASSERT_VK(r, VK_SUCCESS, "Framebuffer creation fail");
				}
			}

			void						SimpleRenderPass::initViews() {
				VkResult				r;
				VkImageViewCreateInfo	img_view_info;
				WindowImpl				*win = dynamic_cast<WindowImpl *>(window.get());

				vk_views = new VkImageView[win->vk_image_count];
				for (uint32_t i = 0; i < win->vk_image_count; i++) {

					std::memset(&img_view_info, 0, sizeof(VkImageViewCreateInfo));
					img_view_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
					img_view_info.image = win->vk_images[i];
					img_view_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
					img_view_info.format = win->vk_format;
					img_view_info.components.r = VK_COMPONENT_SWIZZLE_R;
					img_view_info.components.g = VK_COMPONENT_SWIZZLE_G;
					img_view_info.components.b = VK_COMPONENT_SWIZZLE_B;
					img_view_info.components.a = VK_COMPONENT_SWIZZLE_A;
					img_view_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
					img_view_info.subresourceRange.baseMipLevel = 0;
					img_view_info.subresourceRange.levelCount = 1;
					img_view_info.subresourceRange.baseArrayLayer = 0;
					img_view_info.subresourceRange.layerCount = 1;

					r = vkCreateImageView(device->vk_device, &img_view_info, NULL, &(vk_views[i]));
					F3D_ASSERT_VK(r, VK_SUCCESS, "Create image view failed");
				}
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
				_attachments[0].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
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

			void									SimpleRenderPass::render(VkCommandBuffer cmd, std::shared_ptr< f3d::tree::Scene > scene) {
				WindowImpl							*win = dynamic_cast<WindowImpl *>(window.get());
				VkCommandBufferInheritanceInfo		cmd_hinfo;
				VkCommandBufferBeginInfo			cmd_info;
				VkRenderPassBeginInfo				rp_begin_info;
				VkImageMemoryBarrier				prePresentBarrier;
				VkResult							r;

				std::memset(&cmd_hinfo, 0, sizeof(VkCommandBufferInheritanceInfo));
				cmd_hinfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO;
				cmd_hinfo.renderPass = VK_NULL_HANDLE;
				cmd_hinfo.subpass = 0;
				cmd_hinfo.framebuffer = VK_NULL_HANDLE;
				cmd_hinfo.occlusionQueryEnable = VK_FALSE;
				cmd_hinfo.queryFlags = 0;
				cmd_hinfo.pipelineStatistics = 0;

				std::memset(&cmd_info, 0, sizeof(VkCommandBufferBeginInfo));
				cmd_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
				cmd_info.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
				cmd_info.pInheritanceInfo = &cmd_hinfo;
				r = vkBeginCommandBuffer(cmd, &cmd_info);
				F3D_ASSERT_VK(r, VK_SUCCESS, "Begin command buffer failed");

				std::memset(&rp_begin_info, 0, sizeof(VkRenderPassBeginInfo));
				rp_begin_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
				rp_begin_info.renderPass = vk_renderpass;
				rp_begin_info.framebuffer = vk_framebuffers[win->vk_present_frame];
				rp_begin_info.renderArea.offset.x = 0;
				rp_begin_info.renderArea.offset.y = 0;
				rp_begin_info.renderArea.extent.width = window->width();
				rp_begin_info.renderArea.extent.height = window->height();
				rp_begin_info.clearValueCount = 2;
				rp_begin_info.pClearValues = _clear;
				vkCmdBeginRenderPass(cmd, &rp_begin_info, VkSubpassContents::VK_SUBPASS_CONTENTS_INLINE);

				vkCmdSetViewport(cmd, 0, 1, &vk_viewport);
				vkCmdSetScissor(cmd, 0, 1, &vk_scissor);

				//Push world identity transform
				_matrix.push(glm::mat4());
				for (auto it = scene->getObjects().begin(); it != scene->getObjects().end(); ++it) {
					cmdDrawObject(cmd, scene, (*it)->getRoot());
				}
				//Removes identity tranform: stack should be empty
				_matrix.pop();
				

				vkCmdEndRenderPass(cmd);
				// /*

				std::memset(&prePresentBarrier, 0, sizeof(prePresentBarrier));
				prePresentBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
				prePresentBarrier.srcAccessMask = VK_ACCESS_SHADER_WRITE_BIT | VK_ACCESS_TRANSFER_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
				prePresentBarrier.dstAccessMask = VK_ACCESS_HOST_READ_BIT;
				prePresentBarrier.oldLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
				prePresentBarrier.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
				prePresentBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
				prePresentBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
				prePresentBarrier.subresourceRange = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 };
				prePresentBarrier.image = win->vk_images[win->vk_present_frame];
				vkCmdPipelineBarrier(cmd, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
					VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, 0, 0, NULL, 0,
					NULL, 1, &prePresentBarrier);
				// */
				vkEndCommandBuffer(cmd);
			}
			
			void			SimpleRenderPass::cmdDrawObject(VkCommandBuffer cmd, std::shared_ptr< f3d::tree::Scene > scene, f3d::tree::Node* obj) {
				if (obj == nullptr)
					return;

				//Compute Local transform
				_matrix.push(_matrix.top() * obj->transformation().getTransformation());

				for (auto it = obj->getMeshes().begin(); it != obj->getMeshes().end(); ++it)
					cmdDrawMesh(cmd, scene, *(*it));
				for (auto it = obj->getChildren().begin(); it != obj->getChildren().end(); ++it)
					cmdDrawObject(cmd, scene, *it);

				//Remove local transforms
				_matrix.pop();
			}

			void						SimpleRenderPass::cmdDrawMesh(VkCommandBuffer cmd, std::shared_ptr< f3d::tree::Scene > scene, f3d::tree::Mesh& mesh) {
				f3d::tree::MeshImpl&	m = dynamic_cast<f3d::tree::MeshImpl&>(mesh);
				f3d::tree::CameraImpl&	cam = dynamic_cast<f3d::tree::CameraImpl&>( * scene->getCamera().get());
				f3d::tree::TextureImpl*	texture = nullptr;

				VkBuffer				vertex_bufs[3];
				VkDeviceSize			vertex_offsets[3];

				if (mesh.numUV() == 0) {

					vertex_bufs[0] = m.getVertexBuffer();
					vertex_bufs[1] = m.getNormalBuffer();
					vertex_offsets[0] = 0;
					vertex_offsets[1] = 0;

					m.updateDescriptorSet(_matrix.top());
					VkDescriptorSet sets[2] = { cam.getDescriptorSet() , m.getDescriptorSet() };

					vkCmdBindDescriptorSets(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, _flat_prog->vk_pipeline_layout, 0, 2, sets, 0, nullptr);

					_flat_prog->bind(cmd);

					vkCmdBindVertexBuffers(cmd, 0, 2, vertex_bufs, vertex_offsets);
					vkCmdBindIndexBuffer(cmd, m.getIndexBuffer(), 0, VK_INDEX_TYPE_UINT32);
					vkCmdDrawIndexed(cmd, m.numIndices(), 1, 0, 0, 0);
				}
				else {
					vertex_bufs[0] = m.getVertexBuffer();
					vertex_bufs[1] = m.getNormalBuffer();
					vertex_bufs[2] = m.getUvBuffer();
					vertex_offsets[0] = 0;
					vertex_offsets[1] = 0;
					vertex_offsets[2] = 0;

					m.updateDescriptorSet(_matrix.top());
					texture = dynamic_cast<f3d::tree::TextureImpl *>(scene->getMaterialByName(m.getMaterialName())->getTextures().front());

					VkDescriptorSet sets[3] = { cam.getDescriptorSet() , m.getDescriptorSet(),  texture->getDescriptorSet() };
					vkCmdBindDescriptorSets(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, _texture_prog->vk_pipeline_layout, 0, 3, sets, 0, nullptr);

					_texture_prog->bind(cmd);

					vkCmdBindVertexBuffers(cmd, 0, 3, vertex_bufs, vertex_offsets);
					vkCmdBindIndexBuffer(cmd, m.getIndexBuffer(), 0, VK_INDEX_TYPE_UINT32);
					vkCmdDrawIndexed(cmd, m.numIndices(), 1, 0, 0, 0);
				}
			}
		}
	}
}