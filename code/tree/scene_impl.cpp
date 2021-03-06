#include "scene_impl.h"

namespace f3d {
	namespace tree {
		SceneImpl::SceneImpl(std::shared_ptr<f3d::core::PhysicalDevice>& physical, std::shared_ptr<f3d::core::Device>& device) :
			_physical(physical), _device(device) {
			_camera.reset(new f3d::tree::CameraImpl(physical, device));
			
			_matrix.push(glm::mat4());
		}

		SceneImpl::~SceneImpl() {
			std::cout << "Destructor: " << __FILE__ << std::endl;
		}

		void				SceneImpl::recursive_uniformUpdate(f3d::tree::Node* f3d_node) {
			glm::mat4		t = f3d_node->transformation().getTransformation();


			t = t * _matrix.top();
			//Push local transformationmatrix
			_matrix.push(t);

			for (auto it = f3d_node->getMeshes().begin(); it != f3d_node->getMeshes().end(); ++it) {
				f3d::tree::MeshImpl* m = dynamic_cast<f3d::tree::MeshImpl*>(*it);
				m->updateDescriptorSet(t);
			}

			for (auto it = f3d_node->getChildren().begin(); it != f3d_node->getChildren().end(); ++it)
				recursive_uniformUpdate(*it);

			//Removes current tranform: stack should end with only one identity matrix
			_matrix.pop();
		}

