#include <memory.h>
#include <stdio.h>

// XXX[27.7.2012 alex]: this code reeks of x64-incompatibility


#include "ACMDecompressor.hpp"
// SomeBuff [PackAttr2, SomeSize]

// Reading routines
unsigned char readNextPortion(ACMDecompressor::Context* ctx); // read next block of data
void prepareBits (ACMDecompressor::Context* ctx, int bits); // request bits
int getBits (ACMDecompressor::Context* ctx, int bits); // request and return next bits
int str_read(ACMDecompressor::Context* ctx, uint8_t** d_stream, size_t d_size, uint8_t* f_stream);

void sub_4d420c (int *decBuff, int *someBuff, int someSize, int blocks);
void sub_4d3fcc (short *decBuff, int *someBuff, int someSize, int blocks);

typedef int (*FillerProc) (ACMDecompressor::Context* ctx, int pass, int ind);

namespace fillers {

// These functions are used to fill the buffer with the amplitude values
int Return0 (ACMDecompressor::Context* ctx, int pass, int ind);
int ZeroFill (ACMDecompressor::Context* ctx, int pass, int ind);
int LinearFill (ACMDecompressor::Context* ctx, int pass, int ind);

int k1_3bits (ACMDecompressor::Context* ctx, int pass, int ind);
int k1_2bits (ACMDecompressor::Context* ctx, int pass, int ind);
int t1_5bits (ACMDecompressor::Context* ctx, int pass, int ind);

int k2_4bits (ACMDecompressor::Context* ctx, int pass, int ind);
int k2_3bits (ACMDecompressor::Context* ctx, int pass, int ind);
int t2_7bits (ACMDecompressor::Context* ctx, int pass, int ind);

int k3_5bits (ACMDecompressor::Context* ctx, int pass, int ind);
int k3_4bits (ACMDecompressor::Context* ctx, int pass, int ind);

int k4_5bits (ACMDecompressor::Context* ctx, int pass, int ind);
int k4_4bits (ACMDecompressor::Context* ctx, int pass, int ind);

int t3_7bits (ACMDecompressor::Context* ctx, int pass, int ind);

} // namespace fillers

FillerProc Fillers[32] = {
  &fillers::ZeroFill,
  &fillers::Return0,
  &fillers::Return0,
  &fillers::LinearFill,
  &fillers::LinearFill,
  &fillers::LinearFill,
  &fillers::LinearFill,
  &fillers::LinearFill,
  &fillers::LinearFill,
  &fillers::LinearFill,
  &fillers::LinearFill,
  &fillers::LinearFill,
  &fillers::LinearFill,
  &fillers::LinearFill,
  &fillers::LinearFill,
  &fillers::LinearFill,
  &fillers::LinearFill,
  &fillers::k1_3bits,
  &fillers::k1_2bits,
  &fillers::t1_5bits,
  &fillers::k2_4bits,
  &fillers::k2_3bits,
  &fillers::t2_7bits,
  &fillers::k3_5bits,
  &fillers::k3_4bits,
  &fillers::Return0,
  &fillers::k4_5bits,
  &fillers::k4_4bits,
  &fillers::Return0,
  &fillers::t3_7bits,
  &fillers::Return0,
  &fillers::Return0
};

// Unpacking functions
int createAmplitudeDictionary(ACMDecompressor::Context* ctx);
void unpackValues(ACMDecompressor::Context* ctx); // unpack current block
int makeNewValues(ACMDecompressor::Context* ctx); // prepare new block, then unpack it

char Table1 [27] =
		{0,  1,  2,   4,  5,  6,   8,  9, 10,
		16, 17, 18,  20, 21, 22,  24, 25, 26,
		32, 33, 34,  36, 37, 38,  40, 41, 42};
