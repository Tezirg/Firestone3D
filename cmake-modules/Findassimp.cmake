#
# Find Assimp
#
# Try to find Assimp : Open Asset Import Library.
# This module defines the following variables:
# - ASSIMP_INCLUDE_DIRS
# - ASSIMP_LIBRARIES
# - ASSIMP_FOUND
#
# The following variables can be set as arguments for the module.
# - ASSIMP_ROOT_DIR : Root library directory of Assimp
#

# Additional modules
include(FindPackageHandleStandardArgs)

if (WIN32)
	set(ASSIMP_ROOT_DIR CACHE PATH "ASSIMP root directory")
	
	# Find include files
	find_path(
		ASSIMP_INCLUDE_DIR
		NAMES assimp/scene.h
		PATHS
			$ENV{PROGRAMFILES}/include
			${ASSIMP_ROOT_DIR}/include
		DOC "The directory where assimp/scene.h resides")

	list(APPEND assimp_names "assimp")
	if( MSVC )
		if( MSVC70 OR MSVC71 )
			set(MSVC_PREFIX "vc70")
		elseif( MSVC80 )
			set(MSVC_PREFIX "vc80")
		elseif( MSVC90 )
			set(MSVC_PREFIX "vc90")
		elseif( MSVC10 )
			set(MSVC_PREFIX "vc100")
		elseif( MSVC11 )
			set(MSVC_PREFIX "vc110")
		elseif( MSVC12 )
			set(MSVC_PREFIX "vc120")
		elseif( MSVC14 )
			set(MSVC_PREFIX "vc140")
		else()
			set(MSVC_PREFIX "vc150")
		endif()
		list(APPEND assimp_names "assimp-${MSVC_PREFIX}-mt")
	endif()
	
	# Find library files
	find_library(
		ASSIMP_LIBRARY
		NAMES ${assimp_names}
		PATHS
			$ENV{PROGRAMFILES}/lib
			${ASSIMP_ROOT_DIR}/lib/Debug
			${ASSIMP_ROOT_DIR}/lib/Release
			${ASSIMP_ROOT_DIR}/lib)
else()
	# Find include files
	find_path(
		ASSIMP_INCLUDE_DIR
		NAMES assimp/scene.h
		PATHS
			/usr/include
			/usr/local/include
			/sw/include
			/opt/local/include
		DOC "The directory where assimp/scene.h resides")

	# Find library files
	find_library(
		ASSIMP_LIBRARY
		NAMES assimp
		PATHS
			/usr/lib64
			/usr/lib
			/usr/local/lib64
			/usr/local/lib
			/sw/lib
			/opt/local/lib
			${ASSIMP_ROOT_DIR}/lib
		DOC "The Assimp library")
endif()

# Handle REQUIRD argument, define *_FOUND variable
find_package_handle_standard_args(Assimp DEFAULT_MSG ASSIMP_INCLUDE_DIR ASSIMP_LIBRARY)

if (ASSIMP_FOUND)
	set(ASSIMP_LIBRARIES ${ASSIMP_LIBRARY})
	set(ASSIMP_INCLUDE_DIRS ${ASSIMP_INCLUDE_DIR})
endif()

# Hide some variables
mark_as_advanced(ASSIMP_INCLUDE_DIR ASSIMP_LIBRARY)