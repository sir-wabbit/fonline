# Copyright (c) 2013 Alexander Konovalov. All rights reserved.
# Use of this source code is governed by a GPL-style license that can be
# found in the LICENSE file.

project(FOnlineOpenSource)
cmake_minimum_required(VERSION 2.6)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMake")

if ("${CMAKE_BUILD_TYPE}" STREQUAL "")
  set(CMAKE_BUILD_TYPE Debug)
endif()

if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
endif()

if("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
  include_directories("inc")
  link_directories(${CMAKE_SOURCE_DIR}/lib)
endif()

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wno-strict-aliasing")

###############################################################################

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})

set(modules
  IniFile
  LZSS
  ACMDecompressor
  DatArchive
  FOnlineFileManager
  FOnlineCommon
  FOnlineServer
)

foreach (module ${modules})
  message(STATUS " * Preparing ${module} module...")
  add_subdirectory(src/${module})
endforeach (module)

add_custom_target(build COMMAND make)
