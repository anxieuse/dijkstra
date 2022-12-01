#include <bits/stdc++.h>
// d-ary heap
#include <boost/heap/fibonacci_heap.hpp>
#include <boost/heap/d_ary_heap.hpp>
#include <boost/heap/binomial_heap.hpp>
#include <boost/heap/pairing_heap.hpp>
#include <boost/heap/skew_heap.hpp>

using namespace std;
using namespace boost::heap;

int main() {
  // pairing_heap<pair<int, int>, compare<pair<int, int>>> pq;
  // skew_heap<pair<int, int>, compare<pair<int, int>>> pq;
  // binomial_heap<pair<int, int>, compare<pair<int, int>>> pq;
  // d_ary_heap<pair<int, int>, compare<pair<int, int>>> pq;
  // fibonacci_heap<pair<int, int>, compare<pair<int, int>>> pq;
  // priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;

  // create d_ary_heap with 4-ary heap
  d_ary_heap<pair<int, int>, boost::heap::arity<4>> pq;
}