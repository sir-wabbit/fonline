#ifndef FONLINE_BASE_FO_BASE_HPP_
#define FONLINE_BASE_FO_BASE_HPP_

#if defined(_WIN32) && defined(FONLINE_COMMON_DYNAMIC)
  #ifdef FONLINE_COMMON_EXPORTS
    #define FO_BASE_DECL __declspec(dllexport)
  #else
    #define FO_BASE_DECL __declspec(dllimport)
  #endif
#else
  #define FO_BASE_DECL 
#endif

#endif//FONLINE_BASE_FO_BASE_HPP_