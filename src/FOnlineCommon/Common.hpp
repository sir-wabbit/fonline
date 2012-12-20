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

#define FONLINE_LOG(fmt, ...) WriteLogFull(__FILE__, __LINE__, __FUNCTION__, fmt, __VA_ARGS__)

FONLINE_COMMON_API int OpenLogFile(const char* fileName);
FONLINE_COMMON_API void CloseLogFile();
FONLINE_COMMON_API void WriteLogFull(const char* file, unsigned int line, const char* func, const char* fmt, ...);
FONLINE_COMMON_API void WriteLog(const char* fmt, ...);
FONLINE_COMMON_API void ReportErrorMessage(const char* hdr, const char* fmt, ...);

#endif//FONLINE_BASE_FO_BASE_HPP_