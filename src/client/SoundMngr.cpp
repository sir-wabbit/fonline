#include "common.h"
#include "SoundMngr.h"
#include "CFileMngr.h"
//#include <SimpleLeakDetector/SimpleLeakDetector.hpp>

int SoundManager::Init()
{
	if(active==true) return 1;

	WriteLog("SoundManager Init\n");

	if(!fm.Init()) return 0;;

	if(DirectSoundCreate8(0,&lpDS,0)!=DS_OK)
	{
		WriteLog("Неудалось создать устройство!\n");
		return 0;
	}

	if(lpDS->SetCooperativeLevel(GetForegroundWindow(),DSSCL_NORMAL)!=DS_OK)
	{
		WriteLog("Неудалось установить уровень кооперации!\n");
		return 0;
	}

	cur_snd=1;

	active=true;

	WriteLog("SoundManager Init OK\n");

	return 1;
}

void SoundManager::Clear()
{
	if(active==false) return;

	WriteLog("SoundManager Clear...\n");

	fm.Clear();

	lpDS->Release();
	lpDS=NULL;

	for(sound_map::iterator it=sounds.begin(); it!=sounds.end(); it++)
	{
//		(*it).second->buf->Release();
		delete (*it).second;
	}
	sounds.clear();

	active=false;

	WriteLog("OK\n");
}

void SoundManager::LPESound(char* fname, int TypePath)
{
	uint16_t lpe=0;

	if(!(lpe=LoadSound(fname,TypePath))) return;

	PlaySound(lpe);
}

int SoundManager::LoadSound(char* fname, int TypePath)
{
	if (!fm.LoadFile(fname,TypePath))
	{
		WriteLog("Ошибка - Загрузка звука - Звук |%s| не найден\n",fname);	
		return 0;
	}

	char* ext=strstr(fname,".");

	if(!ext)
	{
		fm.UnloadFile();
		WriteLog("Ошибка - Загрузка звука - Нет расширения у файла:|%s|\n",fname);
		return 0;
	}

	WAVEFORMATEX fmt;
	ZeroMemory(&fmt,sizeof(WAVEFORMATEX));

	unsigned char* smplData=NULL;
	uint32_t sizeData=0;

	if(!stricmp(ext,".wav"))
	{
		if (!LoadWAV(&fmt, &smplData, &sizeData)) { 
		  if (smplData != NULL) {
		    delete [] smplData;
		    smplData = NULL;
		  }
		  fm.UnloadFile();
		  return 0;
		}
	}
	else if(!stricmp(ext,".acm"))
	{
		if(!LoadACM(&fmt,&smplData,&sizeData)) {
      if (smplData != NULL) {
        delete [] smplData;
        smplData = NULL;
      }
		  fm.UnloadFile(); 
		  return 0;
		}
	} else if (!stricmp(ext, ".ogg")) {
		char full_path[256];
		if  (!fm.GetFullPath(fname,TypePath,&full_path[0])) { 
      if (smplData != NULL) {
        delete [] smplData;
        smplData = NULL;
      }
		  fm.UnloadFile();
		  return 0;
		}	
		if (!LoadOGG(&fmt,&smplData,&sizeData,&full_path[0])) {
      if (smplData != NULL) {
        delete [] smplData;
        smplData = NULL;
      }
      fm.UnloadFile();
      return 0;
    }
	} else {
		fm.UnloadFile();
		WriteLog("Ошибка - Загрузка звука - Неизвестный формат файла звука |%s|\n",fname);
		return 0;
	}

	fm.UnloadFile();

	DSBUFFERDESC dsbd;
	ZeroMemory(&dsbd,sizeof(DSBUFFERDESC));

	dsbd.dwBufferBytes=sizeData;
	dsbd.dwFlags=DSBCAPS_STATIC;// | DSBCAPS_LOCSOFTWARE |DSSCL_PRIORITY ;
	dsbd.dwSize=sizeof(DSBUFFERDESC);
	dsbd.lpwfxFormat=&fmt;
	dsbd.dwReserved=0;

	Sound* nsnd=new Sound;

	if (lpDS->CreateSoundBuffer(&dsbd,&nsnd->buf,0) != DS_OK) {
    if (smplData != NULL) {
      delete [] smplData;
      smplData = NULL;
    }
		WriteLog("Ошибка - Загрузка звука - Неудалось создать буфер для звука\n");
		return 0;
	}

	void *pDst=0;
	DWORD wSize=0;

	if (nsnd->buf->Lock(0, 0, &pDst, &wSize, 0, 0, DSBLOCK_ENTIREBUFFER) != DS_OK) {
    if (smplData != NULL) {
      delete [] smplData;
      smplData = NULL;
    }
		WriteLog("Ошибка - Загрузка звука - Невозможно заблокировать память\n");
		return 0;
	}

	memcpy(pDst, smplData, wSize);

  if (smplData != NULL) {
    delete [] smplData;
    smplData = NULL;
  }

	nsnd->buf->Unlock(pDst,wSize,0,0);

	cur_snd++;

	sounds.insert(sound_map::value_type(cur_snd,nsnd));

	return cur_snd;
}

