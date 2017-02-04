#include "scene_impl.h"

namespace f3d {
	namespace tree {
		SceneImpl::SceneImpl(std::shared_ptr<f3d::core::PhysicalDevice>& physical, std::shared_ptr<f3d::core::Device>& device) :
			_physical(physical), _device(device) {
			_camera.reset(new f3d::tree::CameraImpl(physical, device));
		}

		SceneImpl::~SceneImpl() {

		}

		void				SceneImpl::recurs_aiNodeToF3d(const aiScene* scene, aiNode *ainode, f3d::tree::Node* f3d_node) {


			for (uint32_t j = 0; j < ainode->mNumMeshes; j++) {
				aiMesh*					aimesh = scene->mMeshes[ainode->mMeshes[j]];
				f3d::tree::MeshImpl*	fmesh = new f3d::tree::MeshImpl(_physical, _device);

				fmesh->setName(aimesh->mName.C_Str());
				for (uint32_t v = 0; v < aimesh->mNumVertices; v++) {
					fmesh->addVertex(glm::vec3(aimesh->mVertices[v].x, aimesh->mVertices[v].y, aimesh->mVertices[v].z));
					if (aimesh->mNormals != NULL)
						fmesh->addNormal(glm::vec3(aimesh->mNormals[v].x, aimesh->mNormals[v].y, aimesh->mNormals[v].z));
				}
				for (uint32_t f = 0; f < aimesh->mNumFaces; f++) {
					fmesh->addTriangle(aimesh->mFaces[f].mIndices[0], aimesh->mFaces[f].mIndices[1], aimesh->mFaces[f].mIndices[2]);
				}

				fmesh->makeRenderReady();
				f3d_node->addMesh(fmesh);
				std::cout << "Vertices: " << fmesh->numVertices() << std::endl;
				std::cout << "Normals: " << fmesh->numNormals() << std::endl;
				std::cout << "Indices: " << fmesh->numIndices() << std::endl;
			}

			for (uint32_t i = 0; i < ainode->mNumChildren; i++) {
				aiNode *aichild = ainode->mChildren[i];
				f3d::tree::Node *f3dchild = new f3d::tree::Node();
				
				glm::mat4 t;
				t[0][0] = aichild->mTransformation.a1; t[1][0] = aichild->mTransformation.a2; t[2][0] = aichild->mTransformation.a3; t[3][0] = aichild->mTransformation.a4;
				t[0][1] = aichild->mTransformation.b1; t[1][1] = aichild->mTransformation.b2; t[2][1] = aichild->mTransformation.b3; t[3][1] = aichild->mTransformation.b4;
				t[0][2] = aichild->mTransformation.c1; t[1][2] = aichild->mTransformation.c2; t[2][2] = aichild->mTransformation.c3; t[3][2] = aichild->mTransformation.c4;
				t[0][3] = aichild->mTransformation.d1; t[1][3] = aichild->mTransformation.d2; t[2][3] = aichild->mTransformation.d3; t[3][3] = aichild->mTransformation.d4;
				f3dchild->transformation().setTransformation(t);
				recurs_aiNodeToF3d(scene, aichild, f3dchild);
				f3d_node->addChildren(f3dchild);
			}
		}

		void						SceneImpl::loadFromFile(const std::string& path) {
			Assimp::Importer		importer;
			const aiScene*			ai_scene = importer.ReadFile(path.c_str(), aiProcess_GenNormals | aiProcess_JoinIdenticalVertices | aiProcess_Triangulate |    \
																			   aiProcess_SplitLargeMeshes | aiProcess_SortByPType | aiProcess_OptimizeMeshes);
			
			if (ai_scene == 0x0)
				F3D_FATAL_ERROR(importer.GetErrorString());

			/*
			if (ai_scene->HasCameras()) {
				_camera.reset(new f3d::tree::CameraImpl(ai_scene->mCameras[0]));
			}
			*/

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