#include "ErrorHandling.hpp"

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <assert.h>

#ifdef _WIN32
#include <dxerr8.h>

namespace {

VOID ToUTF16(LPCSTR lpText, LPWSTR lpWText, SIZE_T szWText) {
  assert(lpText != NULL);
  assert(lpWText != NULL);

  size_t szText = lstrlenA(lpText);
  size_t wCaptionLen = MultiByteToWideChar(CP_UTF8, 0, lpText, szText, lpWText, szWText / sizeof(wchar_t) - 1);
  lpWText[wCaptionLen] = 0;
}

char* ToUTF8(LPCWSTR lpWText) {
  assert(lpWText != NULL);

  int iSize = WideCharToMultiByte(CP_UTF8, 0, lpWText, -1, NULL, 0, NULL, NULL);
  if (iSize == 0) {
    return NULL;
  }

  char* result = (char*) malloc(iSize + 1);

  iSize = WideCharToMultiByte(CP_UTF8, 0, lpWText, -1, result, iSize, NULL, NULL);
  if (iSize == 0) {
    return NULL;
  }

  return result;
}

int WINAPI MessageBoxU(HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType) {
  assert(lpCaption != NULL);
  assert(lpText != NULL);

  wchar_t wCaption[2048];
  wchar_t wText[2048];
  ToUTF16(lpCaption, wCaption, sizeof(wCaption));
  ToUTF16(lpText, wText, sizeof(wText));
  return MessageBoxW(hWnd, wText, wCaption, uType);
}

char* FormatDirectXError(HRESULT errorCode) {
  LPCWSTR lpMsgBuf = DXGetErrorString8W(errorCode);

  char* result = ToUTF8(lpMsgBuf);

  if (result == NULL) {
    return strdup("Could not convert error message to UTF8.");
  }

  return result;
}

char* FormatWin32Error(DWORD errCode) {
  LPWSTR lpMsgBuf;

  BOOL bOk = FormatMessageW(
    FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
    NULL,
    errCode,
    MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
    (LPWSTR) &lpMsgBuf,
    0,
    NULL
    );

  if (!bOk) {
    return strdup("Could not format error message.");
  }

  LPWSTR p = wcschr(lpMsgBuf, L'\r');
  if(p != NULL) {
    *p = L'\0';
  }

  char* result = ToUTF8(lpMsgBuf);

  if (result == NULL) {
    return strdup("Could not convert error message to UTF8.");
  }

  return result;
}

char* FormatLastWin32Error() {
  DWORD dwError = ::GetLastError();
  char* result = FormatWin32Error(dwError);
  ::SetLastError(dwError);
  return result;
}

}  // anonymous namespace
#endif  // _WIN32

char* FormatStdCError(int errorCode) {
  return strdup(strerror(errorCode));
}