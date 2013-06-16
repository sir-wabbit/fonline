#ifndef __ACM_STREAM_UNPACK
#define __ACM_STREAM_UNPACK

#include <stdlib.h>
#include <stdint.h>

#ifdef _WIN32
  #ifdef ACMDECOMPRESSOR_DLL
    #define ACMDECOMPRESSOR_API __declspec(dllexport)
  #else
    #define ACMDECOMPRESSOR_API __declspec(dllimport)
  #endif
#else
  #define ACMDECOMPRESSOR_API
#endif

//typedef int (_stdcall* FileReadFunction) (int hFile, unsigned char* buffer, int count);

namespace ACMDecompressor {

struct Context {
  // File reading
  //	FileReadFunction read_file; // file reader function

  //	int hFile; // file handle, can be anything, e.g. ptr to reader-object
  uint8_t* hFile;
  uint32_t fileLen;
  int fileCur;

  unsigned char *fileBuffPtr, *buffPos; // pointer to file buffer and current position
  int bufferSize; // size of file buffer
  int availBytes; // new (not yet processed) bytes in file buffer

  // Bits
  unsigned nextBits; // new bits
  int availBits; // count of new bits

  // Parameters of ACM stream
  int	packAttrs, someSize, packAttrs2, someSize2;

  // Unpacking buffers
  int *decompBuff, *someBuff;
  int blocks, totBlSize;
  int valsToGo; // samples left to decompress
  int *values; // pointer to decompressed samples
  int valCnt; // count of decompressed samples
};

// Init the acm decompressor stream.
// @param  ctx     ACM decompressor context.
// @param  fileHandle
// @param  fileLength
// @param  channels
// @param  frequency
// @param  samples
ACMDECOMPRESSOR_API bool Init(Context* ctx, uint8_t* fileHandle, uint32_t fileLength, int &channels, int &frequency, int &samples);

// Close the acm decompressor stream.
// @param  ctx     ACM decompressor context.
ACMDECOMPRESSOR_API void Close(Context* ctx);

// Read sound samples from the ACM stream.
// @param  ctx     ACM decompressor context.
// @param  buffer  Buffer to place decompressed samples.
// @param  count   Buffer size in bytes.
// @return         The number of actually decompressed bytes.
ACMDECOMPRESSOR_API int ReadAndDecompress(Context* ctx, unsigned short* buffer, int count);

} // namespace ACMDecompressor

class CACMUnpacker {
public:
  // CACMUnpacker (FileReadFunction readFunc, int fileHandle, int &channels, int &frequency, int &samples);
	ACMDECOMPRESSOR_API CACMUnpacker (uint8_t* fileHandle, uint32_t fileLenght, int &channels, int &frequency, int &samples) {
	  memset(&ctx, 0, sizeof(ctx));
    ACMDecompressor::Init(&ctx, fileHandle, fileLenght, channels, frequency, samples);
	}
		// samples = count of sound samples (one sample is 16bits)
	ACMDECOMPRESSOR_API ~CACMUnpacker() {
    ACMDecompressor::Close(&ctx);
	}

	ACMDECOMPRESSOR_API int readAndDecompress (unsigned short* buff, int count) {
    return ACMDecompressor::ReadAndDecompress(&ctx, buff, count);
	}
		// read sound samples from ACM stream
		// buff  - buffer to place decompressed samples
		// count - size of buffer (in bytes)
		// return: count of actually decompressed bytes

private:
  ACMDecompressor::Context ctx;
};

#endif
