#ifndef FONLINE_COMMON_ERRORHANDLING_HPP_
#define FONLINE_COMMON_ERRORHANDLING_HPP_

#include "Common.hpp"

#ifdef _WIN32
  #include <Windows.h>
#endif  // _WIN32

#include <errno.h>

FONLINE_COMMON_API char* FormatStdCError(int errorCode);

#ifdef _WIN32
  FONLINE_COMMON_API char* FormatWin32Error(DWORD errCode);
  FONLINE_COMMON_API char* FormatLastWin32Error();
  FONLINE_COMMON_API char* FormatDirectXError(HRESULT errorCode);
#endif  // _WIN32

#endif//FONLINE_COMMON_ERRORHANDLING_HPP_