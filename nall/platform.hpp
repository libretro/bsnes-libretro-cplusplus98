#ifndef NALL_PLATFORM_HPP
#define NALL_PLATFORM_HPP

#include <nall/utf8.hpp>

//=========================
//standard platform headers
//=========================

#include <limits>

#include <assert.h>
#include <limits.h>
#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#if defined(_WIN32)
#if defined(_WIN64) || defined(_MSC_VER) // Odd quirk
  #define interface struct
#endif
  #include <io.h>
  #include <direct.h>
  #include <shlobj.h>
  #undef interface
#else
  #include <unistd.h>
  #include <sys/stat.h>
#endif

//==================
//warning supression
//==================

//Visual C++
#if defined(_MSC_VER)
  //disable libc "deprecation" warnings
  #pragma warning(disable:4996)
#endif

//================
//POSIX compliance
//================

#if defined(_WIN32)
  #define ftruncate   _chsize
#endif

//================
//inline expansion
//================

#if defined(__GNUC__)
  #define noinline      __attribute__((noinline))
  #define inline        inline
  #define alwaysinline  inline __attribute__((always_inline))
#elif defined(_MSC_VER)
  #define noinline      __declspec(noinline)
  #define inline        inline
  #define alwaysinline  inline __forceinline
#else
  #define noinline
  #define inline        inline
  #define alwaysinline  inline
#endif

#endif

