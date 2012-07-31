#ifndef __BUFMNGR_H__
#define __BUFMNGR_H__

#include <platform.hpp>

class Buffer
{
private:
	bool error;
public:
	Buffer();
	Buffer(size_t alen);
	~Buffer();

	void reset();
	void push(char *buf, size_t alen);
	void pop(char *buf, size_t alen);
	
	bool IsError() {return error;}
	bool NeedProcess() {return (read_pos<pos);}
	
	void grow_buf(size_t alen);

	Buffer &operator<<(int i);
	Buffer &operator>>(int &i);

	Buffer &operator<<(DWORD i);
	Buffer &operator>>(DWORD &i);

	Buffer &operator<<(WORD i);
	Buffer &operator>>(WORD &i);

	Buffer &operator<<(BYTE i);
	Buffer &operator>>(BYTE &i);

	Buffer &operator<<(char *i);
	Buffer &operator>>(char *i);


	char* data;
	size_t len;
	size_t pos;
	size_t read_pos;
};


#endif //__BUFMNGR_H__