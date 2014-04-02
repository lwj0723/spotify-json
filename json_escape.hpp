// Copyright (c) 2014 Felix Bruns and Johan Lindstr�m.

#pragma once
#ifndef JSON_ESCAPE_HPP_
#define JSON_ESCAPE_HPP_

#include <ostream>
#include <string>

namespace json {

/**
 * \brief Simple end iterator for null-terminated strings.
 */
class null_terminated_end_iterator {
 public:
  bool operator !=(const char *it) const {
    return *it != '\0';
  }
};

template<typename OutputType>
struct escape_traits {
  static void write(OutputType &out, const char *data, size_t length) {
    out.write(data, length);
  }
  static void put(OutputType &out, char ch) {
    out.put(ch);
  }
};

template<>
void escape_traits<std::string>::write(
    std::string &out, const char *data, size_t length) {
  out.append(data, length);
}

template<>
void escape_traits<std::string>::put(
    std::string &out, char ch) {
  out.push_back(ch);
}

/**
  * \brief Escape a string for use in a JSON string as per RFC 4627.
  *
  * This escapes control characters (0x00 through 0x1F), as well as
  * backslashes and quotation marks.
  *
  * See: http://www.ietf.org/rfc/rfc4627.txt (Section 2.5)
  */
template<typename OutputType, typename InputIterator, typename InputEndIterator>
inline OutputType &write_escaped(OutputType &out, const InputIterator &begin, const InputEndIterator &end) {
  typedef escape_traits<OutputType> traits;

  static const char *HEX = "0123456789ABCDEF";

  for (InputIterator it = begin; end != it; ++it) {
    const unsigned char ch = static_cast<unsigned char>(*it);
    const bool is_control_character(ch < 0x20);
    const bool is_special_character(ch == '\\' || ch == '"');

    if (is_control_character) {
      traits::write(out, "\\u00", 4);
      traits::put(out, HEX[(ch >> 4)]);
      traits::put(out, HEX[(ch & 0x0F)]);
    } else if (is_special_character) {
      traits::put(out, '\\');
      traits::put(out, ch);
    } else {
      traits::put(out, ch);
    }
  }

  return out;
}

}  // namespace json

#endif  // JSON_ESCAPE_HPP_
