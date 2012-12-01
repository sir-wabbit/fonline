#ifndef LZSS_HPP_
#define LZSS_HPP_

#include <stddef.h>
#include <stdint.h>

#ifdef _WIN32
  #ifdef LZSS_DLL
    #define LZSS_API __declspec(dllexport)
  #else
    #define LZSS_API __declspec(dllimport)
  #endif
#else
  #define LZSS_API
#endif

namespace LZSS {

struct LZSSContext {
  unsigned char* window;
  long inBufPtr, inBufSize, outBufPtr;
  long unpackedLen;
  unsigned char *inBuf, *outBuf;
};

void Init(LZSSContext* ctx);
void Close(LZSSContext* ctx);

};

/* number of bits for index and length respectively */
#define INDEX_BC                12
#define LENGTH_BC               4

/* window size and raw lookahead size */
#define WINSIZE                 (1 << INDEX_BC)
#define RLOOK_SIZE              (1 << LENGTH_BC)

/* number of bytes to break even in compressing */
#define BREAK_EVEN              2
//((1 + INDEX_BC + LENGTH_BC) / 9)
#define LOOK_SIZE               (RLOOK_SIZE + BREAK_EVEN)

#define MOD_WINDOW( a )		(( a ) & (WINSIZE - 1))

#define MAX_UNPACK_SIZE 0x44000

class CunLZSS {
protected:
  unsigned char* window;
  long inBufPtr, inBufSize, outBufPtr;
  long unpackedLen;
  unsigned char *inBuf, *outBuf;

  LZSS_API int getC();
  LZSS_API void putC (unsigned char c);
  LZSS_API void decode();
public:
  LZSS_API CunLZSS():
      unpackedLen(0),
        window (NULL),
        outBuf (NULL) {};
      LZSS_API virtual ~CunLZSS() {
        if (window) delete (window);
        if (outBuf) delete (outBuf);
      }

      LZSS_API void takeNewData (unsigned char* in, long availIn, int doUnpack);
      LZSS_API long getUnpacked (unsigned char* to, long count);
      LZSS_API long left() {return unpackedLen;};
      LZSS_API void clear() {unpackedLen = 0;};
};

#endif//LZSS_HPP_
