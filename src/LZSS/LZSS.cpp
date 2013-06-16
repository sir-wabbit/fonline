/*
 * fDAT Plug-in source code: LZSS unpacking class
 *
 * Copyright (C) 2000 Alexander Belyakov
 * E-mail: abel@krasu.ru
 *
 * Decompression function (decode) is based on LZSS32 Delphi unit:
 *    Assembler Programmer: Andy Tam, Pascal Conversion: Douglas Webb,
 *    Unit Conversion and Dynamic Memory Allocation: Andrew Eigus,
 *    Delphi 2 Porting: C.J.Rankin.
 * Variable and constant names are taken from Kevin Doherty's LZSS project.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <string.h>

#include "LZSS.hpp"

// Number of bits for index and length respectively.
#define INDEX_BC                12
#define LENGTH_BC               4

// Window size and raw lookahead size.
#define WINSIZE                 (1 << INDEX_BC)
#define RLOOK_SIZE              (1 << LENGTH_BC)

// Number of bytes to break even in compressing
#define BREAK_EVEN              2
//((1 + INDEX_BC + LENGTH_BC) / 9)
#define LOOK_SIZE               (RLOOK_SIZE + BREAK_EVEN)

#define MOD_WINDOW( a )		(( a ) & (WINSIZE - 1))

#define MAX_UNPACK_SIZE 0x44000

namespace LZSS {

int GetByte(Context* ctx) {
  if (ctx->inBufPtr >= ctx->inBufSize)
    return -1;
  return ctx->inBuf [ctx->inBufPtr++];
}

void PutByte(Context* ctx, unsigned char c) {
  // This condition must always be true, but who knows for sure.
  if (ctx->outBufPtr < MAX_UNPACK_SIZE) {
    ctx->outBuf [ctx->outBufPtr++] = c;
    ctx->unpackedLen++;
  }
}

void Decode(Context* ctx) {
  uint32_t command;
  size_t current_position;
  size_t match_position;
  size_t match_length;
  int c;

  command = 0;
  current_position = WINSIZE - LOOK_SIZE;

  while (1) {
    command >>= 1;

    if ((command & 0xFF00) == 0) {
      if ( (c = GetByte(ctx)) == -1)
        break;
      command = 0xFF00 + c;
    }

    if ( (c = GetByte(ctx)) == -1)
      break;

    if (command & 1) {
      ctx->window [current_position] = c;
      current_position = MOD_WINDOW (current_position + 1);
      PutByte(ctx, c);
    } else {
      match_position = c;
      if ( (c = GetByte(ctx)) == -1)
        break;
      match_position += ((c & 0xF0) << 4);
      match_length = (c & 0xF) + BREAK_EVEN;

      for (size_t i = 0; i <= match_length; i++) {
        c = ctx->window [MOD_WINDOW (match_position + i)];
        ctx->window [current_position] = c;
        PutByte(ctx, c);
        current_position = MOD_WINDOW (current_position + 1);
      }
    }

  }
}

}; // namespace LZSS

CunLZSS::CunLZSS() {
  memset(ctx, 0, sizeof(ctx));
}

CunLZSS::~CunLZSS() {
  if (ctx->window) {
    delete (ctx->window);
    ctx->window = NULL;
  }

  if (ctx->outBuf) {
    delete (ctx->outBuf);
    ctx->outBuf = NULL;
  }
}

void CunLZSS::takeNewData (unsigned char* in, long availIn, int doUnpack) {
  if (!ctx->window) ctx->window = new unsigned char [WINSIZE + LOOK_SIZE - 1];
  if (!ctx->outBuf) ctx->outBuf = new unsigned char [MAX_UNPACK_SIZE];

  if (doUnpack) {
    ctx->inBuf = in;
    ctx->inBufSize = availIn;
    ctx->inBufPtr = ctx->outBufPtr = 0;
    memset (ctx->window, 0x20, WINSIZE + LOOK_SIZE - 1);
    ctx->unpackedLen = 0;
    LZSS::Decode(ctx);
  } else {
    memcpy (ctx->outBuf, in, availIn);
    ctx->unpackedLen = availIn;
  }
  ctx->outBufPtr = 0;
}

long CunLZSS::getUnpacked (unsigned char* to, long count) {
  long res = (count>ctx->unpackedLen)? ctx->unpackedLen: count;
  if (res) {
    memcpy (to, ctx->outBuf+ctx->outBufPtr, res);
    ctx->outBufPtr += res;
    ctx->unpackedLen -= res;
  }
  return res;
}

long CunLZSS::left() {
  return ctx->unpackedLen;
};

void CunLZSS::clear() {
  ctx->unpackedLen = 0;
};
