#ifndef FO_BASE_BUFFER_HPP_
#define FO_BASE_BUFFER_HPP_

#include <Windows.h>
#include "fo-base.hpp"
#include "platform.hpp"

namespace fonline {

class Buffer {
public:
	FO_BASE_DECL Buffer();
	FO_BASE_DECL Buffer(size_t alen);
	FO_BASE_DECL ~Buffer();

	FO_BASE_DECL void Reset();
	FO_BASE_DECL void Write(const void* buf, size_t alen);
	FO_BASE_DECL void Read(void *buf, size_t alen);
	
	// XXX[1.8.2012 alex]: new names
	FO_BASE_DECL bool IsError() {return error;}
	FO_BASE_DECL bool NeedProcess() {return (readPosition<writePosition);}
	
	FO_BASE_DECL void EnsureCapacity(size_t capacity);
	FO_BASE_DECL void EnsureWriteCapacity(size_t dataSize);

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