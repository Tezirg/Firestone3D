#include "camera_impl.h"

namespace f3d {
	namespace tree {
		CameraImpl::CameraImpl(std::shared_ptr< f3d::core::PhysicalDevice >& phys, std::shared_ptr< f3d::core::Device > device) 
			: _physical(phys), _device(device), _ai_camera(new aiCamera) 
		{
			//applyPreset(F3D_CAMERA_PRESET_DEFAULT);
			setUpDirection(aiVector3D(0.0f, 1.0f, 0.0f));
			//setName(std::string("DefaultCamera"));
			setPosition(aiVector3D(0.0f, 0.0f, -1000.0f));
			setAspect(1280.0f / 720.0f);
			setHorizontalFOV(60.0f);
			setClipPlaneNear(0.1f);
			setClipPlaneFar(5000.0f);
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
			
			std::cout << m.a1 << "," << m.a2 << "," << m.a3 << "," << m.a4 << std::endl;
			std::cout << m.b1 << "," << m.b2 << "," << m.b3 << "," << m.b4 << std::endl;
			std::cout << m.c1 << "," << m.c2 << "," << m.c3 << "," << m.c4 << std::endl;
			std::cout << m.d1 << "," << m.d2 << "," << m.d3 << "," << m.d4 << std::endl;

			m.Transpose();
			std::memcpy(pData, &m, sizeof(aiMatrix4x4));
			vkUnmapMemory(_device->vk_device, _memory);
		}

		void				CameraImpl::getMatrix(aiMatrix4x4 & out) {
			static uint32_t angle = 0.0;

			double D2R = 3.1459 / 180.0;
			double yScale = 1.0 / tan(D2R * _ai_camera->mHorizontalFOV / 2);
			double xScale = yScale / _ai_camera->mAspect;
			double nearmfar = _ai_camera->mClipPlaneNear - _ai_camera->mClipPlaneFar;
			aiMatrix4x4 proj(
				xScale, 0, 0, 0,
				0, yScale, 0, 0,
				0, 0, (_ai_camera->mClipPlaneFar + _ai_camera->mClipPlaneNear) / nearmfar, -1,
				0, 0, 2 * _ai_camera->mClipPlaneFar*_ai_camera->mClipPlaneNear / nearmfar, 0
			);

			angle += 1;
			angle = angle % 360;

			aiMatrix4x4		view;
			view.Translation(_ai_camera->mPosition, view);

			aiMatrix4x4		model;
			model.RotationY(angle * D2R, model);
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