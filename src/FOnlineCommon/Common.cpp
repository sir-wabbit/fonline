#include "Common.hpp"

#include <stdio.h>
#include <stdarg.h>
#include <assert.h>
#include <ctype.h>

#include <Windows.h>

namespace {

// A workaround for this bug:
// http://connect.microsoft.com/VisualStudio/feedback/details/646050/prolem-with-chvalidator-in-ictype-c
int _isspace(char ch) {
  if ((unsigned) ch > 127) {
    return false;
  } else {
    return ::isspace(ch);
  }
}

VOID ToUTF16(LPCSTR lpText, LPWSTR lpWText, SIZE_T szWText) {
  assert(lpText != NULL);
  assert(lpWText != NULL);

  size_t szText = lstrlenA(lpText);
  size_t wCaptionLen = MultiByteToWideChar(CP_UTF8, 0, lpText, szText, lpWText, szWText / sizeof(wchar_t) - 1);
  lpWText[wCaptionLen] = 0;
}
}

void __cdecl odprintf(const char* format, ...) {
  assert(format != NULL);

  va_list	args;
  int len;

  va_start(args, format);
  len = _vscprintf(format, args);

  if (len > 0) {
    PSTR buf;
    len += (1 + 2);
    buf = (PSTR) malloc(len);
    if (buf) {
      len = vsprintf_s(buf, len, format, args);
      if (len > 0) {
        while (len && _isspace(buf[len-1])) len--;
        buf[len++] = '\r';
        buf[len++] = '\n';
        buf[len] = 0;
        OutputDebugStringA(buf);
      }
      free(buf);
    }
    va_end(args);
  }
}
void __cdecl wodprintf(const wchar_t* format, ...) {
  assert(format != NULL);

  va_list	args;
  int len;

  va_start(args, format);
  len = _vscwprintf(format, args);

  if (len > 0) {
    PWSTR buf;
    len += (1 + 2);
    buf = (PWSTR) malloc(sizeof(WCHAR)*len);
    if (buf) {
      len = vswprintf_s(buf, len, format, args);
      if (len > 0) {
        while (len && iswspace(buf[len-1])) len--;
        buf[len++] = L'\r';
        buf[len++] = L'\n';
        buf[len] = 0;
        OutputDebugStringW(buf);
      }
      free(buf);
    }
    va_end(args);
  }
}

void __cdecl uodprintf(const char* format, ...) {
  assert(format != NULL);
  
  va_list	args;
  int len;

  va_start(args, format);
  len = _vscprintf(format, args);

  if (len > 0) {
    PSTR buf;
    PWSTR wbuf;
    size_t sz;
    
    len += (1 + 2);
    sz = len;    
    buf = (PSTR) malloc(sz);
    wbuf = (PWSTR) malloc(sz * sizeof(wchar_t));
    
    if (buf && wbuf) {
      len = vsprintf_s(buf, len, format, args);
      if (len > 0) {
        while (len && _isspace(buf[len-1])) len--;
        buf[len++] = '\r';
        buf[len++] = '\n';
        buf[len] = 0;
        
        ToUTF16(buf, wbuf, sz);
        OutputDebugStringW(wbuf);
      }
    }
    
    if (wbuf != NULL) {
      ::free(wbuf);
      wbuf = NULL;
    }
    
    if (buf != NULL) {
      ::free(buf);
      buf = NULL;
    }
    
    va_end(args);
  }
}

FILE* logFile = NULL;

int OpenLogFile(const char* fileName) {
  assert(fileName != NULL);

  logFile = fopen(fileName, "w+b");

  if (logFile == NULL) {
    return 0;
  }

  return 1;
}

void CloseLogFile() {
  if (logFile != NULL) {
    fclose(logFile);
    logFile = NULL;
  }
}

namespace {

const char* strrchrn(const char* str, const char* chr) {
  const char* p = str;
  const char* l = NULL;
  
  while (*p) {
    if (strchr(chr, *p)) {
      l = p;
    }
    p++;
  }
  
  return l;
}
char* strrchrn(char* str, const char* chr) {
  // XXX[20.12.2012 alex]: a necessary evil
  return (char*) strrchrn((const char*) str, chr);
}

}

void WriteLogFull(const char* file, unsigned int line, const char* func, const char* fmt, ...) {
  assert(file != NULL);
  assert(func != NULL);
  assert(fmt != NULL);

  if (logFile == NULL) {
    return;
  }
  
  const char* fileName = strrchrn(file, "\\/");
  if (fileName == NULL) {
    fileName = file;
  } else {
    fileName += 1;
  }

  char buf[2048];

  va_list list;

  va_start(list, fmt);
  vsnprintf(buf, sizeof(buf) - 1, fmt, list);
  va_end(list);

  fprintf(logFile, "%s[%u] %s -> %s\n", fileName, line, func, buf);
  odprintf("%s[%u] %s -> %s\n", fileName, line, func, buf);

  fflush(logFile);

}

void WriteLog(const char* fmt, ...) {
  assert(fmt != NULL);

  if (logFile == NULL) {
    return;
  }

  char buf[2048];

  va_list list;

  va_start(list, fmt);
  vsnprintf(buf, sizeof(buf) - 1, fmt, list);
  va_end(list);

  fprintf(logFile, "%s", buf);
  odprintf("%s", buf);
  
  fflush(logFile);
  
}