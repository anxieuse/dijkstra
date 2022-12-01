#include <bits/stdc++.h>
// d-ary heap
#include <boost/heap/fibonacci_heap.hpp>
#include <boost/heap/d_ary_heap.hpp>
#include <boost/heap/binomial_heap.hpp>
#include <boost/heap/pairing_heap.hpp>
#include <boost/heap/skew_heap.hpp>
#include "nheap.hpp"

using namespace std;
using namespace boost::heap;

int main() {
  d_ary_heap<int, boost::heap::arity<4>> pq;
  pq.push(1);
  pq.erase(pq.begin());
}