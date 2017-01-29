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

configure_file ("${CMAKE_SOURCE_DIR}/include/f3d/version.h.in"
				"${CMAKE_BINARY_DIR}/include/version.h")
				
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

#Subprojects
ADD_SUBDIRECTORY( code/ )
ADD_SUBDIRECTORY( examples/ )