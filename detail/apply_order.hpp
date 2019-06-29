#include <iterator>

namespace hsort {

namespace detail {

template <class SeqRandomIt>
void apply_order(SeqRandomIt first, SeqRandomIt last) {
  using dist_t = decltype(std::distance(first, last));
  dist_t i = 0;
  for (auto me = first; me != last; ++me, ++i) {
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

}  // namespace hsort
