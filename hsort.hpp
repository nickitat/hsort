#pragma once

#include <detail/iterator_wrapper.hpp>

#include <algorithm>
#include <cstddef>
#include <iostream>

namespace hsort {

namespace detail {
template <class SeqRandomIt, class OrdRandomIt>
void apply_order(SeqRandomIt first, OrdRandomIt ofirst, OrdRandomIt olast) {
  const auto size = std::distance(ofirst, olast);
  for (auto i = 0; i < size; ++i) {
    auto& me = *(first + i);
    while (me.index != i) {
      auto& cur = *(first + me.index);
      auto& nxt = *(first + cur.index);
      using std::swap;
      swap(cur, nxt);
      swap(cur.index, nxt.index);
      swap(cur.index, me.index);
    }
  }
}

template <class SeqRandomIt, class OrdRandomIt>
void apply_order2(SeqRandomIt first, OrdRandomIt ofirst, OrdRandomIt olast) {
  const auto size = std::distance(ofirst, olast);
  for (auto i = 0; i < size; ++i) {
    auto me = (first + i);
    auto nextInd = me->index;
    auto meInd = nextInd;
    auto meValue = std::move(*me);
    if (nextInd != i) {
      do {
        auto next = (first + nextInd);
        meInd = nextInd;
        nextInd = next->index;
        *me = std::move(*next);
        me->index = meInd;
        me = next;
      } while (nextInd != i);
      *me = std::move(meValue);
      me->index = meInd;
    }
  }
}

template <class SeqRandomIt, class OrdRandomIt>
void apply_order3(SeqRandomIt first, OrdRandomIt ofirst, OrdRandomIt olast) {
  const auto size = std::distance(ofirst, olast);
  for (auto i = 0; i < size; ++i) {
    auto me = (first + i);
    auto meValue = *me;
    if (me->index != i) {
      do {
        auto next = (first + me->index);
        *me = std::move(*next);
        me->index = (me - first);
        me = next;
      } while (me->index != i);
      *me = std::move(meValue);
      me->index = (me - first);
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
  detail::apply_order2(first, wfirst, wlast);
}

}  // namespace hsort
