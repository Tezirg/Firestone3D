#include "tree/object.h"

namespace f3d {
	namespace tree {
		Object::Object() : _root(new f3d::tree::Node()) {

		}

		Object::~Object() {

		}

		Node*			Object::getRoot() {
			return _root;
		}

		void			Object::setRoot(Node *r) {
			_root = r;
		}

	}
}