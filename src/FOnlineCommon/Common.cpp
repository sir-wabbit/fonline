#include "Common.hpp"

#include <stdio.h>
#include <stdarg.h>
#include <assert.h>

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

void WriteLog(char* fmt, ...) {
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
  fflush(logFile);
}