#include "scene_impl.h"

namespace f3d {
	namespace tree {
		SceneImpl::SceneImpl(std::shared_ptr<f3d::core::PhysicalDevice>& physical, std::shared_ptr<f3d::core::Device>& device) :
			_physical(physical), _device(device) {

		}

		SceneImpl::~SceneImpl() {

		}

		void				SceneImpl::recurs_aiNodeToF3d(const aiScene* scene, aiNode *ainode, f3d::tree::Node* f3d_node) {
			for (uint32_t i = 0; i < ainode->mNumChildren; i++) {
				aiNode *aichild = ainode->mChildren[i];
				f3d::tree::Node *f3dchild = new f3d::tree::Node();
				
				f3dchild->transformation().setTransformation(aichild->mTransformation);
				for (uint32_t j = 0; j < aichild->mNumMeshes; j++) {
					aiMesh*					aimesh = scene->mMeshes[aichild->mMeshes[j]];
					f3d::tree::MeshImpl*	fmesh = new f3d::tree::MeshImpl(_physical, _device);
					
					fmesh->setName(aimesh->mName.C_Str());
					for (uint32_t v = 0; v < aimesh->mNumVertices; v++) {
						fmesh->addVertex(aimesh->mVertices[v]);
						fmesh->addNormal(aimesh->mNormals[v]);
					}
					for (uint32_t f = 0; f < aimesh->mNumFaces; f++) {
						fmesh->addTriangle(aimesh->mFaces[f].mIndices[0], aimesh->mFaces[f].mIndices[1], aimesh->mFaces[f].mIndices[2]);
					}

					fmesh->makeRenderReady();
					f3dchild->addMesh(fmesh);
				}
				std::cout << "Recurs" << std::endl;
				recurs_aiNodeToF3d(scene, aichild, f3dchild);
			}
		}

		void						SceneImpl::loadFromFile(const std::string& path) {
			Assimp::Importer		importer;
			const aiScene*			ai_scene = importer.ReadFile(path.c_str(), aiProcessPreset_TargetRealtime_Fast);
			
			if (ai_scene->HasCameras()) {
				_camera.reset(new f3d::tree::CameraImpl(ai_scene->mCameras[0]));
			}

			for (uint32_t i = 0; i < ai_scene->mNumLights; i++) {
				addLight(new f3d::tree::LightImpl(ai_scene->mLights[i]));
			}
	
			if (ai_scene->mNumMeshes == 0) { //Root that contains multiple objs 
				for (uint32_t i = 0; i < ai_scene->mRootNode->mNumChildren; i++) {
					aiNode				*c = ai_scene->mRootNode->mChildren[i];
					f3d::tree::Object	*o = new f3d::tree::Object();

					recurs_aiNodeToF3d(ai_scene, c, o->getRoot());
					addObject(o);
				}
			}
			else { //Only one object
				f3d::tree::Object	*o = new f3d::tree::Object();
				recurs_aiNodeToF3d(ai_scene, ai_scene->mRootNode, o->getRoot());
				addObject(o);
			}
		
			importer.FreeScene();
		}
	}
}