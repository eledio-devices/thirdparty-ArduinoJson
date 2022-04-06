// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2022, Benoit BLANCHON
// MIT License

#pragma once

#include "remove_cv.hpp"

namespace ARDUINOJSON_NAMESPACE {

template <typename T>
struct is_signed {
  typedef typename remove_cv<T>::type type;
  static const bool value = type(-1) < type(0);
};

}  // namespace ARDUINOJSON_NAMESPACE
