#pragma once

#include <cstddef>
#include <iterator>

namespace hsort {

namespace detail {

template <class _RandomIt>
class iter_wrapper {
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

  explicit iter_wrapper(_RandomIt it) : m_it(it) {
  }

  iter_wrapper& operator--() {
    --m_it;
    return *this;
  }

  iter_wrapper operator--(int) const {
    return iter_wrapper{m_it--};
  }

  iter_wrapper& operator++() {
    ++m_it;
    return *this;
  }

  iter_wrapper operator++(int) const {
    return iter_wrapper{m_it++};
  }

  iter_wrapper operator+(int n) const {
    return iter_wrapper{m_it + n};
  }

  iter_wrapper& operator+=(int n) {
    m_it += n;
    return *this;
  }

  iter_wrapper operator-(int n) const {
    return iter_wrapper{m_it - n};
  }

  iter_wrapper& operator-=(int n) {
    m_it -= n;
    return *this;
  }

  value_type& operator*() {
    return m_it->index;
  }

  const value_type& operator*() const {
    return m_it->index;
  }

  //  private:
  _RandomIt m_it;
};

template <class It>
typename It::difference_type operator-(const iter_wrapper<It>& lhs,
                                       const iter_wrapper<It>& rhs) {
  return lhs.m_it - rhs.m_it;
}

template <class It>
bool operator==(const iter_wrapper<It>& lhs, const iter_wrapper<It>& rhs) {
  return lhs.m_it == rhs.m_it;
}

template <class It>
bool operator!=(const iter_wrapper<It>& lhs, const iter_wrapper<It>& rhs) {
  return lhs.m_it != rhs.m_it;
}

template <class It>
bool operator<(const iter_wrapper<It>& lhs, const iter_wrapper<It>& rhs) {
  return lhs.m_it < rhs.m_it;
}

template <class It>
bool operator>(const iter_wrapper<It>& lhs, const iter_wrapper<It>& rhs) {
  return lhs.m_it > rhs.m_it;
}

template <class It>
bool operator>=(const iter_wrapper<It>& lhs, const iter_wrapper<It>& rhs) {
  return lhs.m_it >= rhs.m_it;
}

template <class RandomIt>
iter_wrapper<RandomIt> wrap(RandomIt it) {
  return iter_wrapper<RandomIt>{it};
}

}  // namespace detail

}  // namespace hsort
