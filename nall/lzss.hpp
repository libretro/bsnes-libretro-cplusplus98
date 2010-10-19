#ifndef NALL_LZSS_HPP
#define NALL_LZSS_HPP

#include <nall/array.hpp>
#include <nall/new.hpp>
#include <nall/stdint.hpp>

namespace nall {
  class lzss {
  public:
    static bool encode(uint8_t *&output, uint64_t &outlength, const uint8_t *input, uint64_t inlength) {
      output = new(zeromemory) uint8_t[inlength * 9 / 8 + 9];

      uint64_t i = 0, o = 0;
      while(i < inlength) {
        uint64_t flagoffset = o++;
        uint8_t flag = 0x00;

        for(uint64_t b = 0; b < 8 && i < inlength; b++) {
          uint64_t longest = 0, pointer;
          for(uint64_t index = 1; index < 4096; index++) {
            uint64_t count = 0;
            while(true) {
              if(count >= 15 + 3) break;                               //verify pattern match is not longer than max length
              if(i + count >= inlength) break;                         //verify pattern match does not read past end of input
              if(i + count < index) break;                             //verify read is not before start of input
              if(input[i + count] != input[i + count - index]) break;  //verify pattern still matches
              count++;
            }

            if(count > longest) {
              longest = count;
              pointer = index;
            }
          }

          if(longest < 3) output[o++] = input[i++];
          else {
            flag |= 1 << b;
            uint16_t x = ((longest - 3) << 12) + pointer;
            output[o++] = x;
            output[o++] = x >> 8;
            i += longest;
          }
        }

        output[flagoffset] = flag;
      }

      outlength = o;
      return true;
    }

    static bool decode(uint8_t *&output, const uint8_t *input, uint64_t length) {
      output = new(zeromemory) uint8_t[length];

      uint64_t i = 0, o = 0;
      while(o < length) {
        uint8_t flag = input[i++];

        for(uint64_t b = 0; b < 8 && o < length; b++) {
          if(!(flag & (1 << b))) output[o++] = input[i++];
          else {
            uint16_t offset = input[i++];
            offset += input[i++] << 8;
            uint16_t lookuplength = (offset >> 12) + 3;
            offset &= 4095;
            for(uint64_t index = 0; index < lookuplength && o + index < length; index++) {
              output[o + index] = output[o + index - offset];
            }
            o += lookuplength;
          }
        }
      }

      return true;
    }
  };
}

#endif
