#ifndef __BUFMNGR_H__
#define __BUFMNGR_H__

/********************************************************************
	created:	06:01:2005   11:17

	author:		Oleg Mareskin
	
	purpose:	
*********************************************************************/

class Buffer
{
private:
	bool error;
public:
	Buffer();
	Buffer(size_t alen);
	~Buffer();

	void reset();
	void push(char *buf,size_t alen);
	void pop(char *buf,size_t alen);
	
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
	DWORD len;
	DWORD pos;
	DWORD read_pos;
};


#endif //__BUFMNGR_H__