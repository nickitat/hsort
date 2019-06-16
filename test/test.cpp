#include "../hsort.hpp"

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
struct Y {
  int key;
  int data[16];
};

using X = hsort::hsort_base<Y>;

#if __cplusplus >= 201703L
static_assert(std::is_aggregate<X>::value, "");
#endif

std::ostream& operator<<(std::ostream& os, const X& x) {
  os << x.key;
  return os;
}
}  // namespace A

int main() {
  static constexpr std::size_t size = 9;
  std::vector<A::X> input;
  for (int i = 0; i < size; ++i) {
    A::X x;
    x.key = i;
    x.index = i;
    input.push_back(std::move(x));
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
