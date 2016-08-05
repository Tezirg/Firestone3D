#
# Find GLFW
#
# Try to find GLFW.
# This module defines the following variables:
# - GLFW_INCLUDE_DIRS
# - GLFW_LIBRARIES
# - GLFW_FOUND
#
# The following variables can be set as arguments for the module.
# - GLFW_ROOT_DIR : Root library directory of GLFW
#

# Additional modules
include(FindPackageHandleStandardArgs)

if (WIN32)
	set(GLFW_ROOT_DIR CACHE PATH "GLFW root directory")
	
	# Find include files
	find_path(
		GLFW_INCLUDE_DIR
		NAMES GLFW/glfw3.h
		PATHS
			$ENV{PROGRAMFILES}/include
			${GLFW_ROOT_DIR}/include
		DOC "The directory where GLFW/glfw3.h resides")

	
	# Find library files
	find_library(
		GLFW_LIBRARY
		NAMES glfw3
		PATHS
			$ENV{PROGRAMFILES}/lib
			${GLFW_ROOT_DIR}/src/Debug
			${GLFW_ROOT_DIR}/src/Release
			${GLFW_ROOT_DIR}/src)
else()
	# Find include files
	find_path(
		GLFW_INCLUDE_DIR
		NAMES GLFW/glfw3.h
		PATHS
			/usr/include
			/usr/local/include
			/sw/include
			/opt/local/include
		DOC "The directory where GLFW/scene.h resides")

	# Find library files
	find_library(
		GLFW_LIBRARY
		NAMES GLFW
		PATHS
			/usr/lib64
			/usr/lib
			/usr/local/lib64
			/usr/local/lib
			/sw/lib
			/opt/local/lib
			${GLFW_ROOT_DIR}/src
		DOC "The GLFW library")
endif()

# Handle REQUIRD argument, define *_FOUND variable
find_package_handle_standard_args(GLFW DEFAULT_MSG GLFW_INCLUDE_DIR GLFW_LIBRARY)

if (GLFW_FOUND)
	set(GLFW_LIBRARIES ${GLFW_LIBRARY})
	set(GLFW_INCLUDE_DIRS ${GLFW_INCLUDE_DIR})
endif()

# Hide some variables
mark_as_advanced(GLFW_INCLUDE_DIR GLFW_LIBRARY)