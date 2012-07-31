#ifndef FONLINE_BASE_FO_BASE_HPP_
#define FONLINE_BASE_FO_BASE_HPP_

#ifdef _WIN32
  #ifdef FO_BASE_DLL
    #define FO_BASE_DECL __declspec(dllexport)
  #else
    #define FO_BASE_DECL __declspec(dllimport)
  #endif
#else
  #define FO_BASE_DECL 
#endif

#endif//FONLINE_BASE_FO_BASE_HPP_