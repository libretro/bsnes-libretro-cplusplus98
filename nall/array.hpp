#ifndef NALL_ARRAY_HPP
#define NALL_ARRAY_HPP

#include <cstdlib>
#include <nall/C++98.hpp>
#include <nall/algorithm.hpp>
#include <nall/bit.hpp>
#include <nall/concept.hpp>
#include <nall/foreach.hpp>
#include <nall/utility.hpp>

namespace nall {
  //dynamic vector array
  //neither constructor nor destructor is ever invoked;
  //thus, this should only be used for POD objects.
  template<typename T> class array {
  protected:
    T *pool;
    uint64_t poolsize, buffersize;

  public:
    uint64_t size() const { return buffersize; }
    uint64_t capacity() const { return poolsize; }

    void reset() {
      if(pool) free(pool);
      pool = 0;
      poolsize = 0;
      buffersize = 0;
    }

    void reserve(uint64_t newsize) {
      if(newsize == poolsize) return;

      pool = (T*)realloc(pool, newsize * sizeof(T));
      poolsize = newsize;
      buffersize = min(buffersize, newsize);
    }

    void resize(uint64_t newsize) {
      if(newsize > poolsize) reserve(bit::round(newsize));  //round reserve size up to power of 2
      buffersize = newsize;
    }

    T* get(uint64_t minsize = 0) {
      if(minsize > buffersize) resize(minsize);
      if(minsize > buffersize) throw "array[] out of bounds";
      return pool;
    }

    void append(const T data) {
      operator[](buffersize) = data;
    }

    template<typename U> void insert(uint64_t index, const U list) {
      uint64_t listsize = container_size(list);
      resize(buffersize + listsize);
      memmove(pool + index + listsize, pool + index, (buffersize - index) * sizeof(T));
      foreach(item, list) pool[index++] = item;
    }

    void insert(uint64_t index, const T item) {
      insert(index, array<T>(item));
    }

    void remove(uint64_t index, uint64_t count = 1) {
      for(uint64_t i = index; count + i < buffersize; i++) {
        pool[i] = pool[count + i];
      }
      if(count + index >= buffersize) resize(index);  //every element >= index was removed
      else resize(buffersize - count);
    }

    optional<uint64_t> find(const T data) {
      for(uint64_t i = 0; i < size(); i++) if(pool[i] == data) return optional<uint64_t>(true, i);
      return optional<uint64_t>(false, 0);
    }

    void clear() {
      memset(pool, 0, buffersize * sizeof(T));
    }

    array() : pool(0), poolsize(0), buffersize(0) {
    }

    array(std::initializer_list<T> list) : pool(0), poolsize(0), buffersize(0) {
      for(const T *p = list.begin(); p != list.end(); ++p) append(*p);
    }

    ~array() {
      reset();
    }

    //copy
    array& operator=(const array &source) {
      if(pool) free(pool);
      buffersize = source.buffersize;
      poolsize = source.poolsize;
      pool = (T*)std::malloc(sizeof(T) * poolsize);            //allocate entire pool size,
      memcpy(pool, source.pool, sizeof(T) * buffersize);  //... but only copy used pool objects
      return *this;
    }

    array(const array &source) : pool(0), poolsize(0), buffersize(0) {
      operator=(source);
    }

    //index
    inline T& operator[](uint64_t index) {
      if(index >= buffersize) resize(index + 1);
      //if(index >= buffersize) throw "array[] out of bounds";
      return pool[index];
    }

    inline const T& operator[](uint64_t index) const {
      //if(index >= buffersize) throw "array[] out of bounds";
      return pool[index];
    }
  };

  template<typename T> struct has_size< array<T> > { enum { value = true }; };
}

#endif
