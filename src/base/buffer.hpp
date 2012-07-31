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

	FO_BASE_DECL void reset();
	FO_BASE_DECL void push(char *buf, size_t alen);
	FO_BASE_DECL void pop(char *buf, size_t alen);
	
	FO_BASE_DECL bool IsError() {return error;}
	FO_BASE_DECL bool NeedProcess() {return (read_pos<pos);}
	
	FO_BASE_DECL void grow_buf(size_t alen);

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
	size_t len;
	size_t pos;
	size_t read_pos;
};

}; // namespace fonline

#endif//FO_BASE_BUFFER_HPP_