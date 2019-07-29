#
# Copyright © 2017-2019, Battelle Memorial Institute; Lawrence Livermore National
# Security, LLC; Alliance for Sustainable Energy, LLC All rights reserved. See LICENSE
# file and DISCLAIMER for more details.
#

cmake_minimum_required(VERSION 3.4)
cmake_policy(VERSION 3.4)

project (GMLC_UTILITIES VERSION 0.1.1)

# -----------------------------------------------------------------------------
# GMLC UTILITIES library Version number
#-----------------------------------------------------------------------------
set (GMLC_UTILITIES_VERSION_BUILD )
set (GMLC_UTILITIES_DATE "07-27-19")

set(GMLC_UTILITIES_VERSION_STRING "${GMLC_UTILITIES_VERSION} (${GMLC_UTILITIES_DATE})")

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
message(STATUS "setting as master project")
    # User settable
    set(UTILITIES_CXX_STD "14"  CACHE STRING "The CMake standard to require")

  #  set(CUR_PROJ ON)
    set(CMAKE_CXX_STANDARD ${UTILITIES_CXX_STD})
    set(CMAKE_CXX_EXTENSIONS OFF)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
	set(UTILITIES_MASTER_PROJECT ON)
	include(CTest)
else()
message(STATUS "setting as subproject")
   set(UTILITIES_MASTER_PROJECT OFF)
endif()

if (NOT CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 14)
endif()
# -----------------------------------------------------------------------------
# set the module path and include some common macros
# -----------------------------------------------------------------------------
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/config/cmake/")
include(extraMacros)
include(CMakeDependentOption)


set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Prohibit in-source build
if("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
  message(
    FATAL_ERROR
      "In-source build is not supported. Please, use an empty directory for building the project."
  )
endif()

option(GMLC_UTILITIES_WITH_CMAKE_PACKAGE "Generate and install cmake package files" ${UTILITIES_MASTER_PROJECT})
mark_as_advanced(GMLC_UTILITIES_WITH_CMAKE_PACKAGE)


# Install instructions for this target
if(GMLC_UTILITIES_WITH_CMAKE_PACKAGE)
    set(UTILITIES_LIBRARY_EXPORT_COMMAND EXPORT utilitiesConfig)
else(GMLC_UTILITIES_WITH_CMAKE_PACKAGE)
    set(UTILITIES_LIBRARY_EXPORT_COMMAND)
endif(GMLC_UTILITIES_WITH_CMAKE_PACKAGE)

add_library(utilities_base INTERFACE)

include(compiler_flags)
message(STATUS "setting version build options to \"${CXX_STANDARD_FLAG}\"")
if(${CXX_STANDARD_FLAG})
  if(MSVC)
		target_compile_options(utilities_base INTERFACE ${CXX_STANDARD_FLAG})
	else()
    target_compile_options(
      utilities_base
      INTERFACE $<$<COMPILE_LANGUAGE:CXX>:${CXX_STANDARD_FLAG}}>
    )
  endif(MSVC)
endif(${CXX_STANDARD_FLAG})


option(GMLC_UTILITIES_INCLUDE_BOOST "Enable some boost library headers to be used" ON)
# Add boost to test boost::optional if available
find_package(Boost 1.61)
if(Boost_FOUND)
	message(STATUS "Boost dir= ${Boost_INCLUDE_DIRS}")
   target_include_directories(utilities_base SYSTEM INTERFACE $<BUILD_INTERFACE:${Boost_INCLUDE_DIRS}>)
    target_compile_definitions(utilities_base INTERFACE USE_BOOST_OPTIONAL)
	target_compile_definitions(utilities_base INTERFACE ENABLE_BOOST_TYPES)

endif()

install(TARGETS utilities_base ${UTILITIES_LIBRARY_EXPORT_COMMAND})



if (UTILITIES_MASTER_PROJECT)
option(GMLC_UTILITIES_TEST "Enable tests for the containers library" ON)
set(GMLC_UTILITIES_OBJECT_LIB OFF)
set(GMLC_UTILITIES_STATIC_LIB ON)
else()
set(GMLC_UTILITIES_TEST OFF)
option(GMLC_UTILITIES_OBJECT_LIB "Enable construction of the utilities object library" OFF)
option(GMLC_UTILITIES_STATIC_LIB "enable Construction of the utilities static library" ON)
endif()

add_subdirectory(gmlc)

if (GMLC_UTILITIES_TEST)
	enable_testing()
	if (BUILD_TESTING)
	   add_subdirectory(tests)
	endif()
endif()



# -------------------------------------------------------------
# Get some configuration for C++17 as that becomes available
# -------------------------------------------------------------
# message(STATUS ${CMAKE_CXX_FLAGS})

option(GENERATE_DOXYGEN_DOC "Generate Doxygen doc target" OFF)

if(GENERATE_DOXYGEN_DOC)
	find_package(Doxygen)
	if(DOXYGEN_FOUND)

    show_variable(
      DOXYGEN_OUTPUT_DIR
      PATH
      "location to put Doxygen docs"
      "${PROJECT_BINARY_DIR}/docs"
    )
    configure_file(
      ${CMAKE_CURRENT_SOURCE_DIR}/config/Doxyfile.in
      ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY
    )
    add_custom_target(
      doc ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
			WORKING_DIRECTORY ${DOXYGET_OUTPUT_DIR}
      COMMENT "Generating API documentation with Doxygen"
      VERBATIM
		)
	endif(DOXYGEN_FOUND)
endif(GENERATE_DOXYGEN_DOC)


