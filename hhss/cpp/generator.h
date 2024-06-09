#pragma once
#include <random>

template <typename T> class generator {
private:
  std::random_device device;
  std::mt19937 mt;

  std::vector<T> data;

public:
  generator() : mt(device()) {}
  generator(const generator &gen) : mt(device()), data(gen.data) {}
  void push(const T &s) { data.push_back(s); }
  const T &next() {
    std::uniform_int_distribution<size_t> dist(0, data.size() - 1);
    return data[dist(mt)];
  }
};
