#ifndef NALL_STRING_STRPOS_HPP
#define NALL_STRING_STRPOS_HPP

//usage example:
//if(auto pos = strpos(str, key)) print(pos(), "\n");
//prints position of key within str, only if it is found

namespace nall {

inline optional<uint64_t> strpos(const char *str, const char *key) {
  uint64_t ssl = strlen(str), ksl = strlen(key);
  if(ksl > ssl) return optional<uint64_t>(false, 0);

  for(uint64_t i = 0; i <= ssl - ksl; i++) {
    if(!memcmp(str + i, key, ksl)) return optional<uint64_t>(true, i);
  }

  return optional<uint64_t>(false, 0);
}

inline optional<uint64_t> qstrpos(const char *str, const char *key) {
  uint64_t ssl = strlen(str), ksl = strlen(key);
  if(ksl > ssl) return optional<uint64_t>(false, 0);

  for(uint64_t i = 0; i <= ssl - ksl;) {
    uint8_t x = str[i];
    if(x == '\"' || x == '\'') {
      uint8_t z = i++;
      while(str[i] != x && i < ssl) i++;
      if(i >= ssl) i = z;
    }
    if(!memcmp(str + i, key, ksl)) return optional<uint64_t>(true, i);
    i++;
  }

  return optional<uint64_t>(false, 0);
}

}

#endif