//Eng: in base-4 system it is:
//Rus: в четверичной системе это будет:
//		000 001 002  010 011 012  020 021 022
//		100 101 102  110 111 112  120 121 122
//		200 201 202  210 211 212  220 221 222
short Table2 [125] =
		{ 0,  1,  2,  3,  4,   8,  9, 10, 11, 12,  16, 17, 18, 19, 20,  24, 25, 26, 27, 28,  32, 33, 34, 35, 36,
		 64, 65, 66, 67, 68,  72, 73, 74, 75, 76,  80, 81, 82, 83, 84,  88, 89, 90, 91, 92,  96, 97, 98, 99,100,
		128,129,130,131,132, 136,137,138,139,140, 144,145,146,147,148, 152,153,154,155,156, 160,161,162,163,164,
		192,193,194,195,196, 200,201,202,203,204, 208,209,210,211,212, 216,217,218,219,220, 224,225,226,227,228,
		256,257,258,259,260, 264,265,266,267,268, 272,273,274,275,276, 280,281,282,283,284, 288,289,290,291,292};
//Eng: in base-8 system:
//Rus: в восьмеричной системе:
//		000 001 002 003 004  010 011 012 013 014 ...
//		100 101 102 103 104 ...
//		200 ...
//		... 
unsigned char Table3 [121] =
		{0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A,
		 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A,
		 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A,
		 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A,
		 0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A,
		 0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A,
		 0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A,
		 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A,
		 0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8A,
		 0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9A,
		 0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8, 0xA9, 0xAA};

short Amplitude_Buffer [0x10000];
short *Buffer_Middle = &Amplitude_Buffer[0x8000];

void sub_4d3fcc (short *decBuff, int *someBuff, int someSize, int blocks);
void sub_4d420c (int *decBuff, int *someBuff, int someSize, int blocks);

//!Cvet edit
//CACMUnpacker::CACMUnpacker (FileReadFunction readFunc, int fileHandle, int &channels, int &frequency, int &samples)

namespace ACMDecompressor {

bool Init(Context* ctx, uint8_t* fileHandle, uint32_t fileLenght, int &channels, int &frequency, int &samples) {
  ctx->fileBuffPtr = NULL;
  ctx->decompBuff = NULL;
  ctx->someBuff = NULL;

	ctx->hFile = fileHandle;
	ctx->fileLen = fileLenght; //!Cvet create
	ctx->fileCur = 0;
	try {
//!Cvet comment
//		if (!readFunc) throw;
//		read_file = readFunc;
//		fileBuffPtr = new unsigned char [0x200];
//		if (!fileBuffPtr) throw;
		ctx->bufferSize = 0x200;
		ctx->availBytes = 0;
		ctx->nextBits = 0;
		ctx->availBits = 0;

		if ((getBits (ctx, 24) & 0xFFFFFF) != 0x032897) throw;
		if ((getBits (ctx, 8) & 0xFF) != 1) throw;
		ctx->valsToGo = (getBits (ctx, 16) & 0xFFFF) |
			((getBits (ctx, 16) & 0xFFFF) << 16);
		channels = getBits (ctx, 16) & 0xFFFF;
		frequency = getBits (ctx, 16) & 0xFFFF;
		samples = ctx->valsToGo;

		ctx->packAttrs = getBits (ctx, 4) & 0xF;
		ctx->packAttrs2 = getBits (ctx, 12) & 0xFFF;

		ctx->someSize = 1 << ctx->packAttrs;
		ctx->someSize2 = ctx->someSize * ctx->packAttrs2;

		int decBuf_size = 0;
		if (ctx->packAttrs)
			decBuf_size = 3*ctx->someSize / 2 - 2;
		
		ctx->blocks = 0x800 / ctx->someSize - 2;
		if (ctx->blocks < 1) ctx->blocks = 1;
		ctx->totBlSize = ctx->blocks * ctx->someSize;

		if (decBuf_size) {
			ctx->decompBuff = (int*)calloc (decBuf_size, sizeof(int));
			if (!ctx->decompBuff) throw;
		}

		ctx->someBuff = new int[ctx->someSize2];
		if (!ctx->someBuff) throw;
		ctx->valCnt = 0;
  // XXX[27.7.2012 alex]: catch void??
	//} catch (void) {
	} catch(...) {
		Close(ctx);
		return false;
	}
	
	return true;
}

void Close(Context* ctx) {
  if (ctx->decompBuff) free (ctx->decompBuff);
  if (ctx->someBuff) delete (ctx->someBuff);
}

int ReadAndDecompress(Context* ctx, unsigned short* buff, int count) {
  //Eng: takes new values (of type int) and writes them into output buffer (of type word),
  //  the values are shifted by packAttrs bits
  //Rus: берет новые значени€ (типа инт) и записывает в выходной буффер (типа ворд),
  // при этом значени€ дел€тс€ на packAttrs бит
  int res = 0;
  while (res < count) {
    if (!ctx->valCnt) {
      if (ctx->valsToGo == 0) break;
      if (!makeNewValues(ctx)) break;
    }
    *buff = (*ctx->values) >> ctx->packAttrs;
    ctx->values++;
    buff++;
    res += 2;
    ctx->valCnt--;
  }
  return res;
}

} // namespace ACMDecompressor


