#pragma once

#ifdef _WIN32
  #ifdef DATARCHIVE_DLL
    #define DATARCHIVE_API __declspec(dllexport)
  #else
    #define DATARCHIVE_API __declspec(dllimport)
  #endif
#else
  #define DATARCHIVE_API
#endif

#include "datfile.hpp"
