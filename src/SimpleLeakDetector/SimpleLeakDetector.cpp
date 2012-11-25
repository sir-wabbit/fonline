#define SIMPLELEAKDETECTOR_PRIVATE
#include "SimpleLeakDetector.hpp"
#undef SIMPLELEAKDETECTOR_PRIVATE

#include <cassert>
#include <vector>
#include <iostream>

namespace SimpleLeakDetector {

struct AllocInfo {
  bool array;
  size_t size;
  const char* file;
  unsigned int line;    
  const void* ptr;
};
std::vector<AllocInfo> allocations;

SLD_API void* Allocate(size_t size, bool array, const char* file, unsigned int line) {
  void* result;
  if (array) {
    result = ::operator new[](size);
  } else {
    result = ::operator new(size);
  }

  AllocInfo info;
  info.array = array;
  info.size = size;
  info.file = file;
  info.line = line;
  info.ptr = result;
  allocations.push_back(info);

  return result;
}
SLD_API void Free(void* ptr, bool array, const char* file, unsigned int line) {
  typedef std::vector<AllocInfo>::iterator It;
  for (It i = allocations.begin(); i != allocations.end(); ++i) {
    if (i->ptr == ptr) {
      assert(i->array == array);
      allocations.erase(i);
      break;
    }
  }
  
  if (array) {
    ::operator delete[](ptr);
  } else {
    ::operator delete(ptr);
  }
}
SLD_API void PrintAllLeaks() {
  typedef std::vector<AllocInfo>::const_iterator It;
  std::cout << "Leaks : " << std::endl;
  for (It i = allocations.begin(); i != allocations.end(); ++i) {
    std::cout << "Leak #" << i->ptr << std::endl <<
      "  array   : " << i->array << std::endl <<
      "  size    : " << i->size << std::endl <<
      "  file    : " << i->file << std::endl <<
      "  line    : " << i->line << std::endl; 
  }
}

} // namespace SimpleLeakDetector