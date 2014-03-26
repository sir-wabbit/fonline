#pragma once

#include <cstddef>

#pragma warning (disable : 4786)

template<class T> void SafeRelease(T*& x) {
  if (x != NULL) {
    x->Release();
    x = NULL;
  }
}
template<class T> void SafeDelete(T*& x) {
  if (x != NULL) {
    delete x;
    x = NULL;
  }
}
template<class T> void SafeDeleteArray(T*& x) {
  if (x != NULL) {
    delete[] x;
    x = NULL;
  }
}

template<class T, class U> T GetBits(T const val, U const mask) {
  return T(val & mask);
}
template<class T, class U> T SetBits(T& val, U const mask) {
  val = T(val | mask);
}
template<class T, class U> T ClearBits(T& val, U const mask) {
  val = T((val | mask) ^ mask);
}
