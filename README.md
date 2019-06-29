[![Build Status](https://travis-ci.com/nickitat/hsort.svg?branch=master)](https://travis-ci.com/nickitat/hsort)

## The problem

It is known that the all broadly used comparison-based sorting algorithms perform O(NlogN) comparisons and, consequently, O(NlogN) swaps in order to sort an array of N elements.
Also known that there are types which are expensive to swap (because copying them is roughly the same amount of operations as moving them).
Consider some good old POD-type we are all using as protocols structure:

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
we still doing O(NlogN) swaps during `sort`, but now we are swaping intigers.
And since `apply` can be done in linear time, only linear number of swaps of values of our expensive-to-move-type are needed.
You may find this idea implemented in `boost::indirect_*_sort` (except that `boost`'s implementation uses pointers).
But this approach has a drawback: dynamic memory allocation for `N` integers on every invocation of `sort`.

## The approach

It is proposed to instantiate class template `hsort::hsort_base` with your class as a template argument.
Effectively it inherits from your type and extends it with the `std::size_t index;` member.
This way, we allocate only once during the object creation and you will be paying this 8-byte cost for the whole lifetime of your data.
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


Run on (1 X 2000.16 MHz CPU )<br/>
CPU Caches:<br/>
  L1 Data 32K (x1)<br/>
  L1 Instruction 32K (x1)<br/>
  L2 Unified 256K (x1)<br/>
  L3 Unified 56320K (x1)<br/>
Load Average: 0.81, 0.52, 0.23<br/>


**Benchmark**|**Time**|**CPU**|**Iterations**
:-----:|:-----:|:-----:|:-----:
BM\_SortRandomInput/std\_sort\_tag\_mean|5275 ns|5273 ns|10
**BM\_SortRandomInput/std\_sort\_tag\_median**|5302 ns|5298 ns|10
BM\_SortRandomInput/std\_sort\_tag\_stddev|117 ns|116 ns|10
BM\_SortRandomInput/sort\_heavy\_tag\_mean|4584 ns|4583 ns|10
**BM\_SortRandomInput/sort\_heavy\_tag\_median**|4650 ns|4648 ns|10
BM\_SortRandomInput/sort\_heavy\_tag\_stddev|277 ns|277 ns|10
BM\_SortRandomInput/boost\_pdq\_tag\_mean|4993 ns|4991 ns|10
**BM\_SortRandomInput/boost\_pdq\_tag\_median**|4969 ns|4967 ns|10
BM\_SortRandomInput/boost\_pdq\_tag\_stddev|223 ns|223 ns|10
BM\_SortRandomInput/boost\_spin\_tag\_mean|5725 ns|5723 ns|10
**BM\_SortRandomInput/boost\_spin\_tag\_median**|5506 ns|5504 ns|10
BM\_SortRandomInput/boost\_spin\_tag\_stddev|343 ns|343 ns|10
BM\_SortRandomInput/boost\_iss\_tag\_mean|5541 ns|5540 ns|10
**BM\_SortRandomInput/boost\_iss\_tag\_median**|5466 ns|5464 ns|10
BM\_SortRandomInput/boost\_iss\_tag\_stddev|220 ns|220 ns|10
BM\_SortRandomInput/boost\_flat\_tag\_mean|5606 ns|5596 ns|10
**BM\_SortRandomInput/boost\_flat\_tag\_median**|5583 ns|5575 ns|10
BM\_SortRandomInput/boost\_flat\_tag\_stddev|167 ns|171 ns|10
BM\_SortRandomInput/boost\_ifs\_tag\_mean|7617 ns|7614 ns|10
**BM\_SortRandomInput/boost\_ifs\_tag\_median**|7600 ns|7597 ns|10
BM\_SortRandomInput/boost\_ifs\_tag\_stddev|259 ns|258 ns|10
