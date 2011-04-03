#ifndef NALL_ALGORITHM_HPP
#define NALL_ALGORITHM_HPP

#undef min
#undef max

namespace nall {
  template<typename T, typename U> T min(const T &t, const U &u) {
    return t < u ? t : u;
  }

  template<typename T, typename U> T max(const T &t, const U &u) {
    return t > u ? t : u;
  }

  template<typename T, typename U>
  inline unsigned isel_if(int cond, T a, U b) {
    int mask = (cond | (-cond)) >> 31;
    return ((unsigned)a & mask) | ((unsigned)b & ~mask);
  }

  template<typename T, typename U>
  inline unsigned isel_gez(int cond, T a, U b) {
    cond >>= 31;
    return ((unsigned)a & ~cond) | ((unsigned)b & cond);
  }
}

#endif
