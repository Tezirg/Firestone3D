#include "tree/object.h"

namespace f3d {
	namespace tree {
		Object::Object() : _root(new f3d::tree::Node()) {

		}

		Object::~Object() {
			std::cout << "Destructor: " << __FILE__ << std::endl;
		}

		Node*			Object::getRoot() {
			return _root;
		}

		void			Object::setRoot(Node *r) {
			_root = r;
		}

		void			Object::translate(const glm::vec3& translation) {
			_root->transformation().translate(translation);
		}

		void			Object::scale(const glm::vec3& scaling) {
			_root->transformation().scale(scaling);
		}

		void			Object::rotate(const float degrees, const glm::vec3& axis) {
			_root->transformation().rotate(degrees, axis);
		}
	}
}