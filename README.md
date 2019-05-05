# hsort

It is well known that all broadly used comparison-based sorting algorithms performs O(NlogN) comparisons and, consequently, O(NlogN) swaps in order to sort an array of N elements. Also known that there exist types which are expensive to swap (because copying them is roughly the same amount of operations as moving them). Consider some good old POD types we are all using as protocols structures:

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

So, seems it would be nice to have an option to make the number of swaps as little as possible. Actually, there is an algorithm performing O(N) swaps - Selection Sort :)
The following reasonably simple idea can help us with this problem. Let we have an array `T a[N]` of elements comparable using some predicate `cmp`. Then instead of just `sort(a, cmp)` we can do the following:
```cpp
uint indices[N] = { 0, 1, 2, ... , N-1 };
cmpi = [&](int i, int j) { return cmp(a[i], a[j]); };
sort(indices, cmpi);
apply(a, indices);
```
we nevertheless doing O(NlogN) swaps inside our `sort` function, but now it swaps intigers. And since `apply` can be done in linear time, only linear number of swaps of values of our expensive-to-move-type is needed.
You may find this idea implemented in `boost::indirect_sort`.
From my perspective there is a problem with this approach: dynamic memory allocation for `N` integers on every invocation of `sort`.
