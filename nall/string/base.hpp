#ifndef NALL_STRING_BASE_HPP
#define NALL_STRING_BASE_HPP

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <nall/concept.hpp>
#include <nall/stdint.hpp>
#include <nall/utf8.hpp>
#include <nall/vector.hpp>

namespace nall {
  class string;

  class string {
  public:
    inline void reserve(uint64_t);
    inline uint64_t length() const;

    inline string& assign(const char*);
    inline string& append(const char*);
    inline string& append(bool);
    inline string& append(signed int value);
    inline string& append(uint64_t value);
    inline string& append(double value);

    template<typename T> inline string& operator= (const T& value);
    template<typename T> inline string& operator<<(const T& value);

    inline operator const char*() const;
    inline char* operator()();
    inline char& operator[](int);

    inline bool operator==(const char*) const;
    inline bool operator!=(const char*) const;
    inline bool operator< (const char*) const;
    inline bool operator<=(const char*) const;
    inline bool operator> (const char*) const;
    inline bool operator>=(const char*) const;

    inline string& operator=(const string&);

    inline string();

    template<typename T1>
    inline string(T1 const &);
    
    template<typename T1, typename T2>
    inline string(T1 const &, T2 const &);

    template<typename T1, typename T2, typename T3>
    inline string(T1 const &, T2 const &, T3 const &);

    template<typename T1, typename T2, typename T3, typename T4>
    inline string(T1 const &, T2 const &, T3 const &, T4 const &);

    template<typename T1, typename T2, typename T3, typename T4, typename T5>
    inline string(T1 const &, T2 const &, T3 const &, T4 const &, T5 const &);

    template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
    inline string(T1 const &, T2 const &, T3 const &, T4 const &, T5 const &, T6 const &);

    template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
    inline string(T1 const &, T2 const &, T3 const &, T4 const &, T5 const &, T6 const &, T7 const &);

    template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
    inline string(T1 const &, T2 const &, T3 const &, T4 const &, T5 const &, T6 const &, T7 const &, T8 const &);

    inline string(string const &);

    inline ~string();

    inline bool readfile(const char*);
    inline string& replace (const char*, const char*);
    inline string& qreplace(const char*, const char*);

    inline string& lower();
    inline string& upper();
    inline string& transform(const char *before, const char *after);
    inline string& ltrim(const char *key = " ");
    inline string& rtrim(const char *key = " ");
    inline string& trim (const char *key = " ");
    inline string& ltrim_once(const char *key = " ");
    inline string& rtrim_once(const char *key = " ");
    inline string& trim_once (const char *key = " ");

  protected:
    char *data;
    uint64_t size;

  #if defined(QSTRING_H)
  public:
    inline operator QString() const;
  #endif
  };

  template<typename T> inline string to_string(const T& t)
  {
     return t;
  }

  class lstring : public linear_vector<string> {
  public:
    template<typename T> inline lstring& operator<<(const T& value);

    inline optional<uint64_t> find(const char*);
    inline void split (const char*, const char*, uint64_t = 0);
    inline void qsplit(const char*, const char*, uint64_t = 0);

    lstring();
    
    lstring(const string &);
    lstring(const string &, const string &);
    lstring(const string &, const string &, const string &);
    lstring(const string &, const string &, const string &, const string &);
    lstring(const string &, const string &, const string &, const string &, const string &);
    lstring(const string &, const string &, const string &, const string &, const string &, const string &);
    lstring(const string &, const string &, const string &, const string &, const string &, const string &, const string &);
    lstring(const string &, const string &, const string &, const string &, const string &, const string &, const string &, const string &);
    
    lstring(const lstring & str);
    lstring(std::initializer_list<string>);
  };

  //compare.hpp
  inline char chrlower(char c);
  inline char chrupper(char c);
  inline int stricmp(const char *dest, const char *src);
  inline bool strbegin (const char *str, const char *key);
  inline bool stribegin(const char *str, const char *key);
  inline bool strend (const char *str, const char *key);
  inline bool striend(const char *str, const char *key);

  //convert.hpp
  inline char* strlower(char *str);
  inline char* strupper(char *str);
  inline char* strtr(char *dest, const char *before, const char *after);
  inline uintmax_t strhex     (const char *str);
  inline intmax_t  strsigned  (const char *str);
  inline uintmax_t struint64_t(const char *str);
  inline uintmax_t strbin     (const char *str);
  inline double    strdouble  (const char *str);

  //match.hpp
  inline bool match(const char *pattern, const char *str);

  //math.hpp
  inline bool strint (const char *str, int &result);
  inline bool strmath(const char *str, int &result);

  //strl.hpp
  inline uint64_t strlcpy(char *dest, const char *src, uint64_t length);
  inline uint64_t strlcat(char *dest, const char *src, uint64_t length);

  //trim.hpp
  inline char* ltrim(char *str, const char *key = " ");
  inline char* rtrim(char *str, const char *key = " ");
  inline char* trim (char *str, const char *key = " ");
  inline char* ltrim_once(char *str, const char *key = " ");
  inline char* rtrim_once(char *str, const char *key = " ");
  inline char* trim_once (char *str, const char *key = " ");

  //utility.hpp
  inline uint64_t strlcpy(string &dest, const char *src, uint64_t length);
  inline uint64_t strlcat(string &dest, const char *src, uint64_t length);
  inline string substr(const char *src, uint64_t start = 0, uint64_t length = 0);
  inline string& strtr(string &dest, const char *before, const char *after);
  
  template<uint64_t length, char padding> inline string strhex     (uintmax_t value);
  template<uint64_t length, char padding> inline string strsigned  (intmax_t  value);
  template<uint64_t length, char padding> inline string struint64_t(uintmax_t value);
  template<uint64_t length, char padding> inline string strbin     (uintmax_t value);
  template<uint64_t length>               inline string strhex     (uintmax_t value) { return strhex      <length, '0'> (value); }
  template<uint64_t length>               inline string strsigned  (intmax_t  value) { return strsigned   <length, '0'> (value); }
  template<uint64_t length>               inline string struint64_t(uintmax_t value) { return struint64_t <length, '0'> (value); }
  template<uint64_t length>               inline string strbin     (uintmax_t value) { return strbin      <length, '0'> (value); }
                                          inline string strhex     (uintmax_t value) { return strhex      <0>           (value); }
                                          inline string strsigned  (intmax_t  value) { return strsigned   <0>           (value); }
                                          inline string struint64_t(uintmax_t value) { return struint64_t <0>           (value); }
                                          inline string strbin     (uintmax_t value) { return strbin      <0>           (value); }
  
  inline uint64_t strdouble(char *str, double value);
  inline string strdouble(double value);

  //variadic.hpp
  template <typename T1>
  inline void print(T1 const &);

  template <typename T1, typename T2>
  inline void print(T1 const &, T2 const &);

  template <typename T1, typename T2, typename T3>
  inline void print(T1 const &, T2 const &, T3 const &);

  template <typename T1, typename T2, typename T3, typename T4>
  inline void print(T1 const &, T2 const &, T3 const &, T4 const &);

  template <typename T1, typename T2, typename T3, typename T4, typename T5>
  inline void print(T1 const &, T2 const &, T3 const &, T4 const &, T5 const &);

  template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
  inline void print(T1 const &, T2 const &, T3 const &, T4 const &, T5 const &, T6 const &);

  template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
  inline void print(T1 const &, T2 const &, T3 const &, T4 const &, T5 const &, T6 const &, T7 const &);

  template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
  inline void print(T1 const &, T2 const &, T3 const &, T4 const &, T5 const &, T6 const &, T7 const &, T8 const &);
};

#endif
