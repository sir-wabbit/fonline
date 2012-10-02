#include "buffer.hpp"
#include <stdlib.h>
#include <string.h>

namespace fonline {

Buffer::Buffer()
{
	capacity=2048;
	writePosition=0;
	readPosition=0;
	data=new char[capacity];
	::memset(data,0,capacity);
	error=0;
}

Buffer::Buffer(size_t alen)
{
	capacity=alen;
	writePosition=0;
	readPosition=0;
	data = reinterpret_cast<char*>(::malloc(capacity));
	::memset(data,0,capacity);
	error=0;
}

Buffer::~Buffer()
{
	if (data != NULL) {
	  ::free(data);
	}
}

void Buffer::Reset()
{
	writePosition=0;
	readPosition=0;
	::memset(data,0,capacity);
}

void Buffer::Write(const void* buf, size_t alen)
{
	if(error) return;

	EnsureCapacity(writePosition + alen);
	::memcpy(data+writePosition,buf,alen);
	writePosition+=alen;
}

void Buffer::Read(void *buf, size_t alen)
{
	if(error) return;

	if(readPosition+alen>writePosition) {error=1;return;}

	::memcpy(buf,data+readPosition,alen);
	readPosition+=alen;
}

void Buffer::EnsureCapacity(size_t capacity) {
  if (capacity > this->capacity) {
    size_t newCapacity = 3 * capacity / 2;
    char* newData = reinterpret_cast<char*>(::malloc(newCapacity));
    
    if (this->data != NULL) {
      ::memcpy(newData, this->data, this->capacity);
      ::free(this->data);
    }
    
    this->data = newData;
    this->capacity = newCapacity;
  }
}
void Buffer::EnsureWriteCapacity(size_t dataSize) {
  EnsureCapacity(this->writePosition + dataSize);
}

}; // namespace fonline