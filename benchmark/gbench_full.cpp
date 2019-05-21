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
struct X : hsort::hsort_base {
  int key;
  int data[16];

  X(std::size_t index, int key) : hsort_base{index}, key(key) {
  }
};

std::vector<X> PrepareInputContainer(std::size_t size) {
  std::vector<X> input;
  for (std::size_t i = 0; i < size; ++i) {
    input.emplace_back(i /*index*/, i /*key*/);
  }
  return input;
}

void ShuffleContainer(std::vector<X>& container) {
  std::random_device rd;
  std::mt19937 g(rd());
  std::shuffle(container.begin(), container.end(), g);
}
}  // namespace

static void SortHeavy(benchmark::State& state) {
  for (auto _ : state) {
    static constexpr std::size_t size = 9;
    std::vector<X> input;
    for (std::size_t i = 0; i < size; ++i) {
      input.emplace_back(/*index*/ i, /*key*/ i);
    }

    auto comparator = [](const X& lhs, const X& rhs) {
      return lhs.key < rhs.key;
    };

    do {
      auto inputCopy = input;
      hsort::sort_heavy(inputCopy.begin(), inputCopy.end(), comparator);
      assert(std::is_sorted(inputCopy.begin(), inputCopy.end(), comparator));
    } while (std::next_permutation(input.begin(), input.end(), comparator));
  }
}
BENCHMARK(SortHeavy)->Repetitions(10)->ReportAggregatesOnly();

static void StdSort(benchmark::State& state) {
  for (auto _ : state) {
    static constexpr std::size_t size = 9;
    std::vector<X> input;
    for (std::size_t i = 0; i < size; ++i) {
      input.emplace_back(/*index*/ i, /*key*/ i);
    }

    auto comparator = [](const X& lhs, const X& rhs) {
      return lhs.key < rhs.key;
    };

    do {
      auto inputCopy = input;
      std::sort(inputCopy.begin(), inputCopy.end(), comparator);
      assert(std::is_sorted(inputCopy.begin(), inputCopy.end(), comparator));
    } while (std::next_permutation(input.begin(), input.end(), comparator));
  }
}
BENCHMARK(StdSort)->Repetitions(10)->ReportAggregatesOnly();

// static void BoostPdqSort(benchmark::State& state) {
//   auto comparator = [](const X& lhs, const X& rhs) {
//     return lhs.key < rhs.key;
//   };

//   const auto size = state.range(0);
//   auto container = PrepareInputContainer(size);
//   for (auto _ : state) {
//     state.PauseTiming();
//     ShuffleContainer(container);
//     state.ResumeTiming();
//     boost::sort::pdqsort(container.begin(), container.end(), comparator);
//   }
// }
// BENCHMARK(BoostPdqSort)
//     ->Arg(100)
//     // ->Repetitions(10)
//     ->ReportAggregatesOnly();

// static void BoostSpinSort(benchmark::State& state) {
//   auto comparator = [](const X& lhs, const X& rhs) {
//     return lhs.key < rhs.key;
//   };

//   const auto size = state.range(0);
//   auto container = PrepareInputContainer(size);
//   for (auto _ : state) {
//     state.PauseTiming();
//     ShuffleContainer(container);
//     state.ResumeTiming();
//     boost::sort::spinsort(container.begin(), container.end(), comparator);
//   }
// }
// BENCHMARK(BoostSpinSort)
//     ->Arg(100)
//     // ->Repetitions(10)
//     ->ReportAggregatesOnly();

static void IndirBoostSpinSort(benchmark::State& state) {
  for (auto _ : state) {
    static constexpr std::size_t size = 9;
    std::vector<X> input;
    for (std::size_t i = 0; i < size; ++i) {
      input.emplace_back(/*index*/ i, /*key*/ i);
    }

    auto comparator = [](const X& lhs, const X& rhs) {
      return lhs.key < rhs.key;
    };

    do {
      auto inputCopy = input;
      boost::sort::indirect_spinsort(inputCopy.begin(), inputCopy.end(),
                                     comparator);
      assert(std::is_sorted(inputCopy.begin(), inputCopy.end(), comparator));
    } while (std::next_permutation(input.begin(), input.end(), comparator));
  }
}
BENCHMARK(IndirBoostSpinSort)->Repetitions(10)->ReportAggregatesOnly();

// static void BoostFlatStableSort(benchmark::State& state) {
//   auto comparator = [](const X& lhs, const X& rhs) {
//     return lhs.key < rhs.key;
//   };

//   const auto size = state.range(0);
//   auto container = PrepareInputContainer(size);
//   for (auto _ : state) {
//     state.PauseTiming();
//     ShuffleContainer(container);
//     state.ResumeTiming();
//     boost::sort::flat_stable_sort(container.begin(), container.end(),
//                                   comparator);
//   }
// }
// BENCHMARK(BoostFlatStableSort)
//     ->Arg(100)
//     // ->Repetitions(10)
//     ->ReportAggregatesOnly();

// static void IndirBoostFlatStableSort(benchmark::State& state) {
//   auto comparator = [](const X& lhs, const X& rhs) {
//     return lhs.key < rhs.key;
//   };

//   const auto size = state.range(0);
//   auto container = PrepareInputContainer(size);
//   for (auto _ : state) {
//     state.PauseTiming();
//     ShuffleContainer(container);
//     state.ResumeTiming();
//     boost::sort::indirect_flat_stable_sort(container.begin(),
//     container.end(),
//                                            comparator);
//   }
// }
// BENCHMARK(IndirBoostFlatStableSort)
//     ->Arg(100)
//     // ->Repetitions(10)
//     ->ReportAggregatesOnly();
