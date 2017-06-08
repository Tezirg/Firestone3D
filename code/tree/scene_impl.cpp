#include "scene_impl.h"

namespace f3d {
	namespace tree {
		SceneImpl::SceneImpl(std::shared_ptr<f3d::core::PhysicalDevice>& physical, std::shared_ptr<f3d::core::Device>& device) :
			_physical(physical), _device(device),
			AttributeContainer::AttributeContainer(physical, device),
			DescriptorContainer::DescriptorContainer(physical, device) {
			_camera.reset(new f3d::tree::CameraImpl(physical, device));
			
			_matrix.push(glm::mat4());

			DescriptorContainer::addDescriptor(0);
			DescriptorContainer::addDescriptorBinding(0, 0, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, VK_SHADER_STAGE_FRAGMENT_BIT);
			DescriptorContainer::addDescriptorBinding(0, 1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_FRAGMENT_BIT);
			AttributeContainer::addAttribute(0, (sizeof(float) * 25 + sizeof(uint32_t) + 6) * 16, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT);
			AttributeContainer::addAttribute(1, sizeof(uint32_t), VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER);
		}

		SceneImpl::~SceneImpl() {
			std::cout << "Destructor: " << __FILE__ << std::endl;
		}

		void				SceneImpl::addLight(f3d::tree::Light *l) {
			f3d::tree::LightImpl	*li = new f3d::tree::LightImpl(_physical, _device, *l);
		
			std::cout << l->getName() << std::endl;
			//Call inherited adder with forced light implementation instance
			Scene::addLight(li);
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
					else
						fmesh->addNormal(glm::vec3(0.0f));
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

		void							SceneImpl::loadFromFile(const std::string& path, const std::string& filename) {
			std::string					sceneFile(path);
			Assimp::Importer			importer;
			const aiScene*				ai_scene = nullptr;

			//Load scene with assimp
			sceneFile.append(filename);
			ai_scene = importer.ReadFile(sceneFile.c_str(), aiProcess_GenSmoothNormals | \
															aiProcess_Triangulate | aiProcess_SplitLargeMeshes | \
															aiProcess_SortByPType | aiProcess_OptimizeMeshes | \
															aiProcess_FlipWindingOrder);
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
			char *b;
			for (uint32_t i = 0; i < ai_scene->mNumLights && i < 16; i++) {
				f3d::tree::LightImpl * l = new f3d::tree::LightImpl(_physical, _device, ai_scene->mLights[i]);
				Scene::addLight(l);
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
		
			//Release assimp scene
			importer.FreeScene();
		}

		f3d::tree::Material*				SceneImpl::_aiMaterialToF3D(const std::string& path, const aiMaterial* aiMat) {
			f3d::tree::MaterialImpl			*mat = nullptr;
			aiReturn						aiRes;
			aiString						aiStr;
			aiColor3D						aiColor;
			float							aiShine;
			std::string						materialName;

			aiRes = aiMat->Get(AI_MATKEY_NAME, aiStr);
			materialName.assign(aiRes == AI_SUCCESS ? aiStr.C_Str() : AI_DEFAULT_MATERIAL_NAME);
			mat = new f3d::tree::MaterialImpl(materialName, _physical, _device);


			std::cout << "====================================" <<  materialName << std::endl;
			//Query Ambient color
			aiRes = aiMat->Get(AI_MATKEY_COLOR_AMBIENT, aiColor);
			if (aiRes == AI_SUCCESS && aiColor.IsBlack() == false)
				mat->setColor(f3d::F3D_COLOR_AMBIENT, glm::vec3(aiColor.r, aiColor.g, aiColor.b));
			std::cout << "has Ambient " << !aiColor.IsBlack() << std::endl;
			//Query diffuse color
			aiRes = aiMat->Get(AI_MATKEY_COLOR_DIFFUSE, aiColor);
			if (aiRes == AI_SUCCESS && aiColor.IsBlack() == false)
				mat->setColor(f3d::F3D_COLOR_DIFFUSE, glm::vec3(aiColor.r, aiColor.g, aiColor.b));
			std::cout << "has Diffuse " << !aiColor.IsBlack() << " " << std::endl;

			//Query specular color
			aiRes = aiMat->Get(AI_MATKEY_COLOR_SPECULAR, aiColor);
			if (aiRes == AI_SUCCESS && aiColor.IsBlack() == false)
				mat->setColor(f3d::F3D_COLOR_SPECULAR, glm::vec3(aiColor.r, aiColor.g, aiColor.b));

			//Query Emmissive color
			aiRes = aiMat->Get(AI_MATKEY_COLOR_EMISSIVE, aiColor);
			if (aiRes == AI_SUCCESS && aiColor.IsBlack() == false)
				mat->setColor(f3d::F3D_COLOR_EMISSIVE, glm::vec3(aiColor.r, aiColor.g, aiColor.b));

			//Query reflective color
			aiRes = aiMat->Get(AI_MATKEY_COLOR_REFLECTIVE, aiColor);
			if (aiRes == AI_SUCCESS && aiColor.IsBlack() == false)
				mat->setColor(f3d::F3D_COLOR_REFLECTIVE, glm::vec3(aiColor.r, aiColor.g, aiColor.b));

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

			mat->writeAttribute();
			mat->writeDescriptorSet();

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
			f3d::eTextureTypeBits					type = F3D_TEXTURE_UNDEFINED;
			f3d::eTextureAddressMode				address_mode[3] = { F3D_ADDRESS_UNDEFINED, F3D_ADDRESS_UNDEFINED, F3D_ADDRESS_UNDEFINED };

			if ((aiRes = aiMaterial->GetTexture(aiType, aiIndex, &aiPath, &mapping, &uvindex, &blend, &op, mapmode)) != AI_SUCCESS)
				return nullptr;

			ASSIMP_TEXTURE_2_F3D(aiType, type);
			ASSIMP_ADDRESS_MODE_2_F3D(mapmode[0], address_mode[0]);
			ASSIMP_ADDRESS_MODE_2_F3D(mapmode[1], address_mode[1]);
			ASSIMP_ADDRESS_MODE_2_F3D(mapmode[2], address_mode[2]);

			std::string texture_path(path);
			texture_path.append(aiPath.C_Str());


			try {
				std::cout << "Loading texture at " << texture_path << std::endl;

				Magick::Image	magick_texture(texture_path);
				magick_texture.magick("RGBA");

				std::cout << "texture is " << magick_texture.size().width() << "x" << magick_texture.size().height() 
						  << " format=" << magick_texture.format() 
					<< " // " << magick_texture.magick()
					<< std::endl;

				Magick::Blob	blob;
				magick_texture.write(&blob);
				uint32_t size = magick_texture.size().width() * magick_texture.size().height();
				texture = new f3d::tree::TextureImpl(magick_texture.size().width(), magick_texture.size().height(), 1, type, address_mode, _physical, _device);

				texture->initializeLinearTiling(magick_texture.size().width(), magick_texture.size().height(), (void*)blob.data(), blob.length(), VK_FORMAT_R8G8B8A8_UNORM);
			}
			catch (Magick::Exception &error_)
			{
				std::cerr << "Caught exception: " << error_.what() << std::endl;
				return nullptr;
			}			

			return texture;
		}

		void								SceneImpl::writeDescriptorSet() {
			f3d::tree::CameraImpl*			cam = dynamic_cast<f3d::tree::CameraImpl *>(_camera.get());
			VkWriteDescriptorSet			pWrites[2];
			VkDescriptorBufferInfo			buffer_info[2];

			//Write camera descriptor sets
			cam->writeDescriptorSet();

			//Write lights into descriptor set
			//Buffers in use
			buffer_info[0].offset = 0;
			buffer_info[0].range = VK_WHOLE_SIZE;
			buffer_info[0].buffer = AttributeContainer::getAttributeBuffer(0); // Buffer 0
			buffer_info[1].offset = 0;
			buffer_info[1].range = VK_WHOLE_SIZE;
			buffer_info[1].buffer = AttributeContainer::getAttributeBuffer(1); // Buffer 1
			//Bind buffer 0 to binding 0 on this descriptor set
			std::memset(&pWrites[0], 0, sizeof(VkWriteDescriptorSet));
			pWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			pWrites[0].dstSet = DescriptorContainer::getDescriptorSet(0);
			pWrites[0].descriptorCount = 1;
			pWrites[0].dstBinding = 0;
			pWrites[0].descriptorType = VkDescriptorType::VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
			pWrites[0].pBufferInfo = &buffer_info[0];

			//Bind buffer 1 to binding 1 on this descriptor set
			std::memset(&pWrites[1], 0, sizeof(VkWriteDescriptorSet));
			pWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			pWrites[1].dstSet = DescriptorContainer::getDescriptorSet(0);
			pWrites[1].descriptorCount = 1;
			pWrites[1].dstBinding = 1;
			pWrites[1].descriptorType = VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			pWrites[1].pBufferInfo = &buffer_info[1];
			vkUpdateDescriptorSets(_device->vk_device, 2, pWrites, 0, nullptr);
		}

		void								SceneImpl::writeAttribute() {
			uint32_t						i = 0;
			char							*b;
			bool							props;
			uint32_t						light_attr_size = 0;

			//Update lights Buffer
			for (auto it = _lights.begin(); it != _lights.end(); ++it) {
				f3d::tree::LightImpl * l = dynamic_cast<f3d::tree::LightImpl *>(*it);
				props = l->getProperties((void **)&b, light_attr_size);
				AttributeContainer::updateAttribute(0, b, i * light_attr_size, light_attr_size); // Light buffer on binding 0
				i++;
				//std::cout << i * light_attr_size << std::endl;
			}
			AttributeContainer::updateAttribute(1, (void*)&i, 0, sizeof(uint32_t)); // num lights in binding 1
		}


		VkDescriptorSet						SceneImpl::getWorldDescriptorSet() {
			f3d::tree::CameraImpl*			cam = dynamic_cast<f3d::tree::CameraImpl *>(_camera.get());

			return cam->getDescriptorSet();
		}

		VkDescriptorSet						SceneImpl::getLightsDescriptorSet() {
			return DescriptorContainer::getDescriptorSet(0);
		}
	}
}