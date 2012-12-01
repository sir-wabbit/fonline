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

class CACMUnpacker {
public:
  // CACMUnpacker (FileReadFunction readFunc, int fileHandle, int &channels, int &frequency, int &samples);
	ACMDECOMPRESSOR_API CACMUnpacker (uint8_t* fileHandle, uint32_t fileLenght, int &channels, int &frequency, int &samples); //!Cvet
		// samples = count of sound samples (one sample is 16bits)
	ACMDECOMPRESSOR_API ~CACMUnpacker() {
//!Cvet comment
//		if (fileBuffPtr) delete (fileBuffPtr);
		if (decompBuff) free (decompBuff);
		if (someBuff) delete (someBuff);
	}

	ACMDECOMPRESSOR_API int readAndDecompress (unsigned short* buff, int count);
		// read sound samples from ACM stream
		// buff  - buffer to place decompressed samples
		// count - size of buffer (in bytes)
		// return: count of actually decompressed bytes

private:
  // File reading
  //	FileReadFunction read_file; // file reader function

  //	int hFile; // file handle, can be anything, e.g. ptr to reader-object
  int str_read (uint8_t** d_stream, int d_size, uint8_t* f_stream);
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

  // Reading routines
  unsigned char readNextPortion(); // read next block of data
  void prepareBits (int bits); // request bits
  int getBits (int bits); // request and return next bits

public:
  // These functions are used to fill the buffer with the amplitude values
  int Return0 (int pass, int ind);
  int ZeroFill (int pass, int ind);
  int LinearFill (int pass, int ind);

  int k1_3bits (int pass, int ind);
  int k1_2bits (int pass, int ind);
  int t1_5bits (int pass, int ind);

  int k2_4bits (int pass, int ind);
  int k2_3bits (int pass, int ind);
  int t2_7bits (int pass, int ind);

  int k3_5bits (int pass, int ind);
  int k3_4bits (int pass, int ind);

  int k4_5bits (int pass, int ind);
  int k4_4bits (int pass, int ind);

  int t3_7bits (int pass, int ind);
  
  // Unpacking functions
  int createAmplitudeDictionary();
  void unpackValues(); // unpack current block
  int makeNewValues(); // prepare new block, then unpack it
};

typedef int (CACMUnpacker::* FillerProc) (int pass, int ind);

#endif