unsigned char readNextPortion(ACMDecompressor::Context* ctx) {
//!Cvet edit
//	if ( !(availBytes = read_file (hFile, fileBuffPtr, bufferSize))) {
	if ( !(ctx->availBytes = str_read (ctx, &ctx->fileBuffPtr, ctx->bufferSize, ctx->hFile))) { //!Cvet new function
		ctx->fileBuffPtr=ctx->hFile; //!Cvet
		memset (ctx->fileBuffPtr, '\0', ctx->bufferSize);
		ctx->availBytes = ctx->bufferSize;
	}
	ctx->buffPos = ctx->fileBuffPtr+1;
	ctx->availBytes--;
	return *ctx->fileBuffPtr;
}
//!Cvet++++++++++++++++++++++++++++++
int str_read(ACMDecompressor::Context* ctx, uint8_t** d_stream, size_t d_size, uint8_t* f_stream)
{
//	if(fileCur+d_size>fileLen) return 0; //!!!!доделать

	if (ctx->fileCur + d_size > ctx->fileLen) {
	  d_size -= (ctx->fileLen - ctx->fileCur);
	}

	*d_stream=ctx->hFile+ctx->fileCur;

	ctx->fileCur += d_size;

	return d_size;
}
//!Cvet------------------------------
void prepareBits(ACMDecompressor::Context* ctx, int bits) {
	while (bits > ctx->availBits) {
		int oneByte;
		ctx->availBytes--;
		if (ctx->availBytes >= 0) {
			oneByte = *ctx->buffPos;
			ctx->buffPos++;
		} else
			oneByte = readNextPortion(ctx);
		ctx->nextBits |= (oneByte << ctx->availBits);
		ctx->availBits += 8;
	}
}
int getBits(ACMDecompressor::Context* ctx, int bits) {
	prepareBits (ctx, bits);
	int res = ctx->nextBits;
	ctx->availBits -= bits;
	ctx->nextBits >>= bits;
	return res;
}


int createAmplitudeDictionary(ACMDecompressor::Context* ctx) {
	int pwr = getBits (ctx, 4) & 0xF,
		val = getBits (ctx, 16) & 0xFFFF,
		count = 1 << pwr,
		v = 0;

	for (int i=0; i<count; i++) {
		Buffer_Middle[i] = v;
		v += val;
	}
	v = -val;
	for (int i=0; i<count; i++) {
		Buffer_Middle[-i-1] = v;
		v -= val;
	}

	// FillTables(). We have aleady done it, see definitions of Tables

	for (int pass=0; pass<ctx->someSize; pass++) {
		int ind = getBits (ctx, 5) & 0x1F;
		if (! ((Fillers[ind]) (ctx, pass, ind)) )
			return 0;
	}
	return 1;
}
void unpackValues(ACMDecompressor::Context* ctx) {
	if (!ctx->packAttrs) return;
	
	int counter = ctx->packAttrs2;
	int* someBuffPtr = ctx->someBuff;

	while (counter > 0) {
		int* decBuffPtr = ctx->decompBuff;
		int loc_blocks = ctx->blocks;
		int loc_someSize = ctx->someSize / 2;

		if (loc_blocks > counter)
			loc_blocks = counter;

		loc_blocks *= 2;
		sub_4d3fcc ((short*)decBuffPtr, someBuffPtr, loc_someSize, loc_blocks);

		decBuffPtr += loc_someSize;

		for (int i=0; i<loc_blocks; i++)
			someBuffPtr [i*loc_someSize]++;

		loc_someSize /= 2;
		loc_blocks *= 2;

		while (loc_someSize != 0) {
			sub_4d420c (decBuffPtr, someBuffPtr, loc_someSize, loc_blocks);
			decBuffPtr += loc_someSize*2;

			loc_someSize /= 2;
			loc_blocks *= 2;
		}

		counter -= ctx->blocks;
		someBuffPtr += ctx->totBlSize;
	}
}
int makeNewValues(ACMDecompressor::Context* ctx) {
	if (!createAmplitudeDictionary(ctx)) return 0;
	unpackValues(ctx);

	ctx->values = ctx->someBuff;
	ctx->valCnt = (ctx->someSize2 > ctx->valsToGo)? ctx->valsToGo: ctx->someSize2;
	ctx->valsToGo -= ctx->valCnt;

	return 1;
}

