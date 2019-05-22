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

struct PerfProfilingWrapper {
  PerfProfilingWrapper(const char* profileName) {
    ProfilerStart(profileName);
  }

  ~PerfProfilingWrapper() {
    ProfilerStop();
  }
};

struct X : hsort::hsort_base {
  int key;
  int data[16];

  X(std::size_t index, int key) : hsort_base{index}, key(key) {
  }
};

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
  for (std::size_t i = 0; i < size; ++i) {
    input.emplace_back(i /*index*/, i /*key*/);
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
  PerfProfilingWrapper perf(profileName);
  const auto input = GetRandomInputContainer(SIZE);
  for (auto _ : state) {
    auto container = input;
    sort(container.begin(), container.end(), Compare());
  }
}

void (*fp)(std::vector<X>::iterator,
           std::vector<X>::iterator,
           Compare) = &std::sort;
BENCHMARK_CAPTURE(BM_SortRandomInput,
                  std_sort_tag,
                  (decltype(*fp))std::sort<std::vector<X>::iterator, Compare>,
                  "profiles/random/profile_stdsort.prof");

BENCHMARK_CAPTURE(BM_SortRandomInput,
                  sort_heavy_tag,
                  hsort::sort_heavy<std::vector<X>::iterator, Compare>,
                  "profiles/random/profile_sort_heavy.prof");

BENCHMARK_CAPTURE(BM_SortRandomInput,
                  boost_pdq_tag,
                  boost::sort::pdqsort<std::vector<X>::iterator, Compare>,
                  "profiles/random/profile_boost_pdq.prof");

BENCHMARK_CAPTURE(BM_SortRandomInput,
                  boost_spin_tag,
                  boost::sort::spinsort<std::vector<X>::iterator, Compare>,
                  "profiles/random/profile_boost_spin.prof");

BENCHMARK_CAPTURE(
    BM_SortRandomInput,
    boost_iss_tag,
    boost::sort::indirect_spinsort<std::vector<X>::iterator, Compare>,
    "profiles/random/profile_boost_iss.prof");

BENCHMARK_CAPTURE(
    BM_SortRandomInput,
    boost_flat_tag,
    boost::sort::flat_stable_sort<std::vector<X>::iterator, Compare>,
    "profiles/random/profile_boost_flat.prof");

BENCHMARK_CAPTURE(
    BM_SortRandomInput,
    boost_ifs_tag,
    boost::sort::indirect_flat_stable_sort<std::vector<X>::iterator, Compare>,
    "profiles/random/profile_boost_ifs.prof");
