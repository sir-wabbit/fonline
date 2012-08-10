#ifndef FO_BASE_PLATFORM_HPP_
#define FO_BASE_PLATFORM_HPP_

#define PP_STRING(a) #a
#define PP_STRING2(a) PP_STRING(a)
#define PP_CONCAT2(a,b)  a##b
#define PP_CONCAT3(a, b, c) a##b##c
#define PP_UNIQUE_LABEL_(prefix, suffix) PP_CONCAT3(__unique_, prefix, suffix)
#define PP_UNIQUE_LABEL(prefix) PP_UNIQUE_LABEL_(prefix, __LINE__)

namespace fonline { namespace detail {
  void __assert(const char* file, const char* func, unsigned int line, const char* expr, const char* fmt, ...);
}; }; // namespace fonline::platform

// The FO_ASSERT() macro is used for checking assertions, and will cause
// an immediate crash if its assertion is not met.
#define FO_ASSERT(x) fonline::detail::__assert(__FILE__, __FUNCTION__, __LINE__, #x, "Assertion failed.")
  
// The SCHECK() macro is used for checking compile-time assertions, 
// and will cause a compilation error if its assertion is not met.
// FIXME: COMPILE_CHECK/COMPILE_ASSERT/CASSERT ?
template<bool x> struct __SCHECK_F;
template<      > struct __SCHECK_F <true> {};
template<int  x> struct __SCHECK_P        {};
#define FO_COMPILE_ASSERT(B) \
  typedef __SCHECK_P<sizeof(__SCHECK_F<((B)? true : false)>)> __SCHECK_ASSERT##__LINE__

namespace fonline {

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef unsigned long long uint64;

FO_COMPILE_ASSERT(sizeof(uint8) == 1);
FO_COMPILE_ASSERT(sizeof(uint16) == 2);
FO_COMPILE_ASSERT(sizeof(uint32) == 4);
FO_COMPILE_ASSERT(sizeof(uint64) == 8);

typedef signed char int8;
typedef signed short int16;
typedef signed int int32;
typedef signed long long int64;

FO_COMPILE_ASSERT(sizeof(int8) == 1);
FO_COMPILE_ASSERT(sizeof(int16) == 2);
FO_COMPILE_ASSERT(sizeof(int32) == 4);
FO_COMPILE_ASSERT(sizeof(int64) == 8);

typedef float float32;
typedef double float64;

FO_COMPILE_ASSERT(sizeof(float32) == 4);
FO_COMPILE_ASSERT(sizeof(float64) == 8);

}; // namespace fonline

#endif//FO_BASE_PLATFORM_HPP_