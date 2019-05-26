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
    auto me_value = *me;
    if (me->index != i) {
      do {
        auto next = (first + me->index);
        *me = std::move(*next);
        me->index = (me - first);
        me = next;
      } while (me->index != i);
      *me = std::move(me_value);
      me->index = (me - first);
    }
  }
}

template <class SeqRandomIt, class OrdRandomIt>
void apply_order3(SeqRandomIt first, OrdRandomIt ofirst, OrdRandomIt olast) {
  const auto size = std::distance(ofirst, olast);
  auto me = (first);
  for (auto i = 0; i < size; ++i, ++me) {
    auto dist_to_me = i;
    auto me_value = std::move(*me);
    auto me_index = me_value.index;
    if (me_index != i) {
      auto next = (first + me_index);
      do {
        *me = std::move(*next);
        auto tmp = me_index;
        me_index = me->index;
        me->index = (dist_to_me);
        dist_to_me = tmp;
        me = next;
        next = (first + me_index);
      } while (me_index != i);
      *me = std::move(me_value);
      me->index = dist_to_me;
      me = next;
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
  detail::apply_order3(first, wfirst, wlast);
}

}  // namespace hsort
