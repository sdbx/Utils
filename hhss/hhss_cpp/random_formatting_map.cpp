#include "random_formatting_map.h"

const std::regex random_formatting_map::exp(R"(\$\{([^{}]*)\})");

const std::string random_formatting_map::operator()(std::string s) {
  for (auto &p : *this) {
    std::smatch match;
    auto begin = s.cbegin();
    while (regex_search(begin, s.cend(), match, exp)) {
      const std::string &next = at(match[1].str()).next();
      s.replace(match[0].first, match[0].second, next);
      begin = s.cbegin() + match.prefix().length() + next.length();
    }
  }
  return s;
}
