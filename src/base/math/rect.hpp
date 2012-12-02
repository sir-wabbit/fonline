#ifndef FO_BASE_MATH_RECT_HPP_
#define FO_BASE_MATH_RECT_HPP_

#include <assert.h>

namespace fonline { namespace math {

template<class T>
struct Rect {
  T left;
  T right;
  T top;
  T bottom;
  
  T operator[] (size_t index) const {
    switch(index)
    {
    case 0: return left;
    case 1: return top;
    case 2: return right;
    case 3: return bottom;
    default: break;
    }
    
    assert(0 <= index && index <= 3);
    
    // XXX[2.12.2012 alex]: we have to return *something*
    return left;
  }
  
  T& operator[] (size_t index) {
    switch(index)
    {
    case 0: return left;
    case 1: return top;
    case 2: return right;
    case 3: return bottom;
    default: break;
    }

    assert(0 <= index && index <= 3);
    
    // XXX[2.12.2012 alex]: we have to return *something*
    return left;
  }
};

}} // namespace fonline::math

#endif//FO_BASE_MATH_RECT_HPP_