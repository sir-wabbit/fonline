#include "buffer.hpp"

namespace fonline {

Buffer::Buffer()
{
	len=2048;
	pos=0;
	read_pos=0;
	data=new char[len];
	memset(data,0,len);
	error=0;
}

Buffer::Buffer(size_t alen)
{
	len=alen;
	pos=0;
	read_pos=0;
	data = reinterpret_cast<char*>(::malloc(len));
	memset(data,0,len);
	error=0;
}

Buffer::~Buffer()
{
	if (data != NULL) {
	  ::free(data);
	}
}

void Buffer::reset()
{
	pos=0;
	read_pos=0;
	memset(data,0,len);
}

void Buffer::grow_buf(size_t alen)
{
		while(pos+alen>=len) len *= 2;
    char* newBuf = reinterpret_cast<char*>(::malloc(len));
		if (data != NULL) {
		  ::memcpy(newBuf,data,pos);
		  ::free(data);
		}
		data=newBuf;
}

void Buffer::push(char *buf, size_t alen)
{
	if(error) return;

	if(pos+alen>=len) grow_buf(alen);
	memcpy(data+pos,buf,alen);
	pos+=alen;
}

void Buffer::pop(char *buf, size_t alen)
{
	if(error) return;

	if(read_pos+alen>pos) {error=1;return;}

	memcpy(buf,data+read_pos,alen);
	read_pos+=alen;
}


Buffer &Buffer::operator<<(int i)
{
	if(error) return *this;

	if(pos+4>=len) grow_buf(4);
	*(int*)(data+pos)=i;
	pos+=4;
	return *this;
}

Buffer &Buffer::operator>>(int &i)
{
	if(error) return *this;

  // XXX[1.8.2012 alex]: old xxx
	//#pragma chMSG("Необходимо позже сделать чтобы при недостатке данных он попытался их прочитать несколько раз с некоторым интервалом и только потом выдавать ошибку")
	if(read_pos+4>pos) {error=1;return *this;}
	i=*(int*)(data+read_pos);
	read_pos+=4;

	return *this;
}

Buffer &Buffer::operator<<(DWORD i)
{
	if(error) return *this;

	if(pos+4>=len) grow_buf(4);
	*(DWORD*)(data+pos)=i;
	pos+=4;
	return *this;
}

Buffer &Buffer::operator>>(DWORD &i)
{
	if(error) return *this;

	if(read_pos+4>pos) {error=1;return *this;}
	i=*(DWORD*)(data+read_pos);
	read_pos+=4;

	return *this;
}

Buffer &Buffer::operator<<(WORD i)
{
	if(error) return *this;

	if(pos+2>=len) grow_buf(2);
	*(WORD*)(data+pos)=i;
	pos+=2;
	return *this;
}

Buffer &Buffer::operator>>(WORD &i)
{
	if(error) return *this;

	if(read_pos+2>pos) {error=1;return *this;}
	i=*(WORD*)(data+read_pos);
	read_pos+=2;

	return *this;
}

Buffer &Buffer::operator<<(BYTE i)
{
	if(error) return *this;

	if(pos+1>=len) grow_buf(1);
	*(BYTE*)(data+pos)=i;
	pos+=1;
	return *this;
}

Buffer &Buffer::operator>>(BYTE &i)
{
	if(error) return *this;

	if(read_pos+1>pos) {error=1;return *this;}
	i=*(BYTE*)(data+read_pos);
	read_pos+=1;

	return *this;
}

}; // namespace fonline