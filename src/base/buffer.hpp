#ifndef FO_BASE_BUFFER_HPP_
#define FO_BASE_BUFFER_HPP_

#include <Windows.h>
#include "fo-base.hpp"
#include "platform.hpp"

namespace fonline {

class Buffer
{
private:
	bool error;
public:
	FO_BASE_DECL Buffer();
	FO_BASE_DECL Buffer(size_t alen);
	FO_BASE_DECL ~Buffer();

	FO_BASE_DECL void Reset();
	FO_BASE_DECL void Write(const void* buf, size_t alen);
	FO_BASE_DECL void Read(void *buf, size_t alen);
	
	FO_BASE_DECL bool IsError() {return error;}
	FO_BASE_DECL bool NeedProcess() {return (readPosition<writePosition);}
	
	FO_BASE_DECL void EnsureCapacity(size_t capacity);
	FO_BASE_DECL void EnsureWriteCapacity(size_t dataSize);

	FO_BASE_DECL Buffer &operator<<(int i);
	FO_BASE_DECL Buffer &operator>>(int &i);

	FO_BASE_DECL Buffer &operator<<(DWORD i);
	FO_BASE_DECL Buffer &operator>>(DWORD &i);

	FO_BASE_DECL Buffer &operator<<(WORD i);
	FO_BASE_DECL Buffer &operator>>(WORD &i);

	FO_BASE_DECL Buffer &operator<<(BYTE i);
	FO_BASE_DECL Buffer &operator>>(BYTE &i);

	FO_BASE_DECL Buffer &operator<<(char *i);
	FO_BASE_DECL Buffer &operator>>(char *i);


	char* data;
	size_t capacity;
	size_t writePosition;
	size_t readPosition;
};

}; // namespace fonline

#endif//FO_BASE_BUFFER_HPP_