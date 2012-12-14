#ifndef FONLINE_BASE_FO_BASE_HPP_
#define FONLINE_BASE_FO_BASE_HPP_

#if defined(_WIN32)
  #ifdef FONLINE_COMMON_EXPORTS
    #define FONLINE_COMMON_API __declspec(dllexport)
  #else
    #define FONLINE_COMMON_API __declspec(dllimport)
  #endif
#else
  #define FONLINE_COMMON_API 
#endif

FONLINE_COMMON_API int OpenLogFile(const char* fileName);
FONLINE_COMMON_API void CloseLogFile();
FONLINE_COMMON_API void WriteLog(char* fmt, ...);
FONLINE_COMMON_API void ReportErrorMessage(char* hdr, char* fmt, ...);

#endif//FONLINE_BASE_FO_BASE_HPP_