		void				SceneImpl::recurs_aiNodeToF3d(const aiScene* scene, aiNode *ainode, f3d::tree::Node* f3d_node) {


			for (uint32_t j = 0; j < ainode->mNumMeshes; j++) {
				aiMesh*					aimesh = scene->mMeshes[ainode->mMeshes[j]];
				f3d::tree::MeshImpl*	fmesh = new f3d::tree::MeshImpl(_physical, _device);

				fmesh->setName(aimesh->mName.C_Str());
				
				aiString aiStr;
				scene->mMaterials[aimesh->mMaterialIndex]->Get(AI_MATKEY_NAME, aiStr);
				fmesh->setMaterialName(aiStr.C_Str());

				for (uint32_t v = 0; v < aimesh->mNumVertices; v++) {
					fmesh->addVertex(glm::vec3(aimesh->mVertices[v].x, aimesh->mVertices[v].y, aimesh->mVertices[v].z));
					if (aimesh->mNormals != NULL)
						fmesh->addNormal(glm::vec3(aimesh->mNormals[v].x, aimesh->mNormals[v].y, aimesh->mNormals[v].z));
					if (aimesh->HasTextureCoords(0)) {
						fmesh->addUV(aimesh->mTextureCoords[0][v].x, aimesh->mTextureCoords[0][v].y);
					}
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

		void						SceneImpl::loadFromFile(const std::string& path, const std::string& filename) {
			std::string				sceneFile(path);
			sceneFile.append(filename);

			Assimp::Importer		importer;
			const aiScene*			ai_scene = importer.ReadFile(sceneFile.c_str(), aiProcess_GenNormals | aiProcess_JoinIdenticalVertices | aiProcess_Triangulate |   \
																					aiProcess_SplitLargeMeshes | aiProcess_SortByPType | aiProcess_OptimizeMeshes | aiProcess_FlipWindingOrder);
			
			if (ai_scene == 0x0)
				F3D_FATAL_ERROR(importer.GetErrorString());

			/*
			if (ai_scene->HasCameras()) {
				_camera.reset(new f3d::tree::CameraImpl(ai_scene->mCameras[0]));
			}
			*/
			//Loading materials
			for (uint32_t i = 0; i < ai_scene->mNumMaterials; i++) {
				f3d::tree::Material* mat = _aiMaterialToF3D(path, ai_scene->mMaterials[i]);
				if (mat != nullptr)
					addMaterial(mat);
			}
				
			//Loading lights
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

		f3d::tree::Material*				SceneImpl::_aiMaterialToF3D(const std::string& path, const aiMaterial* aiMat) {
			f3d::tree::Material				*mat = nullptr;
			aiReturn						aiRes;
			aiString						aiStr;
			aiColor3D						aiColor;
			float							aiShine;
			std::string						materialName;

			aiRes = aiMat->Get(AI_MATKEY_NAME, aiStr);
			materialName.assign(aiRes == AI_SUCCESS ? aiStr.C_Str() : AI_DEFAULT_MATERIAL_NAME);
			mat = new f3d::tree::Material(materialName);

			//Query Ambient color
			aiRes = aiMat->Get(AI_MATKEY_COLOR_AMBIENT, aiColor);
			if (aiRes == AI_SUCCESS)
				mat->setColor(f3d::tree::Material::F3D_COLOR_AMBIENT, glm::vec3(aiColor.r, aiColor.g, aiColor.b));

			//Query diffuse color
			aiRes = aiMat->Get(AI_MATKEY_COLOR_DIFFUSE, aiColor);
			if (aiRes == AI_SUCCESS)
				mat->setColor(f3d::tree::Material::F3D_COLOR_DIFFUSE, glm::vec3(aiColor.r, aiColor.g, aiColor.b));

			//Query specular color
			aiRes = aiMat->Get(AI_MATKEY_COLOR_SPECULAR, aiColor);
			if (aiRes == AI_SUCCESS)
				mat->setColor(f3d::tree::Material::F3D_COLOR_SPECULAR, glm::vec3(aiColor.r, aiColor.g, aiColor.b));

			//Query Emmissive color
			aiRes = aiMat->Get(AI_MATKEY_COLOR_EMISSIVE, aiColor);
			if (aiRes == AI_SUCCESS)
				mat->setColor(f3d::tree::Material::F3D_COLOR_EMMISIVE, glm::vec3(aiColor.r, aiColor.g, aiColor.b));

			//Query reflective color
			aiRes = aiMat->Get(AI_MATKEY_COLOR_REFLECTIVE, aiColor);
			if (aiRes == AI_SUCCESS)
				mat->setColor(f3d::tree::Material::F3D_COLOR_REFLECTIVE, glm::vec3(aiColor.r, aiColor.g, aiColor.b));

			//Shineness property
			aiRes = aiMat->Get(AI_MATKEY_SHININESS, aiShine);
			if (aiRes == AI_SUCCESS)
				mat->setShininess(aiShine);


			f3d::tree::TextureImpl* texture = nullptr;
			texture = _aiTextureToF3D(path, aiMat, aiTextureType_AMBIENT, 0);
			if (texture != nullptr)
				mat->addTexture(texture);

			texture = _aiTextureToF3D(path, aiMat, aiTextureType_DIFFUSE, 0);
			if (texture != nullptr)
				mat->addTexture(texture);

			texture = _aiTextureToF3D(path, aiMat, aiTextureType_SPECULAR, 0);
			if (texture != nullptr)
				mat->addTexture(texture);

			return mat;
		}

		f3d::tree::TextureImpl*						SceneImpl::_aiTextureToF3D(const std::string& path, const aiMaterial *aiMaterial, aiTextureType aiType, uint32_t aiIndex) {
			aiReturn								aiRes;
			aiString								aiPath;
			aiTextureMapping						mapping;
			unsigned int  							uvindex;
			float  									blend;
			aiTextureOp  							op;
			aiTextureMapMode  						mapmode[3];

			f3d::tree::TextureImpl*					texture = nullptr;
			f3d::tree::Texture::eTextureType		type = f3d::tree::Texture::F3D_TEXTURE_UNDEFINED;
			f3d::tree::Texture::eTextureAddressMode	address_mode = f3d::tree::Texture::F3D_ADDRESS_UNDEFINED;

			aiRes = aiMaterial->GetTexture(aiType, aiIndex, &aiPath, &mapping, &uvindex, &blend, &op, mapmode);
			if (aiRes != AI_SUCCESS)
				return nullptr;
			type = (aiType == aiTextureType_AMBIENT) ? f3d::tree::Texture::F3D_TEXTURE_AMBIENT : type;
			type = (aiType == aiTextureType_DIFFUSE) ? f3d::tree::Texture::F3D_TEXTURE_DIFFUSE : type;
			type = (aiType == aiTextureType_SPECULAR) ? f3d::tree::Texture::F3D_TEXTURE_SPECULAR : type;
			type = (aiType == aiTextureType_EMISSIVE) ? f3d::tree::Texture::F3D_TEXTURE_EMISSIVE : type;
			type = (aiType == aiTextureType_HEIGHT) ? f3d::tree::Texture::F3D_TEXTURE_HEIGHT : type;
			type = (aiType == aiTextureType_NORMALS) ? f3d::tree::Texture::F3D_TEXTURE_NORMALS : type;
			type = (aiType == aiTextureType_SHININESS) ? f3d::tree::Texture::F3D_TEXTURE_SHININESS : type;
			type = (aiType == aiTextureType_OPACITY) ? f3d::tree::Texture::F3D_TEXTURE_OPACITY : type;
			type = (aiType == aiTextureType_DISPLACEMENT) ? f3d::tree::Texture::F3D_TEXTURE_DISPLACEMENT : type;
			type = (aiType == aiTextureType_LIGHTMAP) ? f3d::tree::Texture::F3D_TEXTURE_LIGHTMAP : type;
			type = (aiType == aiTextureType_REFLECTION) ? f3d::tree::Texture::F3D_TEXTURE_REFLECTION : type;
			address_mode = (mapmode[0] == aiTextureMapMode_Wrap) ? f3d::tree::Texture::F3D_ADDRESS_REPEAT : address_mode;
			address_mode = (mapmode[0] == aiTextureMapMode_Clamp) ? f3d::tree::Texture::F3D_ADDRESS_CLAMP_BORDER : address_mode;
			address_mode = (mapmode[0] == aiTextureMapMode_Decal) ? f3d::tree::Texture::F3D_ADDRESS_CLAMP_EDGE : address_mode;
			address_mode = (mapmode[0] == aiTextureMapMode_Mirror) ? f3d::tree::Texture::F3D_ADDRESS_MIRROR_REPEAT : address_mode;

			
			std::string texturePath(path);
			std::string	textureExtension(aiPath.C_Str());
			texturePath.append(aiPath.C_Str());

			textureExtension = textureExtension.substr(textureExtension.find_last_of(".") + 1);

			if (textureExtension == "dds" || textureExtension == "ktx") {

				gli::texture	gli_texture = gli::load(texturePath);
				if (gli_texture.empty()) {
					std::cout << "texture is empty" << std::endl;
					return nullptr;
				}

				gli::texture2d gli_texture2d(gli_texture);
				if (gli_texture2d.empty()) {
					std::cout << "texture2D is empty" << std::endl;
					return nullptr;
				}

				if (gli_texture2d[0].empty()) {
					std::cout << "Image is empty" << std::endl;
					return nullptr;
				}

				texture = new f3d::tree::TextureImpl(gli_texture2d[0].extent().x, gli_texture2d[0].extent().y, gli_texture2d.levels(), type, address_mode, _physical, _device);
				texture->initializeLinearTiling(gli_texture2d[0].extent().x, gli_texture2d[0].extent().y, gli_texture2d[0].data(), gli_texture2d[0].size(), VK_FORMAT_R8G8B8A8_SNORM);
			}
			else if (textureExtension == "jpg" || textureExtension == "jpeg" || textureExtension == "tga" || textureExtension == "bmp" || textureExtension == "gif") {
				int x, y, n;
				unsigned char *data = stbi_load(texturePath.c_str(), &x, &y, &n, STBI_rgb_alpha);
				texture = new f3d::tree::TextureImpl(x, y, 0, type, address_mode, _physical, _device);
				texture->initializeLinearTiling(x, y, data, x * y * 4, VK_FORMAT_R8G8B8A8_UNORM);
				stbi_image_free(data);
			}
			else if (textureExtension == "png") {
				std::vector<unsigned char> image; //the raw pixels
				unsigned width, height;

				//decode
				unsigned error = lodepng::decode(image, width, height, texturePath.c_str());
				//if there's an error, display it
				if (error) std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;
				texture = new f3d::tree::TextureImpl(width, height, 0, type, address_mode, _physical, _device);
				texture->initializeLinearTiling(width, height, image.data(), image.size(), VK_FORMAT_R8G8B8A8_UNORM);
			}
			else {
				return nullptr;
			}
			
			return texture;

		}
	}
}