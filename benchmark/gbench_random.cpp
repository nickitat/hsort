#include "profiling_wrapper.hpp"

#include <hsort.hpp>

#include <benchmark/benchmark.h>

#include <gperftools/profiler.h>

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
static constexpr std::size_t SIZE = 100;

struct Y {
  int key;
  int data[16];
};

using X = hsort::hsort_base<Y>;
static_assert(std::is_aggregate<X>::value, "");

struct Compare {
  bool operator()(const X& lhs, const X& rhs) const {
    return lhs.key < rhs.key;
  }
};

void ShuffleContainer(std::vector<X>& container) {
  std::random_device rd;
  std::mt19937 g(rd());
  std::shuffle(container.begin(), container.end(), g);
}

std::vector<X> PrepareRandomInputContainer(std::size_t size) {
  std::vector<X> input;
  for (std::size_t i = 0; i < SIZE; ++i) {
    X x;
    x.key = i;
    x.index = i;
    input.emplace_back(std::move(x));
  }
  ShuffleContainer(input);
  return input;
}

// all the benchmark functions should have the same (random) input
std::vector<X> GetRandomInputContainer(std::size_t size) {
  static const auto container = PrepareRandomInputContainer(size);
  return container;
}
}  // namespace

template <class SortAlgo>
void BM_SortRandomInput(benchmark::State& state,
                        SortAlgo sort,
                        const char* profileName) {
  hsort::PerfProfilingWrapper perf(profileName);
  const auto input = GetRandomInputContainer(SIZE);
  for (auto _ : state) {
    auto container = input;
    sort(container.begin(), container.end(), Compare());
  }
}

#define InstantiationOptions ReportAggregatesOnly()

void (*fp)(std::vector<X>::iterator,
           std::vector<X>::iterator,
           Compare) = &std::sort;
BENCHMARK_CAPTURE(BM_SortRandomInput,
                  std_sort_tag,
                  (decltype(*fp))std::sort<std::vector<X>::iterator, Compare>,
                  "profiles/random/profile_stdsort.prof")->InstantiationOptions;

BENCHMARK_CAPTURE(BM_SortRandomInput,
                  sort_heavy_tag,
                  hsort::sort_heavy<std::vector<X>::iterator, Compare>,
                  "profiles/random/profile_sort_heavy.prof")->InstantiationOptions;

BENCHMARK_CAPTURE(BM_SortRandomInput,
                  boost_pdq_tag,
                  boost::sort::pdqsort<std::vector<X>::iterator, Compare>,
                  "profiles/random/profile_boost_pdq.prof")->InstantiationOptions;

BENCHMARK_CAPTURE(BM_SortRandomInput,
                  boost_spin_tag,
                  boost::sort::spinsort<std::vector<X>::iterator, Compare>,
                  "profiles/random/profile_boost_spin.prof")->InstantiationOptions;

BENCHMARK_CAPTURE(
    BM_SortRandomInput,
    boost_iss_tag,
    boost::sort::indirect_spinsort<std::vector<X>::iterator, Compare>,
    "profiles/random/profile_boost_iss.prof")->InstantiationOptions;

BENCHMARK_CAPTURE(
    BM_SortRandomInput,
    boost_flat_tag,
    boost::sort::flat_stable_sort<std::vector<X>::iterator, Compare>,
    "profiles/random/profile_boost_flat.prof")->InstantiationOptions;

BENCHMARK_CAPTURE(
    BM_SortRandomInput,
    boost_ifs_tag,
    boost::sort::indirect_flat_stable_sort<std::vector<X>::iterator, Compare>,
    "profiles/random/profile_boost_ifs.prof")->InstantiationOptions;
