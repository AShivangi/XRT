# SPDX-License-Identifier: Apache-2.0
# Copyright (C) 2018-2022 Xilinx, Inc. All rights reserved.
# Copyright (C) 2022 Advanced Micro Devices, Inc. All rights reserved.
cmake_minimum_required(VERSION 3.5.0)
project(XRT)
set(PROJECT_DESCRIPTION "https://github.com/Xilinx/XRT")

# Include utility functions
include(CMake/utilities.cmake)

# Include supporting CMake functions
include(CMake/unitTestSupport.cmake)

# Exported interface into XRT for include from implemenation
include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/runtime_src
  ${CMAKE_CURRENT_SOURCE_DIR}/runtime_src/core/include
  ${CMAKE_CURRENT_SOURCE_DIR}/runtime_src/core/common/gsl/include
  ${XRT_BINARY_DIR}/gen
  ${XRT_BINARY_DIR}
  )

if (NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
endif (NOT CMAKE_CXX_STANDARD)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

message("-- Host system processor is ${CMAKE_HOST_SYSTEM_PROCESSOR}")
message("-- Target system processor is ${CMAKE_SYSTEM_PROCESSOR}")

set(XRT_NATIVE_BUILD "yes")
if (NOT ${CMAKE_SYSTEM_PROCESSOR} STREQUAL ${CMAKE_HOST_SYSTEM_PROCESSOR})
  set(XRT_NATIVE_BUILD "no")
endif()

if ( ${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang" )
  set(XRT_WARN_OPTS
  -Wall
  -Wno-mismatched-tags
  -Wno-unused-const-variable
  -Wno-unused-private-field
  -Wno-missing-braces
  -Wno-self-assign
  -Wno-pessimizing-move
  -Wno-unused-function
  -Wno-unused-variable
  -Wno-parentheses
  # These are dangerous and should be reviewed
  -Wno-overloaded-virtual
   )
else()
  set(XRT_WARN_OPTS -Wall)
endif()

if (DEFINED ENV{XRT_NATIVE_BUILD})
  set(XRT_NATIVE_BUILD $ENV{XRT_NATIVE_BUILD})
endif()

if (DEFINED ENV{XRT_CLANGTIDY_REVIEW})
  set(XRT_CLANGTIDY_REVIEW "yes")
endif()

if (DISABLE_ABI_CHECK)
  add_compile_options("-DDISABLE_ABI_CHECK")
endif()

if (NOT CMAKE_BUILD_TYPE)
  set (CMAKE_BUILD_TYPE RelWithDebInfo)
endif (NOT CMAKE_BUILD_TYPE)

# Default component name for any install() command without the COMPONENT argument
# The default component is the xrt run-time component, if XRT_DEV_COMPONENT is
# set to something different then a development component will be created with
# link libraries and header which are then excluded from runtime component
set (CMAKE_INSTALL_DEFAULT_COMPONENT_NAME "xrt")

# Enable development package by specifying development component name
# If XRT_DEV_COMPONENT is same DEFAULT_COMPONENT then only that package
# is created with both development and run-time content.
#set (XRT_DEV_COMPONENT "xrt-dev")
set (XRT_DEV_COMPONENT "xrt")

# Version adjusted to 2.18 for 2024.2
set(XRT_VERSION_RELEASE 202420)
SET(XRT_VERSION_MAJOR 2)
SET(XRT_VERSION_MINOR 18)

if (DEFINED ENV{XRT_VERSION_PATCH})
  SET(XRT_VERSION_PATCH $ENV{XRT_VERSION_PATCH})
else(DEFINED $ENV{XRT_VERSION_PATCH})
  SET(XRT_VERSION_PATCH 0)
endif(DEFINED ENV{XRT_VERSION_PATCH})

# Also update cache to set version for external plug-in .so
set(XRT_SOVERSION ${XRT_VERSION_MAJOR} CACHE INTERNAL "")
set(XRT_VERSION_STRING ${XRT_VERSION_MAJOR}.${XRT_VERSION_MINOR}.${XRT_VERSION_PATCH} CACHE INTERNAL "")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${XRT_SOURCE_DIR}/CMake/")

if (${XRT_NATIVE_BUILD} STREQUAL "yes")
  # Enable testing for this directory and below.  This command should be
  # in the source directory root because ctest expects to find a test
  # file in the build directory root.
  enable_testing()

  # Temporary native wrapper while Linux code is being ported to windows.
  # When completed the two build flows will once again be merged into one
  # common file
  if (WIN32)
    include(CMake/nativeWin.cmake)
  else()
    include(CMake/nativeLnx.cmake)
  endif()
  xrt_include(CMake/nativeTests.cmake)
else()
  include(CMake/embedded_system.cmake)
endif()
