#pragma once

#include <cstddef>
#include <iterator>

namespace hsort {

namespace detail {

template <class _RandomIt>
class iter_wrapper : public _RandomIt {
 public:
  using iterator_category = std::random_access_iterator_tag;
  using value_type = std::size_t;
  using difference_type =
      typename std::iterator_traits<_RandomIt>::difference_type;
  using pointer = typename std::iterator_traits<_RandomIt>::pointer;
  using reference = typename std::iterator_traits<_RandomIt>::reference;

  static_assert(
      std::is_same<
          typename std::iterator_traits<iter_wrapper>::iterator_category,
          std::random_access_iterator_tag>::value,
      "iter_wrapper should be a random access iterator.");

  iter_wrapper(_RandomIt it) : _RandomIt(it) {
  }

  iter_wrapper& operator--() {
    _RandomIt::operator--();
    return *this;
  }

  iter_wrapper operator--(int) {
    return _RandomIt::operator--(0);
  }

  iter_wrapper& operator++() {
    _RandomIt::operator++();
    return *this;
  }

  iter_wrapper operator++(int) {
    return _RandomIt::operator++(0);
  }

  iter_wrapper operator+(int n) const {
    return _RandomIt::operator+(n);
  }

  iter_wrapper& operator+=(int n) {
    _RandomIt::operator+=(n);
    return *this;
  }

  iter_wrapper operator-(int n) const {
    return _RandomIt::operator-(n);
  }

  iter_wrapper& operator-=(int n) {
    _RandomIt::operator-=(n);
    return *this;
  }

  value_type& operator*() const {
    return (_RandomIt::operator*()).index;
  }
};

template <class RandomIt>
iter_wrapper<RandomIt> wrap(RandomIt it) {
  return {it};
}

}  // namespace detail

}  // namespace hsort
