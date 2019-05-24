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

struct Compare {
  bool operator()(const X& lhs, const X& rhs) const {
    return lhs.key < rhs.key;
  }
};

std::ostream& operator<<(std::ostream& os, const X& x) {
  os << x.key;
  return os;
}
}  // namespace A

int main() {
  static constexpr std::size_t size = 10;
  std::vector<A::X> input;
  for (std::size_t i = 0; i < size; ++i) {
    input.emplace_back(/*index*/ i, /*key*/ i);
  }

  ProfilerStart("profile_cmp_with_stdsort_all_perms.prof");

  do {
    auto inputCopy = input;
    hsort::sort_heavy(inputCopy.begin(), inputCopy.end(), A::Compare());
    auto inputc = input;
    std::sort(inputc.begin(), inputc.end(), A::Compare());
  } while (std::next_permutation(input.begin(), input.end(), A::Compare()));

  ProfilerStop();

  return 0;
}