int SoundManager::LoadWAV(WAVEFORMATEX* fformat, unsigned char** sample_data, uint32_t* size_data)
{
	uint32_t dw_buf=fm.GetRDWord();

	if(dw_buf!=MAKEFOURCC('R','I','F','F')) { WriteLog("|RIFF| not found |%d|\n"); return 0; }

	fm.GoForward(4);

	dw_buf=fm.GetRDWord();

	if(dw_buf!=MAKEFOURCC('W','A','V','E')) { WriteLog("|WAVE| not found\n"); return 0; }

	dw_buf=fm.GetRDWord();

	if(dw_buf!=MAKEFOURCC('f','m','t',' ')) { WriteLog("|fmt | not found\n"); return 0; }
	
	dw_buf=fm.GetRDWord();

	if(!dw_buf) { WriteLog("Ошибка - Загрузка звука - .Неизвестный формат аудио файла\n"); return 0; }

	fm.CopyMem(fformat,16);

	fformat->cbSize=0;

	if(fformat->wFormatTag!=1) { WriteLog("Ошибка - Загрузка звука - Сжатые файлы не поддерживаются\n"); return 0; }

	fm.GoForward(dw_buf-16);

	dw_buf=fm.GetRDWord();

	if(dw_buf==MAKEFOURCC('f','a','c','t'))
	{
		dw_buf=fm.GetRDWord();
		fm.GoForward(dw_buf);
		dw_buf=fm.GetRDWord();
	}

	if(dw_buf!=MAKEFOURCC('d','a','t','a')) { WriteLog("Ошибка - Загрузка звука - ..Неизвестный формат аудио файла\n"); return 0; }

	dw_buf=fm.GetRDWord();

	*size_data=dw_buf;

	*sample_data=new unsigned char[dw_buf];

	if(!fm.CopyMem(*sample_data,dw_buf)) { WriteLog("Ошибка - Загрузка звука - Звуковой файл битый\n"); return 0; }

	return 1;
}

int SoundManager::LoadACM(WAVEFORMATEX* fformat, unsigned char** sample_data, uint32_t* size_data)
{
	int channel;
	int freq;
	int data_size;
//	int step_data_size;

  ACMDecompressor::Context acm;
  bool acmInitialized = ACMDecompressor::Init(&acm, fm.GetBuf(),fm.GetFsize(),channel,freq,data_size);
	data_size*=2;

	if (!acmInitialized) { 
	  WriteLog("Ошибка - Загрузка звука - Неинициализировался распаковщик ACM\n"); 
	  return 0; 
	}

	freq/=2; //???

	fformat->wFormatTag=WAVE_FORMAT_PCM;
	fformat->nChannels=2; //??? channel ???
	fformat->nSamplesPerSec=freq;
	fformat->nBlockAlign=4;
	fformat->nAvgBytesPerSec=freq*4;
	fformat->wBitsPerSample=16;
	fformat->cbSize=0;

	*size_data=data_size;
	*sample_data=new unsigned char[data_size];
//	unsigned char *step_buff = new unsigned char[0x10000];
/*
	int cur_buf=0;
	while(data_size)
	{
		step_data_size=acm->readAndDecompress((unsigned short*)step_buff,data_size>0x10000?0x10000:data_size);
		memcpy(*sample_data+cur_buf,step_buff,step_data_size);

		cur_buf+=step_data_size;
		data_size-=step_data_size;
	}
*/
  int dec_data = ACMDecompressor::ReadAndDecompress(&acm, (unsigned short*)*sample_data,data_size);

	if (dec_data != data_size) { 
	  WriteLog("Ошибка - Загрузка звука - Ошибка в декодировании ACM\n"); 
	  if (*sample_data != NULL) {
	    delete sample_data;
	    sample_data = NULL;
	  } 
	  return 0; 
	}

//	SAFEDEL(step_buff);

	return 1;
}