namespace fillers {
// Filling functions:
// int CACMUnpacker::FillerProc (int pass, int ind)
int Return0(ACMDecompressor::Context* ctx, int pass, int ind) {
	return 0;
}
int ZeroFill (ACMDecompressor::Context* ctx, int pass, int ind) {
//Eng: used when the whole column #pass is zero-filled
//Rus: используетс€, когда весь столбец с номером pass заполнен нул€ми

//	the meaning is following:

//	for (int i=0; i<packAttrs2; i++)
//		someBuff [i, pass] = 0;

//	speed-optimized version:
	int *sb_ptr = &ctx->someBuff [pass],
		step = ctx->someSize,
		i = ctx->packAttrs2;
	do {
		*sb_ptr = 0;
		sb_ptr += step;
	} while ((--i) != 0);
	return 1;
}
int LinearFill (ACMDecompressor::Context* ctx, int pass, int ind) {
	int mask = (1 << ind) - 1;
	short* lb_ptr = &Buffer_Middle [(-1l) << (ind-1)];

	for (int i=0; i<ctx->packAttrs2; i++)
		ctx->someBuff [i*ctx->someSize + pass] = lb_ptr [getBits(ctx, ind) & mask];
	return 1;
}
int k1_3bits (ACMDecompressor::Context* ctx, int pass, int ind) {
//Eng: column with number pass is filled with zeros, and also +/-1, zeros are repeated frequently
//Rus: cтолбец pass заполнен нул€ми, а также +/- 1, но нули часто идут подр€д
// efficiency (bits per value): 3-p0-2.5*p00, p00 - cnt of paired zeros, p0 - cnt of single zeros.
//Eng: it makes sense to use, when the freqnecy of paired zeros (p00) is greater than 2/3
//Rus: имеет смысл использовать, когда веро€тность парных нулей (p00) больше 2/3
	for (int i=0; i<ctx->packAttrs2; i++) {
		prepareBits (ctx, 3);
		if ((ctx->nextBits & 1) == 0) {
			ctx->availBits--;
			ctx->nextBits >>= 1;
			ctx->someBuff [i*ctx->someSize + pass] = 0; if ((++i) == ctx->packAttrs2) break;
			ctx->someBuff [i*ctx->someSize + pass] = 0;
		} else if ((ctx->nextBits & 2) == 0) {
			ctx->availBits -= 2;
			ctx->nextBits >>= 2;
			ctx->someBuff [i*ctx->someSize + pass] = 0;
		} else {
			ctx->someBuff [i*ctx->someSize + pass] = (ctx->nextBits & 4)? Buffer_Middle[1]: Buffer_Middle[-1];
			ctx->availBits -= 3;
			ctx->nextBits >>= 3;
		}
	}
	return 1;
}
int k1_2bits (ACMDecompressor::Context* ctx, int pass, int ind) {
//Eng: column is filled with zero and +/-1
//Rus: cтолбец pass заполнен нул€ми, а также +/- 1
// efficiency: 2-P0. P0 - cnt of any zero (P0 = p0 + p00)
//Eng: use it when P0 > 1/3
//Rus: имеет смысл использовать, когда веро€тность нул€ больше 1/3
	for (int i=0; i<ctx->packAttrs2; i++) {
		prepareBits (ctx, 2);
		if ((ctx->nextBits & 1) == 0) {
			ctx->availBits--;
			ctx->nextBits >>= 1;
			ctx->someBuff [i*ctx->someSize + pass] = 0;
		} else {
			ctx->someBuff [i*ctx->someSize + pass] = (ctx->nextBits & 2)? Buffer_Middle[1]: Buffer_Middle[-1];
			ctx->availBits -= 2;
			ctx->nextBits >>= 2;
		}
	}
	return 1;
}
int t1_5bits (ACMDecompressor::Context* ctx, int pass, int ind) {
//Eng: all the -1, 0, +1 triplets
//Rus: все комбинации троек -1, 0, +1.
// efficiency: always 5/3 bits per value
// use it when P0 <= 1/3
	for (int i=0; i<ctx->packAttrs2; i++) {
		char bits = getBits (ctx, 5) & 0x1f;
		bits = Table1 [bits];

		ctx->someBuff [i*ctx->someSize + pass] = Buffer_Middle[-1 + (bits & 3)];
			if ((++i) == ctx->packAttrs2) break;
			bits >>= 2;
		ctx->someBuff [i*ctx->someSize + pass] = Buffer_Middle[-1 + (bits & 3)];
			if ((++i) == ctx->packAttrs2) break;
			bits >>= 2;
		ctx->someBuff [i*ctx->someSize + pass] = Buffer_Middle[-1 + bits];
	}
	return 1;
}
int k2_4bits (ACMDecompressor::Context* ctx, int pass, int ind) {
// -2, -1, 0, 1, 2, and repeating zeros
// efficiency: 4-2*p0-3.5*p00, p00 - cnt of paired zeros, p0 - cnt of single zeros.
//Eng: makes sense to use when p00>2/3
//Rus: имеет смысл использовать, когда веро€тность парных нулей (p00) больше 2/3
	for (int i=0; i<ctx->packAttrs2; i++) {
		prepareBits (ctx, 4);
		if ((ctx->nextBits & 1) == 0) {
			ctx->availBits--;
			ctx->nextBits >>= 1;
			ctx->someBuff [i*ctx->someSize + pass] = 0; if ((++i) == ctx->packAttrs2) break;
			ctx->someBuff [i*ctx->someSize + pass] = 0;
		} else if ((ctx->nextBits & 2) == 0) {
			ctx->availBits -= 2;
			ctx->nextBits >>= 2;
			ctx->someBuff [i*ctx->someSize + pass] = 0;
		} else {
			ctx->someBuff [i*ctx->someSize + pass] =
				(ctx->nextBits & 8)?
					( (ctx->nextBits & 4)? Buffer_Middle[2]: Buffer_Middle[1] ):
					( (ctx->nextBits & 4)? Buffer_Middle[-1]: Buffer_Middle[-2] );
			ctx->availBits -= 4;
			ctx->nextBits >>= 4;
		}
	}
	return 1;
}
int k2_3bits (ACMDecompressor::Context* ctx, int pass, int ind) {
// -2, -1, 0, 1, 2
// efficiency: 3-2*P0, P0 - cnt of any zero (P0 = p0 + p00)
//Eng: use when P0>1/3
//Rus: имеет смысл использовать, когда веро€тность нул€ больше 1/3
	for (int i=0; i<ctx->packAttrs2; i++) {
		prepareBits (ctx, 3);
		if ((ctx->nextBits & 1) == 0) {
			ctx->availBits--;
			ctx->nextBits >>= 1;
			ctx->someBuff [i*ctx->someSize + pass] = 0;
		} else {
			ctx->someBuff [i*ctx->someSize + pass] =
				(ctx->nextBits & 4)?
					( (ctx->nextBits & 2)? Buffer_Middle[2]: Buffer_Middle[1] ):
					( (ctx->nextBits & 2)? Buffer_Middle[-1]: Buffer_Middle[-2] );
			ctx->availBits -= 3;
			ctx->nextBits >>= 3;
		}
	}
	return 1;
}
int t2_7bits (ACMDecompressor::Context* ctx, int pass, int ind) {
//Eng: all the +/-2, +/-1, 0  triplets
// efficiency: always 7/3 bits per value
//Rus: все комбинации троек -2, -1, 0, +1, 2.
// эффективность: 7/3 бита на значение - всегда
// use it when p0 <= 1/3
	for (int i=0; i<ctx->packAttrs2; i++) {
		char bits = getBits (ctx, 7) & 0x7f;
		short val = Table2 [bits];

		ctx->someBuff [i*ctx->someSize + pass] = Buffer_Middle[-2 + (val & 7)];
			if ((++i) == ctx->packAttrs2) break;
			val >>= 3;
		ctx->someBuff [i*ctx->someSize + pass] = Buffer_Middle[-2 + (val & 7)];
			if ((++i) == ctx->packAttrs2) break;
			val >>= 3;
		ctx->someBuff [i*ctx->someSize + pass] = Buffer_Middle[-2 + val];
	}
	return 1;
}
int k3_5bits (ACMDecompressor::Context* ctx, int pass, int ind) {
// fills with values: -3, -2, -1, 0, 1, 2, 3, and double zeros
// efficiency: 5-3*p0-4.5*p00-p1, p00 - cnt of paired zeros, p0 - cnt of single zeros, p1 - cnt of +/- 1.
// can be used when frequency of paired zeros (p00) is greater than 2/3
	for (int i=0; i<ctx->packAttrs2; i++) {
		prepareBits (ctx, 5);
		if ((ctx->nextBits & 1) == 0) {
			ctx->availBits--;
			ctx->nextBits >>= 1;
			ctx->someBuff [i*ctx->someSize + pass] = 0; if ((++i) == ctx->packAttrs2) break;
			ctx->someBuff [i*ctx->someSize + pass] = 0;
		} else if ((ctx->nextBits & 2) == 0) {
			ctx->availBits -= 2;
			ctx->nextBits >>= 2;
			ctx->someBuff [i*ctx->someSize + pass] = 0;
		} else if ((ctx->nextBits & 4) == 0) {
			ctx->someBuff [i*ctx->someSize + pass] = (ctx->nextBits & 8)? Buffer_Middle[1]: Buffer_Middle[-1];
			ctx->availBits -= 4;
			ctx->nextBits >>= 4;
		} else {
			ctx->availBits -= 5;
			int val = (ctx->nextBits & 0x18) >> 3;
			ctx->nextBits >>= 5;
			if (val >= 2) val += 3;
			ctx->someBuff [i*ctx->someSize + pass] = Buffer_Middle[-3 + val];
		}
	}
	return 1;
}
int k3_4bits (ACMDecompressor::Context* ctx, int pass, int ind) {
// fills with values: -3, -2, -1, 0, 1, 2, 3.
// efficiency: 4-3*P0-p1, P0 - cnt of all zeros (P0 = p0 + p00), p1 - cnt of +/- 1.
	for (int i=0; i<ctx->packAttrs2; i++) {
		prepareBits (ctx, 4);
		if ((ctx->nextBits & 1) == 0) {
			ctx->availBits--;
			ctx->nextBits >>= 1;
			ctx->someBuff [i*ctx->someSize + pass] = 0;
		} else if ((ctx->nextBits & 2) == 0) {
			ctx->availBits -= 3;
			ctx->someBuff [i*ctx->someSize + pass] = (ctx->nextBits & 4)? Buffer_Middle[1]: Buffer_Middle[-1];
			ctx->nextBits >>= 3;
		} else {
			int val = (ctx->nextBits &0xC) >> 2;
			ctx->availBits -= 4;
			ctx->nextBits >>= 4;
			if (val >= 2) val += 3;
			ctx->someBuff [i*ctx->someSize + pass] = Buffer_Middle[-3 + val];
		}
	}
	return 1;
}
int k4_5bits (ACMDecompressor::Context* ctx, int pass, int ind) {
// fills with values: +/-4, +/-3, +/-2, +/-1, 0, and double zeros
// efficiency: 5-3*p0-4.5*p00, p00 - cnt of paired zeros, p0 - cnt of single zeros.
//Eng: makes sense to use when p00>2/3
//Rus: имеет смысл использовать, когда веро€тность парных нулей (p00) больше 2/3
	for (int i=0; i<ctx->packAttrs2; i++) {
		prepareBits (ctx, 5);
		if ((ctx->nextBits & 1) == 0) {
			ctx->availBits--;
			ctx->nextBits >>= 1;
			ctx->someBuff [i*ctx->someSize + pass] = 0; if ((++i) == ctx->packAttrs2) break;
			ctx->someBuff [i*ctx->someSize + pass] = 0;
		} else if ((ctx->nextBits & 2) == 0) {
			ctx->availBits -= 2;
			ctx->nextBits >>= 2;
			ctx->someBuff [i*ctx->someSize + pass] = 0;
		} else {
			int val = (ctx->nextBits &0x1C) >> 2;
			if (val >= 4) val++;
			ctx->someBuff [i*ctx->someSize + pass] = Buffer_Middle[-4 + val];
			ctx->availBits -= 5;
			ctx->nextBits >>= 5;
		}
	}
	return 1;
}
int k4_4bits (ACMDecompressor::Context* ctx, int pass, int ind) {
// fills with values: +/-4, +/-3, +/-2, +/-1, 0, and double zeros
// efficiency: 4-3*P0, P0 - cnt of all zeros (both single and paired).
	for (int i=0; i<ctx->packAttrs2; i++) {
		prepareBits (ctx, 4);
		if ((ctx->nextBits & 1) == 0) {
			ctx->availBits--;
			ctx->nextBits >>= 1;
			ctx->someBuff [i*ctx->someSize + pass] = 0;
		} else {
			int val = (ctx->nextBits &0xE) >> 1;
			ctx->availBits -= 4;
			ctx->nextBits >>= 4;
			if (val >= 4) val++;
			ctx->someBuff [i*ctx->someSize + pass] = Buffer_Middle[-4 + val];
		}
	}
	return 1;
}
int t3_7bits (ACMDecompressor::Context* ctx, int pass, int ind) {
//Eng: all the pairs of values from -5 to +5
// efficiency: 7/2 bits per value
//Rus: все комбинации пар от -5 до +5
// эффективность: 7/2 бита на значение - всегда
	for (int i=0; i<ctx->packAttrs2; i++) {
		char bits = getBits (ctx, 7) & 0x7f;
		unsigned char val = Table3 [bits];

		ctx->someBuff [i*ctx->someSize + pass] = Buffer_Middle[-5 + (val & 0xF)];
			if ((++i) == ctx->packAttrs2) break;
			val >>= 4;
		ctx->someBuff [i*ctx->someSize + pass] = Buffer_Middle[-5 + val];
	}
	return 1;
}
} // namespace fillers

