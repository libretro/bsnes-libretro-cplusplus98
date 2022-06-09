#ifndef NALL_STRING_SPLIT_HPP
#define NALL_STRING_SPLIT_HPP

namespace nall {

void lstring::split(const char *key, const char *src, unsigned limit) {
  reset();

  int ssl = strlen(src), ksl = strlen(key);
  int lp = 0, split_count = 0;

  for(int i = 0; i <= ssl - ksl;) {
    if(!memcmp(src + i, key, ksl)) {
      strlcpy(operator[](split_count++), src + lp, i - lp + 1);
      i += ksl;
      lp = i;
      if(!--limit) break;
    } else i++;
  }

  operator[](split_count++) = src + lp;
}

};

#endif
