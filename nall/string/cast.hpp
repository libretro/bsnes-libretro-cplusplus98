#ifndef NALL_STRING_CAST_HPP
#define NALL_STRING_CAST_HPP

namespace nall {

//this is needed, as C++0x does not support explicit template specialization inside classes
template<> inline string to_string<bool>         (const bool& v)          { return v ? "true" : "false"; }
template<> inline string to_string<signed int>   (const signed int& v)    { return strsigned(v); }
template<> inline string to_string<uint64_t> (const uint64_t& v)  { return struint64_t(v); }
template<> inline string to_string<double>       (const double& v)        { return strdouble(v); }
template<> inline string to_string<char*>        (char * const & v)   { return v; }
template<> inline string to_string<string>      (const string& v)        { return v; }


template<typename T> string& string::operator= (const T& value) { return assign(to_string(value)); }
template<typename T> string& string::operator<<(const T& value) { return append(to_string(value)); }

template<typename T> lstring& lstring::operator<<(const T& value) {
  operator[](size()).assign(to_string(value));
  return *this;
}

#if defined(QSTRING_H)
template<> inline string to_string<QString>(QString v) { return v.toUtf8().constData(); }
template<> inline string to_string<const QString&>(const QString &v) { return v.toUtf8().constData(); }
string::operator QString() const { return QString::fromUtf8(*this); }
#endif

}

#endif
