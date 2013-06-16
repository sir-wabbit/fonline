# Copyright (c) 2013 Alexander Konovalov. All rights reserved.
# Use of this source code is governed by a GPL-style license that can be
# found in the LICENSE file.

project(DatArchive)

# List all the .h (header) files in the module in the alphabetic order.
set(LIB_HXX
  DatArchive.hpp
  datfile.hpp
  cfile/cfile.hpp
)

# List all the non-test .cpp files in the alphabetic order.
set(LIB_CXX
  datfile.cpp
  cfile/cfile.cpp
)

# List all the *_test.cpp files in the alphabetic order.
set(TEST_CXX

)

add_library(DatArchive ${LIB_CXX} ${LIB_HXX})
IF("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DDATARCHIVE_DLL")
ENDIF()

include_directories(..)
target_link_libraries(DatArchive LZSS)