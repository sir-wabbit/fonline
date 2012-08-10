#ifndef _CSOUNDMNGR_H_
#define _CSOUNDMNGR_H_

/********************************************************************
	created:	19:01:2007   00:00;

	author:		Anton Tsvetinsky
	
	purpose:	acm,ogg
*********************************************************************/

#include "CFileMngr.h"

#include "decomp_acm\acmstrm.h"

#include <vorbis/codec.h>
#include <vorbis/vorbisfile.h>

struct Sound
{
	Sound():buf(NULL){};

	IDirectSoundBuffer* buf;
};

typedef std::map<WORD, Sound*> sound_map;

class CSoundMngr
{
public:

	CSoundMngr():lpDS(NULL),active(false){};

	int Init();
	void Clear();

	BOOL IsActive(){return (BOOL)(active);};

	void LPESound(char* fname, int TypePath);

	int LoadSound(char* fname, int TypePath);

	void PlaySound(WORD id);
//	void PlaySound(WORD id, WORD loop, bool erase);
	void StopSound(WORD id);
	void PauseSound(WORD id);

	void EraseSound(WORD id);

	IDirectSound8 *lpDS;

	sound_map sounds;
	WORD cur_snd;

private:

	int LoadWAV(WAVEFORMATEX* fformat, unsigned char** sample_data, DWORD* size_data);
	int LoadACM(WAVEFORMATEX* fformat, unsigned char** sample_data, DWORD* size_data);
	int LoadOGG(WAVEFORMATEX* fformat, unsigned char** sample_data, DWORD* size_data, char* ogg_path);

	FileManager fm;

	bool active;
};

#endif //_CSOUNDMNGR_H_