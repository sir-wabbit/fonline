/********************************************************************
	author:	Oleg Mareskin
*********************************************************************/

#include "stdafx.h"

#include "CFileMngr.h"


//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-//

char pathlst[][50]=
{
	"maps\\",
	"maps\\wm_mask\\",
};


CFileMngr::Init()
{
	if(crtd) return 1;

	LogExecStr("CFileMngr Initialization...\n");

	LogExecStr("CFileMngr Initialization complete\n");
	crtd=1;
	return 1;
}

void CFileMngr::Clear()
{
	LogExecStr("CFileMngr Clear...\n");
	UnloadFile();
	LogExecStr("CFileMngr Clear complete\n");
}

void CFileMngr::UnloadFile()
{
	SAFEDELA(buf);
}

void CFileMngr::LoadSimple(HANDLE hFile)
{
  	fsize = GetFileSize(hFile,NULL);
	buf = new BYTE[fsize+1];
	DWORD br;
	ReadFile(hFile,buf,fsize,&br,NULL);
	CloseHandle(hFile);	

	buf[fsize]=0;

	cur_pos=0;
}

int CFileMngr::LoadFile(char* fname, int PathType)
{
		if(!crtd)
		{
			LogExecStr("FileMngr LoadFile","FileMngr не был иницилазирован до загрузки файла %s",fname);
			return 0;
		}
		UnloadFile();

		char path[1024]="";
		char pfname[1024]="";

		strcpy(pfname,pathlst[PathType]);
		strcat(pfname,fname);

		strcpy(path,fo_dat);
		strcat(path,pfname);
	
		WIN32_FIND_DATA fd;
		HANDLE f=FindFirstFile(path,&fd);
		if(f!=INVALID_HANDLE_VALUE)
		{
			HANDLE hFile=CreateFile(path,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_FLAG_SEQUENTIAL_SCAN,NULL);
			if(hFile!=INVALID_HANDLE_VALUE)
			{
				LoadSimple(hFile);
				return 1;
			}
		}
		FindClose(f);

//		LogExecStr("FileMngr LoadFile - Файл %s не найден",pfname);//!Cvet
		return 0;
}

void CFileMngr::SetCurPos(DWORD pos)
{
	if(pos<fsize) cur_pos=pos;
}

void CFileMngr::GoForward(DWORD offs)
{
	if((cur_pos+offs)<fsize) cur_pos+=offs;
}


int CFileMngr::GetStr(char* str,DWORD len)
{
	if(cur_pos>=fsize) return 0;

	int rpos=cur_pos;
	int wpos=0;
	for(;wpos<len && rpos<fsize;rpos++)
	{
		if(buf[rpos]==0xD)
		{
			rpos+=2;
			break;
		}

		str[wpos++]=buf[rpos];
	}
	str[wpos]=0;
	cur_pos=rpos;

	return 1;
}

int CFileMngr::CopyMem(void* ptr, size_t size)
{
	if(cur_pos+size>fsize) return 0;

	memcpy(ptr,buf+cur_pos,size);

	cur_pos+=size;

	return 1;
}

BYTE CFileMngr::GetByte() //!Cvet
{
	if(cur_pos>=fsize) return 0;
	BYTE res=0;

	res=buf[cur_pos++];

	return res;
}

WORD CFileMngr::GetWord()
{
	if(cur_pos>=fsize) return 0;
	WORD res=0;

	BYTE *cres=(BYTE*)&res;
	cres[1]=buf[cur_pos++];
	cres[0]=buf[cur_pos++];

	return res;
}

WORD CFileMngr::GetRWord() //!Cvet
{
	if(cur_pos>=fsize) return 0;
	WORD res=0;

	BYTE *cres=(BYTE*)&res;
	cres[0]=buf[cur_pos++];
	cres[1]=buf[cur_pos++];

	return res;
}

DWORD CFileMngr::GetDWord()
{
	if(cur_pos>=fsize) return 0;
	DWORD res=0;
	BYTE *cres=(BYTE*)&res;
	for(int i=3;i>=0;i--)
	{
		cres[i]=buf[cur_pos++];
	}

	return res;
}

DWORD CFileMngr::GetRDWord() //!Cvet
{
	if(cur_pos>=fsize) return 0;
	DWORD res=0;
	BYTE *cres=(BYTE*)&res;
	for(int i=0;i<=3;i++)
	{
		cres[i]=buf[cur_pos++];
	}

	return res;
}

int CFileMngr::GetFullPath(char* fname, int PathType, char* get_path) //!Cvet полный путь к файлу
{
	get_path[0]=0;
	strcpy(get_path,fo_dat);
	strcat(get_path,pathlst[PathType]);
	strcat(get_path,fname);

	WIN32_FIND_DATA fd;
	HANDLE f=FindFirstFile(get_path,&fd);

	if(f!=INVALID_HANDLE_VALUE) return 1;

//	LogExecStr("Файл:|%s|\n",get_path);

	return 0;
}