void sub_4d3fcc (short *decBuff, int *someBuff, int someSize, int blocks) {
  int row_0, row_1, row_2, row_3, db_0, db_1;
  if (blocks == 2) {
    for (int i=0; i<someSize; i++) {
      row_0 = someBuff[0];
      row_1 = someBuff[someSize];
      someBuff [0] = someBuff[0] + decBuff[0] + 2*decBuff[1];
      someBuff [someSize] = 2*row_0 - decBuff[1] - someBuff[someSize];
      decBuff [0] = row_0;
      decBuff [1] = row_1;

      decBuff += 2;
      someBuff++;
      //Eng: reverse process most likely will be:
      //Rus: обратный процесс будет, по всей видимости:
      //	pk[0]  = real[0] - db[0] - 2*db[1];
      //	pk[ss] = 2*pk[0] - db[1] - real[ss];
      //	db[0]  = pk[0];
      //	db[1]  = pk[ss];
      //Eng: or even
      //Rus: а может даже:
      //	db[i][0] = (pk[i]    = real[i] - db[i][0] - 2*db[i][1]);
      //	db[i][1] = (pk[i+ss] = 2*pk[i] - db[i][1] - real[i+ss]);
    }
  } else if (blocks == 4) {
    for (int i=0; i<someSize; i++) {
      row_0 = someBuff[0];
      row_1 = someBuff[someSize];
      row_2 = someBuff[2*someSize];
      row_3 = someBuff[3*someSize];

      someBuff [0]          =  decBuff[0] + 2*decBuff[1] + row_0;
      someBuff [someSize]   = -decBuff[1] + 2*row_0      - row_1;
      someBuff [2*someSize] =  row_0      + 2*row_1      + row_2;
      someBuff [3*someSize] = -row_1      + 2*row_2      - row_3;

      decBuff [0] = row_2;
      decBuff [1] = row_3;

      decBuff += 2;
      someBuff++;
      //	pk[0][i] = -pk[-2][i] - 2*pk[-1][i] + real[0][i];
      //	pk[1][i] = -pk[-1][i] + 2*pk[0][i]  - real[1][i];
      //	pk[2][i] = -pk[0][i]  - 2*pk[1][i]  + real[2][i];
      //	pk[3][i] = -pk[1][i]  + 2*pk[2][i]  - real[3][i];
    }
  } else {
    for (int i=0; i<someSize; i++) {
      int* someBuff_ptr = someBuff;
      if (((blocks >> 1) & 1) != 0) {
        row_0 = someBuff_ptr[0];
        row_1 = someBuff_ptr[someSize];

        someBuff_ptr [0]        =  decBuff[0] + 2*decBuff[1] + row_0;
        someBuff_ptr [someSize] = -decBuff[1] + 2*row_0      - row_1;
        someBuff_ptr += 2*someSize;

        db_0 = row_0;
        db_1 = row_1;
      } else {
        db_0 = decBuff[0];
        db_1 = decBuff[1];
      }

      for (int j=0; j<blocks >> 2; j++) {
        row_0 = someBuff_ptr[0];  someBuff_ptr [0] =  db_0  + 2*db_1  + row_0;  someBuff_ptr += someSize;
        row_1 = someBuff_ptr[0];  someBuff_ptr [0] = -db_1  + 2*row_0 - row_1;  someBuff_ptr += someSize;
        row_2 = someBuff_ptr[0];  someBuff_ptr [0] =  row_0 + 2*row_1 + row_2;  someBuff_ptr += someSize;
        row_3 = someBuff_ptr[0];  someBuff_ptr [0] = -row_1 + 2*row_2 - row_3;  someBuff_ptr += someSize;

        db_0 = row_2;
        db_1 = row_3;
      }
      decBuff [0] = row_2;
      decBuff [1] = row_3;
      //Eng: the same as in previous cases, but larger. The process is seem to be reversible
      //Rus: то же самое, что и в предыдущих случа€х, только больше по количеству. –адует, что процесс обратимый
      decBuff += 2;
      someBuff++;
    }
  }
}


