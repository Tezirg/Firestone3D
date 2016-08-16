cmake_minimum_required( VERSION 3.0 )

SET( F3D_LIB_NAME f3d )
SET( F3D_HEADER_PATH ../include/f3d/ )
SET( F3D_PRIVATE_HEADER_PATH ../code/ )
SET( F3D_HEADERS 
  ${F3D_HEADER_PATH}/f3d.h
  ${F3D_HEADER_PATH}/core/firestone.h
  ${F3D_HEADER_PATH}/core/settings.h
  ${F3D_HEADER_PATH}/core/timer.h
  ${F3D_HEADER_PATH}/core/window.h
  ${F3D_HEADER_PATH}/core/renderer.h
  ${F3D_HEADER_PATH}/tree/scene.h
  ${F3D_HEADER_PATH}/tree/mesh.h
  ${F3D_HEADER_PATH}/tree/light.h
  ${F3D_HEADER_PATH}/tree/camera.h
  ${F3D_HEADER_PATH}/utils/logging.h
 )
 
 SET( F3D_PRIVATE_HEADERS 
  ${F3D_PRIVATE_HEADER_PATH}/core/firestone_impl.h
  ${F3D_PRIVATE_HEADER_PATH}/core/window_impl.h
  ${F3D_PRIVATE_HEADER_PATH}/core/physical_device.h
  ${F3D_PRIVATE_HEADER_PATH}/core/device.h
  ${F3D_PRIVATE_HEADER_PATH}/core/renderer_impl.h
  ${F3D_PRIVATE_HEADER_PATH}/core/programs/program.h
  ${F3D_PRIVATE_HEADER_PATH}/core/programs/prog_flat.h
  ${F3D_PRIVATE_HEADER_PATH}/core/renderpasses/render_pass.h
  ${F3D_PRIVATE_HEADER_PATH}/core/renderpasses/rp_simple.h
  ${F3D_PRIVATE_HEADER_PATH}/tree/scene_impl.h
  ${F3D_PRIVATE_HEADER_PATH}/tree/mesh_impl.h
  ${F3D_PRIVATE_HEADER_PATH}/tree/light_impl.h
  ${F3D_PRIVATE_HEADER_PATH}/tree/camera_impl.h
  ${F3D_PRIVATE_HEADER_PATH}/utils/vulkan.h
 )

SET( f3d_src
	 core/firestone.cpp
	 core/firestone_impl.cpp
	 core/settings.cpp
	 core/timer.cpp
	 core/window.cpp
	 core/window_impl.cpp
	 core/physical_device.cpp
	 core/device.cpp
	 core/renderer.cpp
	 core/renderer_impl.cpp
	 core/renderpasses/render_pass.cpp
	 core/renderpasses/rp_simple.cpp
	 core/programs/program.cpp
	 core/programs/prog_flat.cpp
	 tree/scene.cpp
	 tree/scene_impl.cpp
	 tree/mesh.cpp
	 tree/mesh_impl.cpp
	 tree/light.cpp
	 tree/light_impl.cpp
	 tree/camera.cpp
	 tree/camera_impl.cpp
	 utils/vulkan.cpp
	 
	 ${F3D_HEADERS}
	 ${F3D_PRIVATE_HEADERS}
)

if ( MSVC )
  ADD_DEFINITIONS( -D_SCL_SECURE_NO_WARNINGS )
  ADD_DEFINITIONS( -D_CRT_SECURE_NO_WARNINGS )
endif ( MSVC )

add_definitions( -DF3D_EXPORT_STATIC )

INCLUDE_DIRECTORIES ( ${F3D_HEADER_PATH} ${F3D_PRIVATE_HEADER_PATH} ${VULKAN_INCLUDE_DIRS} ${ASSIMP_INCLUDE_DIRS} ${GLFW_INCLUDE_DIRS})
ADD_LIBRARY ( firestone ${f3d_src} )
TARGET_LINK_LIBRARIES ( firestone ${ASSIMP_LIBRARIES} ${VULKAN_LIBRARIES} ${GLFW_LIBRARIES} )

SET_TARGET_PROPERTIES( firestone PROPERTIES
						VERSION ${F3D_VERSION}
						SOVERSION ${F3D_SOVERSION}
						OUTPUT_NAME ${F3D_LIB_NAME}
)

INSTALL( TARGETS firestone
		 LIBRARY DESTINATION ${F3D_LIB_INSTALL_DIR}
		 ARCHIVE DESTINATION ${F3D_LIB_INSTALL_DIR}
		 RUNTIME DESTINATION ${F3D_BIN_INSTALL_DIR}
)