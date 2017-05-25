cmake_minimum_required( VERSION 3.0 )

SET( F3D_HEADER_PATH ../include/f3d/ )
SET( F3D_PRIVATE_HEADER_PATH ./ )

SET ( f3d_core_sdk
	core/firestone.cpp
	core/settings.cpp
	core/window.cpp
	core/renderer.cpp
)
SET ( f3d_core_sdk_headers
	${F3D_HEADER_PATH}/f3d.h
	${F3D_HEADER_PATH}/core/firestone.h
	${F3D_HEADER_PATH}/core/platform.h
	${F3D_HEADER_PATH}/core/types.h
	${F3D_HEADER_PATH}/core/settings.h
	${F3D_HEADER_PATH}/core/window.h
	${F3D_HEADER_PATH}/core/renderer.h
) 
SOURCE_GROUP( sdk\\core FILES ${f3d_core_sdk} ${f3d_core_sdk_headers} )
 
SET ( f3d_core_src
	core/firestone_impl.cpp
	core/window_impl.cpp
	core/physical_device.cpp
	core/device.cpp
	core/depth.cpp
	core/renderer_impl.cpp
	core/renderpasses/rp_simple.cpp
	core/renderpasses/render_pass.cpp
	core/programs/program.cpp
	core/programs/p-0001_0000_0000_0000.cpp
	core/programs/p-0001_0000_0000_0001.cpp
	core/programs/prog_flat.cpp
	core/programs/prog_textured.cpp
)
SET ( f3d_core_headers 
	${F3D_PRIVATE_HEADER_PATH}/core/firestone_impl.h
	${F3D_PRIVATE_HEADER_PATH}/core/window_impl.h
	${F3D_PRIVATE_HEADER_PATH}/core/physical_device.h
	${F3D_PRIVATE_HEADER_PATH}/core/device.h
	${F3D_PRIVATE_HEADER_PATH}/core/depth.h
	${F3D_PRIVATE_HEADER_PATH}/core/renderer_impl.h
	${F3D_PRIVATE_HEADER_PATH}/core/renderpasses/render_pass.h	
	${F3D_PRIVATE_HEADER_PATH}/core/renderpasses/rp_simple.h
	${F3D_PRIVATE_HEADER_PATH}/core/programs/program.h
	${F3D_PRIVATE_HEADER_PATH}/core/programs/p-0001_0000_0000_0000.h
	${F3D_PRIVATE_HEADER_PATH}/core/programs/p-0001_0000_0000_0001.h
	${F3D_PRIVATE_HEADER_PATH}/core/programs/prog_flat.h
	${F3D_PRIVATE_HEADER_PATH}/core/programs/prog_textured.h
)

SOURCE_GROUP( impl\\core FILES ${f3d_core_src} ${f3d_core_headers})
 
SET ( f3d_tree_sdk 
	tree/scene.cpp
	tree/object.cpp
	tree/mesh.cpp
	tree/camera.cpp
	tree/light.cpp
	tree/node.cpp
	tree/material.cpp
	tree/texture.cpp
)
SET ( f3d_tree_sdk_headers
	${F3D_HEADER_PATH}/tree/scene.h
	${F3D_HEADER_PATH}/tree/object.h
	${F3D_HEADER_PATH}/tree/mesh.h
	${F3D_HEADER_PATH}/tree/light.h
	${F3D_HEADER_PATH}/tree/camera.h
	${F3D_HEADER_PATH}/tree/node.h
	${F3D_HEADER_PATH}/tree/material.h
	${F3D_HEADER_PATH}/tree/texture.h
)
SOURCE_GROUP ( sdk\\tree FILES ${f3d_tree_sdk} ${f3d_tree_sdk_headers} ) 

SET ( f3d_tree_src 
	tree/scene_impl.cpp
	tree/mesh_impl.cpp
	tree/light_impl.cpp
	tree/camera_impl.cpp
	tree/material_impl.cpp	
	tree/texture_impl.cpp
)
SET ( f3d_tree_headers
	${F3D_PRIVATE_HEADER_PATH}/tree/scene_impl.h
	${F3D_PRIVATE_HEADER_PATH}/tree/mesh_impl.h
	${F3D_PRIVATE_HEADER_PATH}/tree/light_impl.h
	${F3D_PRIVATE_HEADER_PATH}/tree/camera_impl.h
	${F3D_PRIVATE_HEADER_PATH}/tree/material_impl.h
	${F3D_PRIVATE_HEADER_PATH}/tree/texture_impl.h
)
SOURCE_GROUP( impl\\tree FILES ${f3d_tree_src} ${f3d_tree_headers} )
 
 
SET ( f3d_utils_sdk
		utils/inputs.cpp
		utils/timer.cpp
		utils/transform.cpp	
) 
SET ( f3d_utils_sdk_headers
	${F3D_HEADER_PATH}/utils/inputs.h
	${F3D_HEADER_PATH}/utils/timer.h
	${F3D_HEADER_PATH}/utils/transform.h
	
	${F3D_HEADER_PATH}/helpers/xbox_360.hpp
)
SOURCE_GROUP ( sdk\\utils FILES ${f3d_utils_sdk} ${f3d_utils_sdk_headers} )

SET ( f3d_utils_src
	utils/vulkan.cpp
)
SET ( f3d_utils_headers
	${F3D_PRIVATE_HEADER_PATH}/utils/vulkan.h
	${F3D_PRIVATE_HEADER_PATH}/utils/f3d_to_vk.h
	) 
SOURCE_GROUP( impl\\utils FILES ${f3d_utils_src} ${f3d_utils_headers} )

SET( f3d_src
	${f3d_core_sdk}
	${f3d_core_sdk_headers}
	${f3d_core_src}
	${f3d_core_headers}

	${f3d_tree_sdk}
	${f3d_tree_sdk_headers}
	${f3d_tree_src}
	${f3d_tree_headers}
	 
	${f3d_utils_sdk}
	${f3d_utils_sdk_headers}
	${f3d_utils_src}
	${f3d_utils_headers}
)

if ( MSVC )
	ADD_DEFINITIONS( -D_SCL_SECURE_NO_WARNINGS )
	ADD_DEFINITIONS( -D_CRT_SECURE_NO_WARNINGS )
endif ()

ADD_DEFINITIONS( -D_USE_MATH_DEFINES )

if ( F3D_EXPORT_STATIC )
	SET (F3D_EXPORT_DEF -DF3D_EXPORT_STATIC)
	SET (F3D_LINK STATIC)
else ()
	SET (F3D_EXPORT_DEF -DF3D_EXPORT_DYNAMIC)
	SET (F3D_LINK SHARED)
endif ()
	
ADD_DEFINITIONS( ${F3D_EXPORT_DEF} )

ADD_LIBRARY ( firestone ${F3D_LINK} ${f3d_src} )
TARGET_INCLUDE_DIRECTORIES ( firestone
	PUBLIC ${F3D_HEADER_PATH} 
	PUBLIC ${F3D_PRIVATE_HEADER_PATH}
	PUBLIC ${VULKAN_INCLUDE_DIRS} 
	PUBLIC ${ASSIMP_INCLUDE_DIRS} 
	PUBLIC ${GLFW_INCLUDE_DIRS}
	PUBLIC ${ImageMagick_INCLUDE_DIRS}
)
TARGET_LINK_LIBRARIES ( firestone ${ASSIMP_LIBRARIES} ${VULKAN_LIBRARIES} ${GLFW_LIBRARIES} ${ImageMagick_LIBRARIES})

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