void sub_4d420c (int *decBuff, int *someBuff, int someSize, int blocks) {
  int row_0, row_1, row_2, row_3, db_0, db_1;
  if (blocks == 4) {
    for (int i=0; i<someSize; i++) {
      row_0 = someBuff[0];
      row_1 = someBuff[someSize];
      row_2 = someBuff[2*someSize];
      row_3 = someBuff[3*someSize];

      someBuff [0]          =  decBuff[0] + 2*decBuff[1] + row_0;
      someBuff [someSize]   = -decBuff[1] + 2*row_0      - row_1;
      someBuff [2*someSize] =  row_0      + 2*row_1      + row_2;
      someBuff [3*someSize] = -row_1      + 2*row_2      - row_3;

      decBuff [0] = row_2;
      decBuff [1] = row_3;

      decBuff += 2;
      someBuff++;
    }
  } else {
    for (int i=0; i<someSize; i++) {
      int* someBuff_ptr = someBuff;
      db_0 = decBuff[0]; db_1 = decBuff[1];
      for (int j=0; j<blocks >> 2; j++) {
        row_0 = someBuff_ptr[0];  someBuff_ptr [0] =  db_0  + 2*db_1  + row_0;  someBuff_ptr += someSize;
        row_1 = someBuff_ptr[0];  someBuff_ptr [0] = -db_1  + 2*row_0 - row_1;  someBuff_ptr += someSize;
        row_2 = someBuff_ptr[0];  someBuff_ptr [0] =  row_0 + 2*row_1 + row_2;  someBuff_ptr += someSize;
        row_3 = someBuff_ptr[0];  someBuff_ptr [0] = -row_1 + 2*row_2 - row_3;  someBuff_ptr += someSize;

        db_0 = row_2;
        db_1 = row_3;
      }
      decBuff [0] = row_2;
      decBuff [1] = row_3;

      decBuff += 2;
      someBuff++;
    }
  }
} // namespace filters