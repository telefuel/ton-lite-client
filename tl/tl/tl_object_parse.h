#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "tl/TlObject.h"

#include "td/utils/int_types.h"

namespace ton {

template <class Func, std::int32_t constructor_id>
class TlFetchBoxed {
 public:
  template <class Parser>
  static auto parse(Parser &p) -> decltype(Func::parse(p)) {
    if (p.fetch_int() != constructor_id) {
      p.set_error("Wrong constructor found");
      return decltype(Func::parse(p))();
    }
    return Func::parse(p);
  }
};

class TlFetchTrue {
 public:
  template <class Parser>
  static bool parse(Parser &p) {
    return true;
  }
};

class TlFetchBool {
 public:
  template <class Parser>
  static bool parse(Parser &p) {
    constexpr std::int32_t ID_BOOL_FALSE = 0xbc799737;
    constexpr std::int32_t ID_BOOL_TRUE = 0x997275b5;

    std::int32_t c = p.fetch_int();
    if (c == ID_BOOL_TRUE) {
      return true;
    }
    if (c != ID_BOOL_FALSE) {
      p.set_error("Bool expected");
    }
    return false;
  }
};

class TlFetchInt {
 public:
  template <class Parser>
  static std::int32_t parse(Parser &p) {
    return p.fetch_int();
  }
};

class TlFetchLong {
 public:
  template <class Parser>
  static std::int64_t parse(Parser &p) {
    return p.fetch_long();
  }
};

class TlFetchDouble {
 public:
  template <class Parser>
  static double parse(Parser &p) {
    return p.fetch_double();
  }
};

class TlFetchInt128 {
 public:
  template <class Parser>
  static td::UInt128 parse(Parser &p) {
    return p.template fetch_binary<td::UInt128>();
  }
};

class TlFetchInt256 {
 public:
  template <class Parser>
  static td::UInt256 parse(Parser &p) {
    return p.template fetch_binary<td::UInt256>();
  }
};

template <class T>
class TlFetchString {
 public:
  template <class Parser>
  static T parse(Parser &p) {
    return p.template fetch_string<T>();
  }
};

template <class T>
class TlFetchBytes {
 public:
  template <class Parser>
  static T parse(Parser &p) {
    return p.template fetch_string<T>();
  }
};

template <class Func>
class TlFetchVector {
 public:
  template <class Parser>
  static auto parse(Parser &p) -> std::vector<decltype(Func::parse(p))> {
    const std::uint32_t multiplicity = p.fetch_int();
    std::vector<decltype(Func::parse(p))> v;
    if (p.get_left_len() < multiplicity) {
      p.set_error("Wrong vector length");
    } else {
      v.reserve(multiplicity);
      for (std::uint32_t i = 0; i < multiplicity; i++) {
        v.push_back(Func::parse(p));
      }
    }
    return v;
  }
};

template <class T>
class TlFetchObject {
 public:
  template <class Parser>
  static tl_object_ptr<T> parse(Parser &p) {
    return move_tl_object_as<T>(T::fetch(p));
  }
};

}  // namespace ton
