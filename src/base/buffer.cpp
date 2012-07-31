#include "buffer.hpp"

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


Buffer &Buffer::operator<<(int value) {
	if(error) return *this;
	Write(&value, sizeof(value));
	return *this;
}

Buffer &Buffer::operator>>(int &i)
{
	if(error) return *this;

  // XXX[1.8.2012 alex]: old xxx
	//#pragma chMSG("Необходимо позже сделать чтобы при недостатке данных он попытался их прочитать несколько раз с некоторым интервалом и только потом выдавать ошибку")
	if(readPosition+4>writePosition) {error=1;return *this;}
	i=*(int*)(data+readPosition);
	readPosition+=4;

	return *this;
}

Buffer &Buffer::operator<<(DWORD value)
{
  if(error) return *this;
  Write(&value, sizeof(value));
  return *this;
}

Buffer &Buffer::operator>>(DWORD &i)
{
	if(error) return *this;

	if(readPosition+4>writePosition) {error=1;return *this;}
	i=*(DWORD*)(data+readPosition);
	readPosition+=4;

	return *this;
}

Buffer &Buffer::operator<<(WORD value)
{
  if(error) return *this;
  Write(&value, sizeof(value));
  return *this;
}

Buffer &Buffer::operator>>(WORD &i)
{
	if(error) return *this;

	if(readPosition+2>writePosition) {error=1;return *this;}
	i=*(WORD*)(data+readPosition);
	readPosition+=2;

	return *this;
}

Buffer &Buffer::operator<<(BYTE value)
{
  if(error) return *this;
  Write(&value, sizeof(value));
  return *this;
}

Buffer &Buffer::operator>>(BYTE &i)
{
	if(error) return *this;

	if(readPosition+1>writePosition) {error=1;return *this;}
	i=*(BYTE*)(data+readPosition);
	readPosition+=1;

	return *this;
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