int SoundManager::LoadOGG(WAVEFORMATEX* fformat, unsigned char** sample_data, uint32_t* size_data, char* ogg_path)
{
	FILE *fs;
	if(!(fs=fopen(ogg_path,"rb"))) return 0;

	OggVorbis_File vf;
	if(ov_open(fs,&vf,NULL,0)) return 0;
    vorbis_info* vi=ov_info(&vf,-1);

	int data_len=(int)ov_pcm_total(&vf,-1);
	int start_pos=(int)ov_pcm_tell(&vf);

	int BPS=vi->channels==1?2:4;
    *size_data=(data_len-start_pos)*BPS;

	*sample_data=new unsigned char[*size_data];

	int decoded=0;

	while(decoded<*size_data)
	{
		int curr;
		decoded+=ov_read(&vf,(char*)(*sample_data)+decoded,(*size_data)-decoded,0,2,1,&curr);
	}

	fformat->wFormatTag=WAVE_FORMAT_PCM;
	fformat->nChannels=vi->channels;
	fformat->nSamplesPerSec=vi->rate;
	fformat->wBitsPerSample=16;
	fformat->nBlockAlign=vi->channels*fformat->wBitsPerSample/8;
	fformat->nAvgBytesPerSec=fformat->nBlockAlign*vi->rate;
	fformat->cbSize=0;

	return 1;
}

void SoundManager::PlaySound(uint16_t id)
{
	if(!id)
	{
		WriteLog("Ошибка - Проигрывание звука - Звук не загружен!\n");
		return;
	}

	sound_map::iterator it=sounds.find(id);

	if(it==sounds.end())
	{
		WriteLog("Ошибка - Прогигрывание звука - Звук №%d не найден!",id);
		return;
	}

	(*it).second->buf->Play(0,0,0);
}

void SoundManager::StopSound(uint16_t id)
{
	if(!id)
	{
		WriteLog("Ошибка - Приостоновление звука - Звук не загружен!\n");
		return;
	}

	sound_map::iterator it=sounds.find(id);

	if(it==sounds.end())
	{
		WriteLog("Ошибка - Приостоновление звука - Звук №%d не найден!",id);
		return;
	}

	(*it).second->buf->Stop();
	(*it).second->buf->SetCurrentPosition(0);
}

void SoundManager::PauseSound(uint16_t id)
{
	if(!id)
	{
		WriteLog("Ошибка - Приостоновление звука - Звук не загружен!\n");
		return;
	}

	sound_map::iterator it=sounds.find(id);

	if(it==sounds.end())
	{
		WriteLog("Ошибка - Приостоновление звука - Звук №%d не найден!",id);
		return;
	}

	(*it).second->buf->Stop();
}

void SoundManager::EraseSound(uint16_t id)
{
	if(!id)
	{
		WriteLog("Ошибка - Удаление звука - Звук не загружен!\n");
		return;
	}

	sound_map::iterator it=sounds.find(id);

	if(it==sounds.end())
	{
		WriteLog("Ошибка - Удаление звука - Звук №%d не найден!",id);
		return;
	}

//	(*it).second->buf->Release();

	delete (*it).second;

	sounds.erase(it);
}