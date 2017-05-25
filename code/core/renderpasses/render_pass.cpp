#include "render_pass.h"

namespace f3d {
	namespace core {
		RenderPass::RenderPass(RenderPassType type, 
								std::shared_ptr<f3d::core::Device>& dev, 
								std::shared_ptr<f3d::core::PhysicalDevice>& phys, 
								std::shared_ptr<f3d::core::Window>& win):
			_type(type), device(dev), physical(phys), window(win) {
			std::memset(&vk_viewport, 0, sizeof(vk_viewport));
			vk_viewport.width = (float)window->width();
			vk_viewport.height = (float)window->height();
			vk_viewport.minDepth = 0.0f;
			vk_viewport.maxDepth = 1.0f;

			std::memset(&vk_scissor, 0, sizeof(vk_scissor));
			vk_scissor.extent.width = window->width();
			vk_scissor.extent.height = window->height();
			vk_scissor.offset.x = 0;
			vk_scissor.offset.y = 0;
			vk_views = nullptr;
			vk_framebuffers = nullptr;
		}

		RenderPass::~RenderPass() {
			std::cout << "Destructor: " << __FILE__ << std::endl;
		}

		RenderPassType			RenderPass::getType() const {
			return _type;
		}

		void					RenderPass::setType(const RenderPassType type) {
			_type = type;
		}

		f3d::core::Program*		RenderPass::getProgram(const F3D_Mask mask) {
			std::cout << std::hex << mask << std::endl;
			try {
				return _programs.at(mask).get();
			}
			catch (std::exception& error) {
				return nullptr;
			}
		}

		void					RenderPass::setProgram(f3d::core::Program *program) {
			std::cout << std::hex << program->getMask() << std::endl;

			_programs[program->getMask()].reset(program);
		}
	}
}