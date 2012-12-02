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

struct Context {
  uint8_t* window;
  long inBufPtr, inBufSize, outBufPtr;
  long unpackedLen;
  uint8_t* inBuf;
  uint8_t* outBuf;
};

int GetByte(Context* ctx);
void PutByte(Context* ctx, unsigned char c);
void Decode(Context* ctx);

}; // namespace LZSS

class CunLZSS {
public:
  LZSS_API CunLZSS();
  LZSS_API virtual ~CunLZSS();

  LZSS_API void takeNewData (unsigned char* in, long availIn, int doUnpack);
  LZSS_API long getUnpacked (unsigned char* to, long count);
  LZSS_API long left();
  LZSS_API void clear();

private:
  LZSS::Context* ctx;
};

#endif//LZSS_HPP_
