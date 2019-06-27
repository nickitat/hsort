#pragma once

#include <detail/iterator_wrapper.hpp>

#include <algorithm>
#include <cstddef>
#include <iostream>

namespace hsort {

namespace detail {

template <class SeqRandomIt>
void apply_order(SeqRandomIt first, SeqRandomIt last) {
  const auto size = std::distance(first, last);
  auto me = first;
  for (auto i = 0; i < size; ++i, ++me) {
    auto dist_to_me = i;
    auto me_value{std::move(*me).value()};
    auto me_index = me->__hsort_index;
    auto next = (first + me_index);
    if (me_index != i) {
      do {
        *me = std::move(*next).value();
        me->__hsort_index = dist_to_me;
        dist_to_me = me_index;
        me_index = next->__hsort_index;
        me = next;
        next = first + me_index;
      } while (me_index != i);
      *me = std::move(me_value);
      me->__hsort_index = dist_to_me;
      me = next;
    }
  }
}

}  // namespace detail

template <class SourceType>
class hsort_base : public SourceType {
  SourceType&& value() && {
    return static_cast<SourceType&&>(*this);
  }

  void operator=(SourceType&& oth) {
    static_cast<SourceType&>(*this) = std::move(oth);
  }

  template <class SeqRandomIt>
  friend void detail::apply_order(SeqRandomIt, SeqRandomIt);

 public:
  std::size_t __hsort_index;
};

template <class RandomIt, class Comparator>
void sort_heavy(RandomIt first, RandomIt last, Comparator cmp) {
  auto comparator = [&first, &cmp](std::size_t lhs, std::size_t rhs) {
    return cmp(*(first + lhs), *(first + rhs));
  };

  std::sort(detail::wrap(first), detail::wrap(last), comparator);
  detail::apply_order(first, last);
}

}  // namespace hsort
