#include "profiling_wrapper.hpp"

#include <hsort.hpp>

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
struct Y {
  int key;
  int data[16];
};

using X = hsort::hsort_base<Y>;

#if __cplusplus >= 201703L
static_assert(std::is_aggregate<X>::value, "");
#endif

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
  for (size_t i = 0; i < size; ++i) {
    A::X x{static_cast<int>(i), {}, i};
    input.push_back(std::move(x));
  }

  hsort::PerfProfilingWrapper perf("profile_cmp_with_stdsort_all_perms.prof");

  do {
    auto inputCopy = input;
    hsort::sort_heavy(inputCopy.begin(), inputCopy.end(), A::Compare());
    auto inputd = input;
    std::sort(inputd.begin(), inputd.end(), A::Compare());
  } while (std::next_permutation(input.begin(), input.end(), A::Compare()));

  return 0;
}
