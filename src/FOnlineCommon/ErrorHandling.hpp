#ifndef FONLINE_COMMON_ERRORHANDLING_HPP_
#define FONLINE_COMMON_ERRORHANDLING_HPP_

#include "Common.hpp"

#include <Windows.h>
#include <errno.h>

FONLINE_COMMON_API char* FormatStdCError(errno_t errorCode);
FONLINE_COMMON_API char* FormatDirectXError(HRESULT errorCode);
FONLINE_COMMON_API char* FormatWin32Error(DWORD errCode);
FONLINE_COMMON_API char* FormatLastWin32Error();

#endif//FONLINE_COMMON_ERRORHANDLING_HPP_