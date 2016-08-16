#include "render_pass.h"

namespace f3d {
	namespace core {
		RenderPass::RenderPass(eRenderPassType type): _type(type) {
		}

		RenderPass::~RenderPass() {
		}

		eRenderPassType		RenderPass::getType() const {
			return _type;
		}

		void				RenderPass::setType(const eRenderPassType type) {
			_type = type;
		}
	}
}