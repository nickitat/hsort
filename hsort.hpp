#pragma once

#include <detail/iterator_wrapper.hpp>

#include <algorithm>
#include <cstddef>
#include <type_traits>

namespace hsort {

template <class SourceType>
struct hsort_base : public SourceType {
  using index_type = std::size_t;

  index_type index;
};

namespace detail {

template <class SourceType>
SourceType&& value(hsort_base<SourceType>&& obj) noexcept {
  return static_cast<SourceType&&>(obj);
}

template <class SourceType>
void assign(hsort_base<SourceType>& obj, SourceType&& value) noexcept(
    std::is_nothrow_move_assignable<SourceType>::value) {
  static_cast<SourceType&>(obj) = std::move(value);
}

template <class RandomIt>
void apply_order(RandomIt first, RandomIt last) {
  decltype(last - first) i = 0;
  for (auto me = first; me != last; ++me, ++i) {
    auto dist_to_me = i;
    auto me_value{(value)(std::move(*me))};
    auto me_index = me->index;
    auto next = first + me_index;
    if (me_index != i) {
      do {
        (assign)(*me, (value)(std::move(*next)));
        me->index = dist_to_me;
        dist_to_me = me_index;
        me_index = next->index;
        me = next;
        next = first + me_index;
      } while (me_index != i);
      (assign)(*me, std::move(me_value));
      me->index = dist_to_me;
      me = next;
    }
  }
}

}  // namespace detail

template <class RandomIt, class Comparator>
void sort_heavy(RandomIt first, RandomIt last, Comparator cmp) {
  using index_type = typename decltype(detail::wrap(first))::value_type;

  auto comparator = [&](index_type lhs, index_type rhs) {
    return cmp(*(first + lhs), *(first + rhs));
  };

  std::sort(detail::wrap(first), detail::wrap(last), std::move(comparator));
  detail::apply_order(first, last);
}

}  // namespace hsort
