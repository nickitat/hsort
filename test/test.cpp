#include "../hsort.hpp"

#include <gperftools/profiler.h>

#include <algorithm>
#include <cassert>
#include <chrono>
#include <iostream>
#include <iterator>
#include <numeric>
#include <random>
#include <type_traits>
#include <vector>

namespace A {
struct X : hsort::hsort_base {
  int key;
  int data[16];

  X(std::size_t index, int key) : hsort_base{index}, key(key) {
  }

  friend std::ostream& operator<<(std::ostream& os, const X& x);
};

std::ostream& operator<<(std::ostream& os, const X& x) {
  os << x.key;
  return os;
}
}  // namespace A

int main() {
  static constexpr std::size_t size = 9;
  std::vector<A::X> input;
  for (std::size_t i = 0; i < size; ++i) {
    input.emplace_back(/*index*/ i, /*key*/ i);
  }

  auto comparator = [](const A::X& lhs, const A::X& rhs) {
    return lhs.key < rhs.key;
  };

  do {
    auto inputCopy = input;
    hsort::sort_heavy(inputCopy.begin(), inputCopy.end(), comparator);
    assert(std::is_sorted(inputCopy.begin(), inputCopy.end(), comparator));
  } while (std::next_permutation(input.begin(), input.end(), comparator));

  return 0;
}
