#ifndef NALL_STRING_UTILITY_HPP
#define NALL_STRING_UTILITY_HPP

namespace nall {

inline unsigned strlcpy(string &dest, const char *src, unsigned length) {
  dest.reserve(length);
  return strlcpy(dest(), src, length);
}

inline unsigned strlcat(string &dest, const char *src, unsigned length) {
  dest.reserve(length);
  return strlcat(dest(), src, length);
}

inline string substr(const char *src, unsigned start, unsigned length) {
  string dest;
  if(length == 0) {
    //copy entire string
    dest = src + start;
  } else {
    //copy partial string
    strlcpy(dest, src + start, length + 1);
  }
  return dest;
}

/* arithmetic <> string */

inline string integer(intmax_t value) {
  bool negative = value < 0;
  if(negative) value = abs(value);

  char buffer[64];
  unsigned size = 0;

  do {
    unsigned n = value % 10;
    buffer[size++] = '0' + n;
    value /= 10;
  } while(value);
  buffer[size++] = negative ? '-' : '+';
  buffer[size] = 0;

  char result[size + 1];
  memset(result, '0', size);
  result[size] = 0;

  for(signed x = size - 1, y = 0; x >= 0 && y < size; x--, y++) {
    result[x] = buffer[y];
  }

  return &result[0];
}

template<unsigned length> inline string linteger(intmax_t value) {
  bool negative = value < 0;
  if(negative) value = abs(value);

  char buffer[64];
  unsigned size = 0;

  do {
    unsigned n = value % 10;
    buffer[size++] = '0' + n;
    value /= 10;
  } while(value);
  buffer[size++] = negative ? '-' : '+';
  buffer[size] = 0;

  char result[length + 1];
  memset(result, ' ', length);
  result[length] = 0;

  for(signed x = 0, y = size - 1; x < length && y >= 0; x++, y--) {
    result[x] = buffer[y];
  }

  return result;
}

template<unsigned length> inline string rinteger(intmax_t value) {
  bool negative = value < 0;
  if(negative) value = abs(value);

  char buffer[64];
  unsigned size = 0;

  do {
    unsigned n = value % 10;
    buffer[size++] = '0' + n;
    value /= 10;
  } while(value);
  buffer[size++] = negative ? '-' : '+';
  buffer[size] = 0;

  char result[length + 1];
  memset(result, ' ', length);
  result[length] = 0;

  for(signed x = length - 1, y = 0; x >= 0 && y < size; x--, y++) {
    result[x] = buffer[y];
  }

  return result;
}

inline string decimal(uintmax_t value) {
  char buffer[64];
  unsigned size = 0;

  do {
    unsigned n = value % 10;
    buffer[size++] = '0' + n;
    value /= 10;
  } while(value);
  buffer[size] = 0;

  char result[size + 1];
  memset(result, '0', size);
  result[size] = 0;

  for(signed x = size - 1, y = 0; x >= 0 && y < size; x--, y++) {
    result[x] = buffer[y];
  }

  return &result[0];
}

template<unsigned length> inline string ldecimal(uintmax_t value) {
  char buffer[64];
  unsigned size = 0;

  do {
    unsigned n = value % 10;
    buffer[size++] = '0' + n;
    value /= 10;
  } while(value);
  buffer[size] = 0;

  char result[length + 1];
  memset(result, ' ', length);
  result[length] = 0;

  for(signed x = 0, y = size - 1; x < length && y >= 0; x++, y--) {
    result[x] = buffer[y];
  }

  return &result[0];
}

template<unsigned length> inline string rdecimal(uintmax_t value) {
  char buffer[64];
  unsigned size = 0;

  do {
    unsigned n = value % 10;
    buffer[size++] = '0' + n;
    value /= 10;
  } while(value);
  buffer[size] = 0;

  char result[length + 1];
  memset(result, ' ', length);
  result[length] = 0;

  for(signed x = length - 1, y = 0; x >= 0 && y < size; x--, y++) {
    result[x] = buffer[y];
  }

  return &result[0];
}

template<unsigned length> inline string hex(uintmax_t value) {
  string output;
  unsigned offset = 0;

  //render string backwards, as we do not know its length yet
  do {
    unsigned n = value & 15;
    output[offset++] = n < 10 ? '0' + n : 'a' + n - 10;
    value >>= 4;
  } while(value);

  while(offset < length) output[offset++] = '0';
  output[offset--] = 0;

  //reverse the string in-place
  for(unsigned i = 0; i < (offset + 1) >> 1; i++) {
    char temp = output[i];
    output[i] = output[offset - i];
    output[offset - i] = temp;
  }

  return &output[0];
}

template<unsigned length> inline string binary(uintmax_t value) {
  string output;
  unsigned offset = 0;

  do {
    unsigned n = value & 1;
    output[offset++] = '0' + n;
    value >>= 1;
  } while(value);

  while(offset < length) output[offset++] = '0';
  output[offset--] = 0;

  for(unsigned i = 0; i < (offset + 1) >> 1; i++) {
    char temp = output[i];
    output[i] = output[offset - i];
    output[offset - i] = temp;
  }

  return &output[0];
}

//using sprintf is certainly not the most ideal method to convert
//a double to a string ... but attempting to parse a double by
//hand, digit-by-digit, results in subtle rounding errors.
inline unsigned fp(char *str, double value) {
  char buffer[256];
  sprintf(buffer, "%f", value);

  //remove excess 0's in fraction (2.500000 -> 2.5)
  for(char *p = buffer; *p; p++) {
    if(*p == '.') {
      char *p = buffer + strlen(buffer) - 1;
      while(*p == '0') {
        if(*(p - 1) != '.') *p = 0;  //... but not for eg 1.0 -> 1.
        p--;
      }
      break;
    }
  }

  unsigned length = strlen(buffer);
  if(str) strcpy(str, buffer);
  return length + 1;
}

inline string fp(double value) {
  string temp;
  temp.reserve(fp(0, value));
  fp(temp(), value);
  return temp;
}

}

#endif
