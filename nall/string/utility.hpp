#ifndef NALL_STRING_UTILITY_HPP
#define NALL_STRING_UTILITY_HPP

namespace nall {

uint64_t strlcpy(string &dest, const char *src, uint64_t length) {
  dest.reserve(length);
  return strlcpy(dest(), src, length);
}

uint64_t strlcat(string &dest, const char *src, uint64_t length) {
  dest.reserve(length);
  return strlcat(dest(), src, length);
}

string substr(const char *src, uint64_t start, uint64_t length) {
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

template<uint64_t length, char padding> string strhex(uintmax_t value) {
  string output;
  uint64_t offset = 0;

  //render string backwards, as we do not know its length yet
  do {
    uint64_t n = value & 15;
    output[offset++] = n < 10 ? '0' + n : 'a' + n - 10;
    value >>= 4;
  } while(value);

  while(offset < length) output[offset++] = padding;
  output[offset--] = 0;

  //reverse the string in-place
  for(uint64_t i = 0; i < (offset + 1) >> 1; i++) {
    char temp = output[i];
    output[i] = output[offset - i];
    output[offset - i] = temp;
  }

  return output;
}

template<uint64_t length, char padding> string strsigned(intmax_t value) {
  string output;
  uint64_t offset = 0;

  bool negative = value < 0;
  if(negative) value = abs(value);

  do {
    uint64_t n = value % 10;
    output[offset++] = '0' + n;
    value /= 10;
  } while(value);

  while(offset < length) output[offset++] = padding;
  if(negative) output[offset++] = '-';
  output[offset--] = 0;

  for(uint64_t i = 0; i < (offset + 1) >> 1; i++) {
    char temp = output[i];
    output[i] = output[offset - i];
    output[offset - i] = temp;
  }

  return output;
}

template<uint64_t length, char padding> string struint64_t(uintmax_t value) {
  string output;
  uint64_t offset = 0;

  do {
    uint64_t n = value % 10;
    output[offset++] = '0' + n;
    value /= 10;
  } while(value);

  while(offset < length) output[offset++] = padding;
  output[offset--] = 0;

  for(uint64_t i = 0; i < (offset + 1) >> 1; i++) {
    char temp = output[i];
    output[i] = output[offset - i];
    output[offset - i] = temp;
  }

  return output;
}

template<uint64_t length, char padding> string strbin(uintmax_t value) {
  string output;
  uint64_t offset = 0;

  do {
    uint64_t n = value & 1;
    output[offset++] = '0' + n;
    value >>= 1;
  } while(value);

  while(offset < length) output[offset++] = padding;
  output[offset--] = 0;

  for(uint64_t i = 0; i < (offset + 1) >> 1; i++) {
    char temp = output[i];
    output[i] = output[offset - i];
    output[offset - i] = temp;
  }

  return output;
}

//using sprintf is certainly not the most ideal method to convert
//a double to a string ... but attempting to parse a double by
//hand, digit-by-digit, results in subtle rounding errors.
uint64_t strdouble(char *str, double value) {
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

  uint64_t length = strlen(buffer);
  if(str) strcpy(str, buffer);
  return length + 1;
}

string strdouble(double value) {
  string temp;
  temp.reserve(strdouble(0, value));
  strdouble(temp(), value);
  return temp;
}

}

#endif
