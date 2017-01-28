#include "camera_impl.h"

namespace f3d {
	namespace tree {
		CameraImpl::CameraImpl(std::shared_ptr< f3d::core::PhysicalDevice >& phys, std::shared_ptr< f3d::core::Device > device) 
			: _physical(phys), _device(device), _ai_camera(new aiCamera) 
		{
			//applyPreset(F3D_CAMERA_PRESET_DEFAULT);
			setUpDirection(aiVector3D(0.0f, 1.0f, 0.0f));
			//setName(std::string("DefaultCamera"));
			setPosition(aiVector3D(0.0f, 0.0f, 20.0f));
			setAspect(1280.0f / 720.0f);
			setHorizontalFOV(45.0f);
			setClipPlaneNear(0.1f);
			setClipPlaneFar(1000.0f);
			createAttribute();
			//updateAttribute();
		}

		CameraImpl::CameraImpl(aiCamera *camera) : _ai_camera(new aiCamera(*camera)) {
		}

		CameraImpl::~CameraImpl() {
		}

		void						CameraImpl::createAttribute() {
			VkResult				r;
			VkBufferCreateInfo		buff_info;
			VkMemoryRequirements	mem_reqs;
			VkMemoryAllocateInfo	mem_info;

			std::memset(&buff_info, 0, sizeof(VkBufferCreateInfo));
			buff_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			buff_info.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
			buff_info.size = sizeof(aiMatrix4x4);
			r = vkCreateBuffer(_device->vk_device, &buff_info, NULL, &_buffer);
			F3D_ASSERT_VK(r, VK_SUCCESS, "Creation vk buffer failed");

			vkGetBufferMemoryRequirements(_device->vk_device, _buffer, &mem_reqs);
			std::memset(&mem_info, 0, sizeof(VkMemoryAllocateInfo));
			mem_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
			mem_info.memoryTypeIndex = _physical->getMemoryIndex(mem_reqs.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
			mem_info.allocationSize = mem_reqs.size;
			r = vkAllocateMemory(_device->vk_device, &mem_info, NULL, &_memory);
			F3D_ASSERT_VK(r, VK_SUCCESS, "Buffer mem allocation failed");

			r = vkBindBufferMemory(_device->vk_device, _buffer, _memory, 0);
			F3D_ASSERT_VK(r, VK_SUCCESS, "Bind memory to buffer fail");
		}

		void				CameraImpl::updateAttribute() {
			VkResult			r;
			char				*pData;
			aiMatrix4x4			m;

			r = vkMapMemory(_device->vk_device, _memory, 0, sizeof(aiMatrix4x4), 0, (void **)&pData);
			F3D_ASSERT_VK(r, VK_SUCCESS, "Can't map buffer memory");

			getMatrix(m);
			
			/*
			std::cout << "MVP" << std::endl;
			std::cout << m.a1 << "," << m.a2 << "," << m.a3 << "," << m.a4 << std::endl;
			std::cout << m.b1 << "," << m.b2 << "," << m.b3 << "," << m.b4 << std::endl;
			std::cout << m.c1 << "," << m.c2 << "," << m.c3 << "," << m.c4 << std::endl;
			std::cout << m.d1 << "," << m.d2 << "," << m.d3 << "," << m.d4 << std::endl;
			std::cout << std::endl;
			*/

			m.Transpose();
			std::memcpy(pData, &m, sizeof(aiMatrix4x4));
			vkUnmapMemory(_device->vk_device, _memory);
		}

		void				CameraImpl::getMatrix(aiMatrix4x4 & out) {
			static uint32_t angle = 0;

			float D2R = 3.1459f / 180.0f;
			float yScale = 1.0f / tan(D2R * _ai_camera->mHorizontalFOV / 2.0f);
			float xScale = yScale / _ai_camera->mAspect;
			float nearmfar = _ai_camera->mClipPlaneNear - _ai_camera->mClipPlaneFar;
			aiMatrix4x4 proj(
				xScale, 0.0f, 0.0f, 0.0f,
				0.0f, yScale, 0.0f, 0.0f,
				0.0f, 0.0f, (_ai_camera->mClipPlaneFar + _ai_camera->mClipPlaneNear) / nearmfar, -1.0f,
				0.0f, 0.0f, 2.0f * _ai_camera->mClipPlaneFar*_ai_camera->mClipPlaneNear / nearmfar, 0.0f
			);
			/*
			std::cout << "Perspective " << std::endl;
			std::cout << proj.a1 << "," << proj.a2 << "," << proj.a3 << "," << proj.a4 << std::endl;
			std::cout << proj.b1 << "," << proj.b2 << "," << proj.b3 << "," << proj.b4 << std::endl;
			std::cout << proj.c1 << "," << proj.c2 << "," << proj.c3 << "," << proj.c4 << std::endl;
			std::cout << proj.d1 << "," << proj.d2 << "," << proj.d3 << "," << proj.d4 << std::endl;
			std::cout << std::endl;
			*/

			angle += 1;
			angle %= 360;

			aiMatrix4x4		view;
			view.Translation(_ai_camera->mPosition, view);
			/*
			std::cout << "View " << std::endl;
			std::cout << view.a1 << "," << view.a2 << "," << view.a3 << "," << view.a4 << std::endl;
			std::cout << view.b1 << "," << view.b2 << "," << view.b3 << "," << view.b4 << std::endl;
			std::cout << view.c1 << "," << view.c2 << "," << view.c3 << "," << view.c4 << std::endl;
			std::cout << view.d1 << "," << view.d2 << "," << view.d3 << "," << view.d4 << std::endl;
			std::cout << std::endl;
			*/

			aiMatrix4x4		model_rotated;
			aiMatrix4x4		model_translated;
			aiMatrix4x4		model_scaled;
			aiMatrix4x4		model;
			model.RotationY(angle * D2R, model_rotated);
			model.Translation(aiVector3D(0.0, 0.0, -10.0f), model_translated);
			model.Scaling(aiVector3D(200.0f), model_scaled);
			model = model_scaled * model_rotated * model_translated;

			/*
			std::cout << "Model " << std::endl;
			std::cout << model.a1 << "," << model.a2 << "," << model.a3 << "," << model.a4 << std::endl;
			std::cout << model.b1 << "," << model.b2 << "," << model.b3 << "," << model.b4 << std::endl;
			std::cout << model.c1 << "," << model.c2 << "," << model.c3 << "," << model.c4 << std::endl;
			std::cout << model.d1 << "," << model.d2 << "," << model.d3 << "," << model.d4 << std::endl;
			std::cout << std::endl;
			*/

			//out = model * view * proj;
			out = proj * view * model;
		}

		//Underlying Assimp attributes
		float 				CameraImpl::getAspect() const { return _ai_camera->mAspect; }
		void 				CameraImpl::setAspect(float val) { _ai_camera->mAspect = val; }
		float 				CameraImpl::getClipPlaneFar() const { return _ai_camera->mClipPlaneFar; }
		void 				CameraImpl::setClipPlaneFar(float val) { _ai_camera->mClipPlaneFar = val;  }
		float 				CameraImpl::getClipPlaneNear() const { return _ai_camera->mClipPlaneNear;  }
		void 				CameraImpl::setClipPlaneNear(float val) { _ai_camera->mClipPlaneNear = val; }
		float 				CameraImpl::getHorizontalFOV() const { return _ai_camera->mHorizontalFOV; }
		void 				CameraImpl::setHorizontalFOV(float val) { _ai_camera->mHorizontalFOV = val; }
		aiVector3D 			CameraImpl::getLookAt() const { return _ai_camera->mLookAt; }
		void				CameraImpl::setLookAt(const aiVector3D& val) { _ai_camera->mLookAt = val; }
		std::string 		CameraImpl::getName() const { return _ai_camera->mName.data; }
		void				CameraImpl::setName(std::string& val) { _ai_camera->mName.Set(val); }
		aiVector3D 			CameraImpl::getPosition() const { return _ai_camera->mPosition;  }
		void				CameraImpl::setPosition(const aiVector3D& val) { _ai_camera->mPosition = val;  }
		aiVector3D 			CameraImpl::getUpDirection() { return _ai_camera->mUp; }
		void				CameraImpl::setUpDirection(const aiVector3D& val) { _ai_camera->mUp = val; }
	}
}