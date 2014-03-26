# Copyright (c) 2013 Alexander Konovalov. All rights reserved.
# Use of this source code is governed by a GPL-style license that can be
# found in the LICENSE file.

project(FOnlineCommon)

file(GLOB LIB_HXX
  "*.hpp"
  "math/*.hpp"
)

file(GLOB LIB_CXX
  "*.cpp"
  "math/*.cpp"
)

# List all the *_test.cpp files in the alphabetic order.
set(TEST_CXX

)

add_library(FOnlineCommon ${LIB_CXX} ${LIB_HXX})
IF("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DFONLINE_COMMON_EXPORTS")
ENDIF()
