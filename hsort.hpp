#pragma once

#include <detail/apply_order.hpp>
#include <detail/iterator_wrapper.hpp>

#include <algorithm>
#include <cstddef>

namespace hsort {

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
