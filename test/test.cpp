#include "../hsort.hpp"

#include <algorithm>
#include <chrono>
#include <iostream>
#include <iterator>
#include <numeric>
#include <random>
#include <type_traits>
#include <vector>

struct X : hsort::hsort_base {
  int key;
  int data[16];

  X(std::size_t index, int key) : hsort_base{index}, key(key) {
  }
};

int main() {
  static constexpr std::size_t size = 9;
  std::vector<X> input;
  for (std::size_t i = 0; i < size; ++i) {
    input.emplace_back(/*index*/ i, /*key*/ i);
  }

  auto comparator = [](const X& lhs, const X& rhs) {
    return lhs.key < rhs.key;
  };

  do {
    auto inputCopy = input;
    hsort::sort_heavy(inputCopy.begin(), inputCopy.end(), comparator);
    assert(std::is_sorted(inputCopy.begin(), inputCopy.end(), comparator));
  } while (std::next_permutation(input.begin(), input.end(), comparator));

  return 0;
}
