#include "stdafx.h"

#include "CFileMngr.h"
#include "common.h"


//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-//

char pathlst[][50]=
{
	"art\\critters\\",
	"art\\intrface\\",
	"art\\inven\\",
	"art\\items\\",
	"art\\misc\\",
	"art\\scenery\\",
	"art\\skilldex\\",
	"art\\splash\\",
	"art\\tiles\\",
	"art\\walls\\",

	"maps\\",

	"proto\\items\\",
	"proto\\misc\\",
	"proto\\scenery\\",
	"proto\\tiles\\",
	"proto\\walls\\",

	"sound\\music\\",
	"sound\\sfx\\",

	"text\\english\\game\\",
};


int CFileMngr::Init()
{
	WriteLog("CFileMngr Initialization...\n");
	master_dat[0]=0;
	if(!opt_masterpath[0])
	{
		ErrMsg("CFileMngr Init","Не найден файл %s или в нем нет раздела master_dat",CFG_FILE);
		return 0;
	}
	if(opt_masterpath[0]=='.') strcat(master_dat,".");
		else if(opt_masterpath[1]!=':') strcat(master_dat,"..\\");
	strcat(master_dat,opt_masterpath);
	if(strstr(master_dat,".dat"))
	{
		lpDAT=new TDatFile(master_dat);	
		if(lpDAT->ErrorType==ERR_CANNOT_OPEN_FILE)
		{
			ErrMsg("CFileMngr Init>","файл %s не найден",master_dat);
			return 0;
		}
	}
	else
	{
		if(master_dat[strlen(master_dat)-1]!='\\') strcat(master_dat,"\\");
	}

	crit_dat[0]=0;
	if(!opt_critterpath[0])
	{
		ErrMsg("CFileMngr Init","Не найден файл %s или в нем нет раздела critter_dat",CFG_FILE);
		return 0;
	}
	if(opt_critterpath[0]=='.') strcat(crit_dat,".");
		else if(opt_critterpath[1]!=':') strcat(crit_dat,"..\\");
	strcat(crit_dat,opt_critterpath);
	if(strstr(crit_dat,".dat"))
	{
		lpDATcr=new TDatFile(crit_dat);		
		if(lpDATcr->ErrorType==ERR_CANNOT_OPEN_FILE)
		{
			ErrMsg("CFileMngr Init>","файл %s не найден",crit_dat);
			return 0;
		}
	}
	else
	{
		if(crit_dat[strlen(crit_dat)-1]!='\\') strcat(crit_dat,"\\");
	}

	if(!opt_fopath[0])
	{
		ErrMsg("CFileMngr Init","Не найден файл %s или в нем нет раздела fonline_dat",CFG_FILE);
		return 0;
	}
	strcpy(fo_dat,opt_fopath);
	if(fo_dat[strlen(fo_dat)-1]!='\\') strcat(fo_dat,"\\");

	WriteLog("CFileMngr Initialization complete\n");
	crtd=1;
	return 1;
}

void CFileMngr::Clear()
{
	WriteLog("CFileMngr Clear...\n");
	UnloadFile();
	SAFEDEL(lpDAT);
	SAFEDEL(lpDATcr);
	WriteLog("CFileMngr Clear complete\n");
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
			ErrMsg("FileMngr LoadFile","FileMngr не был иницилазирован до загрузки файла %s",fname);
			return 0;
		}
		UnloadFile();

		char path[1024]="";
		char pfname[1024]="";

		strcpy(pfname,pathlst[PathType]);
		strcat(pfname,fname);

		strcpy(path,fo_dat);
		strcat(path,pfname);
	
		//данные fo
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

		if(PathType==PT_ART_CRITTERS)
		{
			if(!lpDATcr)
			{
				//попрбуем загрузить из critter_dat если это каталог
				strcpy(path,crit_dat);
				strcat(path,pfname);
		
				HANDLE hFile=CreateFile(path,GENERIC_READ,0,NULL,OPEN_EXISTING,FILE_FLAG_SEQUENTIAL_SCAN,NULL);
				if(hFile!=INVALID_HANDLE_VALUE)
				{
					LoadSimple(hFile);
					return 1;
				}
				else return 0; //а вот не вышло
			}	

			if(lpDATcr->DATOpenFile(pfname)!=INVALID_HANDLE_VALUE)
			{
			
				fsize = lpDATcr->DATGetFileSize();			

				buf = new BYTE[fsize+1];
				DWORD br;
			
				lpDATcr->DATReadFile(buf,fsize,&br);

				buf[fsize]=0;

				cur_pos=0;
				return 1;
			}

		}
		
		if(!lpDAT)
		{
			//попрбуем загрузить из master_dat если это каталог
			strcpy(path,master_dat);
			strcat(path,pfname);
	
			HANDLE hFile=CreateFile(path,GENERIC_READ,0,NULL,OPEN_EXISTING,FILE_FLAG_SEQUENTIAL_SCAN,NULL);
			if(hFile!=INVALID_HANDLE_VALUE)
			{
				LoadSimple(hFile);
				return 1;
			}
			else return 0; //а вот не вышло
		}

		if(lpDAT->DATOpenFile(pfname)!=INVALID_HANDLE_VALUE)
		{
		
			fsize = lpDAT->DATGetFileSize();

			buf = new BYTE[fsize+1];
			DWORD br;
			
			lpDAT->DATReadFile(buf,fsize,&br);

			buf[fsize]=0;

			cur_pos=0;
			return 1;
		}

		WriteLog("FileMngr - Файл %s не найден\n",pfname);//!Cvet
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

	WriteLog("Файл:|%s|\n",get_path);

	return 0;
}