#pragma once

#include <detail/iterator_wrapper.hpp>

#include <algorithm>
#include <cstddef>

namespace hsort {

namespace detail {
template <class SeqRandomIt, class OrdRandomIt>
void apply_order(SeqRandomIt first, OrdRandomIt ofirst, OrdRandomIt olast) {
  // testing::ScopedTimer timer(testing::ScopedTimer::ApplyOrder);

  for (auto current = ofirst; current != olast; ++current) {
    // auto current = it;
    const auto target = std::distance(ofirst, current);
    // std::cout << "target: " << target << std::endl;
    while (*current != target) {
      // std::cout << *current << std::endl;
      // auto& me = *(first + (current - ofirst));
      // auto& cur = *(first + *current);
      // auto& nxt = *(first + *(ofirst + *current));

      // using std::swap;
      // std::cout << cur << " " << nxt << std::endl;
      // swap(cur, nxt);
      // std::cout << cur << " " << nxt << std::endl;
      // swap(cur.index, nxt.index);
      // std::cout << cur << " " << nxt << std::endl;
      // swap(cur.index, me.index);
      // std::cout << cur << " " << nxt << std::endl << std::endl;
      // current = ofirst + me.index;

      auto next = ofirst + *current;
      int me = current - ofirst;
      int cur = *current;
      int nxt = *next;
      using std::swap;
      // std::cout << *(first + cur) << " " << *(first + nxt) << std::endl;
      swap(*(first + cur), *(first + nxt));
      // std::cout << *(first + cur) << " " << *(first + nxt) << std::endl;
      swap((first + cur)->index, (first + nxt)->index);
      // std::cout << *(first + cur) << " " << *(first + nxt) << std::endl;
      swap((first + cur)->index, (first + me)->index);
      // std::cout << *(first + cur) << " " << *(first + nxt) << std::endl;
    }
  }
}
}  // namespace detail

struct hsort_base {
  std::size_t index;
};

template <class RandomIt, class Comparator>
void sort_heavy(RandomIt first, RandomIt last, Comparator cmp) {
  auto comparator = [&first, &cmp](std::size_t lhs, std::size_t rhs) {
    return cmp(*(first + lhs), *(first + rhs));
  };
  const auto wfirst = detail::wrap(first);
  const auto wlast = detail::wrap(last);

  std::sort(wfirst, wlast, comparator);
  detail::apply_order(first, wfirst, wlast);
}

}  // namespace hsort
