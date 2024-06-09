#pragma once
#include "generator.h"
#include <map>
#include <regex>

class random_formatting_map
    : private std::map<std::string, generator<std::string>> {
private:
  static const std::regex exp;

public:
  void put(const std::string &key, const std::string &value) {
    (*this)[key].push(value);
  }
  const std::string operator()(std::string s);
};
