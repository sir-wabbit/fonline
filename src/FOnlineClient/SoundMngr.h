#ifndef _CSOUNDMNGR_H_
#define _CSOUNDMNGR_H_

/********************************************************************
	created:	19:01:2007   00:00;

	author:		Anton Tsvetinsky
	
	purpose:	acm,ogg
*********************************************************************/

#include <dsound.h>

#include <vorbis/codec.h>
#include <vorbis/vorbisfile.h>

#include <ACMDecompressor/ACMDecompressor.hpp>
#include <FOnlineFileManager/FileManager.hpp>

struct Sound
{
	Sound():buf(NULL){};

	IDirectSoundBuffer* buf;
};

typedef std::map<uint16_t, Sound*> sound_map;

class SoundManager
{
public:

	SoundManager():lpDS(NULL),active(false){};

	int Init();
	void Clear();

	BOOL IsActive(){return (BOOL)(active);};

	void LPESound(char* fname, int TypePath);

	int LoadSound(char* fname, int TypePath);

	void PlaySound(uint16_t id);
//	void PlaySound(uint16_t id, uint16_t loop, bool erase);
	void StopSound(uint16_t id);
	void PauseSound(uint16_t id);

	void EraseSound(uint16_t id);

	IDirectSound8 *lpDS;

	sound_map sounds;
	uint16_t cur_snd;

private:

	int LoadWAV(WAVEFORMATEX* fformat, unsigned char** sample_data, uint32_t* size_data);
	int LoadACM(WAVEFORMATEX* fformat, unsigned char** sample_data, uint32_t* size_data);
	int LoadOGG(WAVEFORMATEX* fformat, unsigned char** sample_data, uint32_t* size_data, char* ogg_path);

	FileManager fm;

	bool active;
};

#endif //_CSOUNDMNGR_H_