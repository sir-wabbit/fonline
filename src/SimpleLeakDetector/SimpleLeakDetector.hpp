#ifndef SIMPLELEAKDETECTOR_HPP_
#define SIMPLELEAKDETECTOR_HPP_

#include <stddef.h>
#include <stdint.h>

#ifdef _WIN32
  #ifdef SIMPLELEAKDETECTOR_DLL
    #define SLD_API __declspec(dllexport)
  #else
    #define SLD_API __declspec(dllimport)
  #endif
#else
  #define SLD_API
#endif

namespace SimpleLeakDetector {

SLD_API void* Allocate(size_t size, bool array, const char* file, unsigned int line);
SLD_API void Free(void* ptr, bool array, const char* file = 0, unsigned int line = 0);
SLD_API void PrintAllLeaks();

} // namespace SimpleLeakDetector

#ifndef SIMPLELEAKDETECTOR_PRIVATE
inline void* operator new(size_t size, const char* file, unsigned int line) {
  return SimpleLeakDetector::Allocate(size, false, file, line);
}
inline void* operator new[](size_t size, const char* file, unsigned int line) {
  return SimpleLeakDetector::Allocate(size, true, file, line);
}
inline void operator delete(void* ptr, const char* file, unsigned int line) {
  SimpleLeakDetector::Free(ptr, false, file, line);
}
inline void operator delete[](void* ptr, const char* file, unsigned int line) {
  SimpleLeakDetector::Free(ptr, true, file, line);
}
inline void operator delete(void* ptr) {
  SimpleLeakDetector::Free(ptr, false);
}
inline void operator delete[](void* ptr) {
  SimpleLeakDetector::Free(ptr, true);
}

#define new new(__FILE__, __LINE__)
#endif//SIMPLELEAKDETECTOR_PRIVATE

#endif//SIMPLELEAKDETECTOR_HPP_
