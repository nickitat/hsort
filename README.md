[![Build Status](https://travis-ci.com/nickitat/hsort.svg?branch=master)](https://travis-ci.com/nickitat/hsort)

## The problem

It is well known that all broadly used comparison-based sorting algorithms performs O(NlogN) comparisons and, consequently, O(NlogN) swaps in order to sort an array of N elements.
Also known that there are types which are expensive to swap (because copying them is roughly the same amount of operations as moving them).
Consider some good old POD type we are all using as protocols structures:

```cpp
struct Message {
  uint  id;
  byte  data[N];
};
```

or without an array, if you wish:

```cpp
struct Employee {
  uint id;
  uint department;
  uint building;
  uint floor;
  uint place;
  uint grade;
  uint salary;
};
```

So, seems it would be nice to have an option to make the number of swaps as little as possible.
Actually, there is the algorithm performing O(N) swaps - Selection Sort :)
The following reasonably simple idea can help us with this issue.
Let we have an array `T a[N]` of elements comparable using some predicate `cmp`.
Then instead of just `sort(a, cmp)` we can do the following:
```cpp
uint indices[N] = { 0, 1, 2, ... , N-1 };
cmp_ind = [&](int i, int j) { return cmp(a[i], a[j]); };
sort(indices, cmp_ind);
apply(indices, a);
```
we nevertheless doing O(NlogN) swaps during our `sort`, but now it swaps intigers.
And since `apply` can be done in linear time, only linear number of swaps of values of our expensive-to-move-type are needed.
You may find this idea implemented in `boost::indirect_*_sort` (except `boost`'s implementation uses pointers).
From my perspective, there is a problem with this approach: dynamic memory allocation for `N` integers on every invocation of `sort`.

## The approach

It is proposed to instantiate class template `hsort::hsort_base` with your class as a template argument.
Effectively it inherits from your type and extends it with the `std::size_t index;` member.
This way, we allocate only once during the object creation and you will be paying this 8-byte cost for the entire lifetime of your data.
But in exchange you get slightly faster sorting.

## Sample benchmarks

Input: 100 randomly shuffled instances of type `hsort::hsort_base<Y>`, where
```cpp
struct Y {
  int key;
  int data[16];
};
```
with keys from [0; 100)

**Benchmark**|**Time**|**CPU**|**Iterations**
:-----:|:-----:|:-----:|:-----:
BM\_SortRandomInput/std\_sort\_tag\_mean|5323 ns|5320 ns|100
**BM\_SortRandomInput/std\_sort\_tag\_median**|5122 ns|5117 ns|100
BM\_SortRandomInput/std\_sort\_tag\_stddev|359 ns|359 ns|100
BM\_SortRandomInput/sort\_heavy\_tag\_mean|4621 ns|4619 ns|100
**BM\_SortRandomInput/sort\_heavy\_tag\_median**|4441 ns|4440 ns|100
BM\_SortRandomInput/sort\_heavy\_tag\_stddev|280 ns|279 ns|100
BM\_SortRandomInput/boost\_pdq\_tag\_mean|4942 ns|4940 ns|100
**BM\_SortRandomInput/boost\_pdq\_tag\_median**|4949 ns|4948 ns|100
BM\_SortRandomInput/boost\_pdq\_tag\_stddev|91.4 ns|91.8 ns|100
BM\_SortRandomInput/boost\_spin\_tag\_mean|5797 ns|5793 ns|100
**BM\_SortRandomInput/boost\_spin\_tag\_median**|5788 ns|5787 ns|100
BM\_SortRandomInput/boost\_spin\_tag\_stddev|168 ns|165 ns|100
BM\_SortRandomInput/boost\_iss\_tag\_mean|5322 ns|5319 ns|100
**BM\_SortRandomInput/boost\_iss\_tag\_median**|5294 ns|5291 ns|100
BM\_SortRandomInput/boost\_iss\_tag\_stddev|97.1 ns|95.2 ns|100
BM\_SortRandomInput/boost\_flat\_tag\_mean|5629 ns|5625 ns|100
**BM\_SortRandomInput/boost\_flat\_tag\_median**|5688 ns|5687 ns|100
BM\_SortRandomInput/boost\_flat\_tag\_stddev|270 ns|271 ns|100
BM\_SortRandomInput/boost\_ifs\_tag\_mean|7684 ns|7679 ns|100
**BM\_SortRandomInput/boost\_ifs\_tag\_median**|7802 ns|7797 ns|100
BM\_SortRandomInput/boost\_ifs\_tag\_stddev|231 ns|229 ns|100
