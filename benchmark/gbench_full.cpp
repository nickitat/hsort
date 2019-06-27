#include <hsort.hpp>

#include <benchmark/benchmark.h>

#include <algorithm>
#include <chrono>
#include <iostream>
#include <iterator>
#include <numeric>
#include <random>
#include <type_traits>
#include <vector>

#include <boost/sort/flat_stable_sort/flat_stable_sort.hpp>
#include <boost/sort/pdqsort/pdqsort.hpp>
#include <boost/sort/sort.hpp>
#include <boost/sort/spinsort/spinsort.hpp>

namespace {
static constexpr std::size_t SIZE = 9;

struct Y {
  int key;
  int data[16];
};

using X = hsort::hsort_base<Y>;

#if __cplusplus >= 201703L
static_assert(std::is_aggregate<X>::value, "");
#endif

struct Compare {
  bool operator()(const X& lhs, const X& rhs) const {
    return lhs.key < rhs.key;
  }
};
}  // namespace

template <class SortAlgo>
void BM_SortAllPermutations(benchmark::State& state, SortAlgo sort) {
  for (auto _ : state) {
    std::vector<X> input;
    for (size_t i = 0; i < SIZE; ++i) {
      X x{static_cast<int>(i), {}, i};
      input.push_back(std::move(x));
    }

    do {
      auto inputCopy = input;
      sort(inputCopy.begin(), inputCopy.end(), Compare());
    } while (std::next_permutation(input.begin(), input.end(), Compare()));
  }
}

void (*fp)(std::vector<X>::iterator,
           std::vector<X>::iterator,
           Compare) = &std::sort;
BENCHMARK_CAPTURE(BM_SortAllPermutations,
                  std_sort_tag,
                  (decltype(*fp))std::sort<std::vector<X>::iterator, Compare>)
    ->Repetitions(10)
    ->ReportAggregatesOnly();

BENCHMARK_CAPTURE(BM_SortAllPermutations,
                  sort_heavy_tag,
                  hsort::sort_heavy<std::vector<X>::iterator, Compare>)
    ->Repetitions(10)
    ->ReportAggregatesOnly();

BENCHMARK_CAPTURE(BM_SortAllPermutations,
                  boost_pdq_tag,
                  boost::sort::pdqsort<std::vector<X>::iterator, Compare>)
    ->Repetitions(10)
    ->ReportAggregatesOnly();

BENCHMARK_CAPTURE(BM_SortAllPermutations,
                  boost_spin_tag,
                  boost::sort::spinsort<std::vector<X>::iterator, Compare>)
    ->Repetitions(10)
    ->ReportAggregatesOnly();

BENCHMARK_CAPTURE(
    BM_SortAllPermutations,
    boost_iss_tag,
    boost::sort::indirect_spinsort<std::vector<X>::iterator, Compare>)
    ->Repetitions(10)
    ->ReportAggregatesOnly();

BENCHMARK_CAPTURE(
    BM_SortAllPermutations,
    boost_flat_tag,
    boost::sort::flat_stable_sort<std::vector<X>::iterator, Compare>)
    ->Repetitions(10)
    ->ReportAggregatesOnly();

BENCHMARK_CAPTURE(
    BM_SortAllPermutations,
    boost_ifs_tag,
    boost::sort::indirect_flat_stable_sort<std::vector<X>::iterator, Compare>)
    ->Repetitions(10)
    ->ReportAggregatesOnly();
