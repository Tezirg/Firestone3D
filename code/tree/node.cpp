#include "tree/node.h"

namespace f3d {
	namespace tree {

		Node::Node() : _parent(nullptr) {
		}

		Node::~Node() {
		}

		Node*							Node::getParent() {
			return _parent;
		}

		void							Node::setParent(Node *p) {
			_parent = p;
		}

		std::list<Node *>&				Node::getChildren() {
			return _children;
		}

		void							Node::addChildren(Node *c) {
			_children.push_back(c);
		}

		void							Node::removeChildren(Node *c) {
			_children.remove(c);
		}

		std::list<f3d::tree::Mesh*>&	Node::getMeshes() {
			return _meshes;
		}

		void							Node::addMesh(f3d::tree::Mesh *m) {
			_meshes.push_back(m);
		}
		void							Node::removeMesh(f3d::tree::Mesh *m) {
			_meshes.remove(m);
		}

		f3d::utils::Transform&			Node::transformation() {
			return _transformation;
		}

	}
}
