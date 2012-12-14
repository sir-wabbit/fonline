#ifndef FO_BASE_BUFFER_HPP_
#define FO_BASE_BUFFER_HPP_

#include "Common.hpp"
#include <stdint.h>

namespace fonline {

class Buffer {
public:
	FONLINE_COMMON_API Buffer();
	FONLINE_COMMON_API Buffer(size_t alen);
	FONLINE_COMMON_API ~Buffer();

	FONLINE_COMMON_API void Reset();
	FONLINE_COMMON_API void Write(const void* buf, size_t alen);
	FONLINE_COMMON_API void Read(void *buf, size_t alen);
	
	// XXX[1.8.2012 alex]: new names
	FONLINE_COMMON_API bool IsError() {return error;}
	FONLINE_COMMON_API bool NeedProcess() {return (readPosition<writePosition);}
	
	FONLINE_COMMON_API void EnsureCapacity(size_t capacity);
	FONLINE_COMMON_API void EnsureWriteCapacity(size_t dataSize);

  template<class T> Buffer& operator<<(const T& value) {
    // XXX[1.8.2012 alex]: endianness?
    Write(&value, sizeof(value));
    return *this;
  }
  template<class T> Buffer& operator>>(T& value) {
    // XXX[1.8.2012 alex]: endianness?
    Read(&value, sizeof(value));
    return *this;
  }

  bool error;
	char* data;
	size_t capacity;
	size_t writePosition;
	size_t readPosition;
};

}; // namespace fonline

#endif//FO_BASE_BUFFER_HPP_