#ifndef DATARCHIVE_DATARCHIVE_HPP_
#define DATARCHIVE_DATARCHIVE_HPP_

#ifdef _WIN32
  #ifdef DATARCHIVE_DLL
    #define DATARCHIVE_API __declspec(dllexport)
  #else
    #define DATARCHIVE_API __declspec(dllimport)
  #endif
#else
  #define DATARCHIVE_API
#endif

#include "cfile/cfile.hpp"
#include "datfile.hpp"

#endif//DATARCHIVE_DATARCHIVE_HPP_