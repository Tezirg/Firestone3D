cmake_minimum_required( VERSION 3.0 )

### PROJECT
#Project declaration
project( Firestone3D )
#Readme
SET( F3D_README "${CMAKE_SOURCE_DIR}/README.md" )

#Version
SET( F3D_VERSION_MAJOR 0 )
SET( F3D_VERSION_MINOR 1 )
SET( F3D_VERSION_PATCH 0 )
SET( F3D_VERSION_TWEAK 0 )
SET( F3D_VERSION ${F3D_VERSION_MAJOR}.${F3D_VERSION_MINOR}.${F3D_VERSION_PATCH}.${F3D_VERSION_TWEAK} )
SET( F3D_SOVERSION ${F3D_VERSION_MAJOR} )

if ( MSVC )
	if( MSVC70 OR MSVC71 )
		SET(MSVC_PREFIX "vc70")
	elseif( MSVC80 )
		SET(MSVC_PREFIX "vc80")
	elseif( MSVC90 )
		SET(MSVC_PREFIX "vc90")
	elseif( MSVC10 )
		SET(MSVC_PREFIX "vc100")
	elseif( MSVC11 )
		SET(MSVC_PREFIX "vc110")
	elseif( MSVC12 )
		SET(MSVC_PREFIX "vc120")
	elseif( MSVC14 )
		SET(MSVC_PREFIX "vc140")
	else()
		SET(MSVC_PREFIX "vc150")
	endif()
	SET ( F3D_LIB_NAME f3d-${F3D_VERSION}-${MSVC_PREFIX} )
else()
	SET ( F3D_LIB_NAME f3d-${F3D_VERSION} )
endif ()

configure_file ("${CMAKE_SOURCE_DIR}/include/f3d/version.h.in"
				"${CMAKE_SOURCE_DIR}/include/f3d/version.h")
				
#Install targets
SET( F3D_LIB_INSTALL_DIR "${CMAKE_BINARY_DIR}/lib" CACHE PATH
		"Path the built library files are installed to." )
SET( F3D_INCLUDE_INSTALL_DIR "${CMAKE_BINARY_DIR}/include" CACHE PATH
		"Path the header files are installed to." )
SET( F3D_BIN_INSTALL_DIR "${CMAKE_BINARY_DIR}/bin" CACHE PATH
		"Path the executables are installed to." )

#Compile outputs
SET( CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib" )
SET( CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib" )
SET( CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin" )
IF (CMAKE_BUILD_TYPE STREQUAL "Debug")
  SET( CMAKE_DEBUG_POSTFIX "d" CACHE STRING "Debug Postfix for lib, samples and tools" )
ELSE()
  SET( CMAKE_DEBUG_POSTFIX "" CACHE STRING "Debug Postfix for lib, samples and tools" )
ENDIF()

#Dependencies
SET( CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake-modules" )
FIND_PACKAGE( Vulkan REQUIRED )
FIND_PACKAGE( Assimp REQUIRED )
FIND_PACKAGE( GLFW REQUIRED )

INCLUDE_DIRECTORIES( "${CMAKE_BINARY_DIR}/include/" )
INCLUDE_DIRECTORIES( include/ )
INCLUDE_DIRECTORIES( include/f3d/ )
INCLUDE_DIRECTORIES( include/glm/ )
INCLUDE_DIRECTORIES( include/gli/ )
INCLUDE_DIRECTORIES( include/stb/ )

#Subprojects
ADD_SUBDIRECTORY( code/ )
ADD_SUBDIRECTORY( examples/ )