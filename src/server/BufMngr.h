#ifndef __BUFMNGR_H__
#define __BUFMNGR_H__

/********************************************************************
	created:	06:01:2005   11:17

	author:		Oleg Mareskin
	
	purpose:	
*********************************************************************/

class CBufMngr
{
private:
	bool error;
public:
	CBufMngr();
	CBufMngr(DWORD alen);
	~CBufMngr();

	void reset();
	void push(char *buf,DWORD alen);
	void pop(char *buf,DWORD alen);
	
	bool IsError() {return error;}
	bool NeedProcess() {return (read_pos<pos);}
	
	void grow_buf(DWORD alen);

	CBufMngr &operator<<(int i);
	CBufMngr &operator>>(int &i);

	CBufMngr &operator<<(DWORD i);
	CBufMngr &operator>>(DWORD &i);

	CBufMngr &operator<<(WORD i);
	CBufMngr &operator>>(WORD &i);

	CBufMngr &operator<<(BYTE i);
	CBufMngr &operator>>(BYTE &i);

	CBufMngr &operator<<(char *i);
	CBufMngr &operator>>(char *i);


	char* data;
	DWORD len;
	DWORD pos;
	DWORD read_pos;
};


#endif //__BUFMNGR_H__