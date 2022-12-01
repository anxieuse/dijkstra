#include <iostream>
#include <chrono>

#include "graph.hpp"
#include "dijk.hpp"

#include <boost/heap/fibonacci_heap.hpp>
#include <boost/heap/d_ary_heap.hpp>
#include <boost/heap/binomial_heap.hpp>
#include <boost/heap/pairing_heap.hpp>
#include <boost/heap/skew_heap.hpp>
#include <boost/heap/priority_queue.hpp>

using namespace std;
using namespace std::chrono;
using namespace boost::heap;

int main() {
  int n, m, q;
  cin >> n >> m >> q;

  Graph g(n, m);

  cout << "Reading edges" << endl;
  for (int i = 0; i < m; i++) {
    int u, v, w;
    cin >> u >> v >> w;
    --u, --v;
    g.addEdge(u, v, w);
  }

  unordered_map<string, unordered_map<string, double>> heapMetrics = {
    {"stl_priority_queue", {
      {"push", 0},
      {"pop", 0},
      {"time", 0},
    }},
    {"4_ary_heap", {
      {"push", 0},
      {"pop", 0},
      {"time", 0},
    }},
  };

  int exps = 10;
  cout << "Running global Dijkstra " << exps << " times" << endl;
  while(exps--)
  for (int i = 0; i < n; i++) {
    // pq
    {
      auto start = high_resolution_clock::now();
      std::priority_queue<pair<int, int>> pq;
      Dijkstra(g, i, pq);
      auto stop = high_resolution_clock::now();
      auto duration = duration_cast<microseconds>(stop - start);
      heapMetrics["stl_priority_queue"]["time"] += duration.count();
      heapMetrics["stl_priority_queue"]["push"] += pushCount;
      heapMetrics["stl_priority_queue"]["pop"] += popCount;
    }

    // 4_ary_heap
    {
      auto start = high_resolution_clock::now();
      d_ary_heap<pair<int, int>, boost::heap::arity<4>> pq;
      Dijkstra(g, i, pq);
      auto stop = high_resolution_clock::now();
      auto duration = duration_cast<microseconds>(stop - start);
      heapMetrics["4_ary_heap"]["time"] += duration.count();
      heapMetrics["4_ary_heap"]["push"] += pushCount;
      heapMetrics["4_ary_heap"]["pop"] += popCount;
    }
  }

  cout << endl;

  for (auto &heap : heapMetrics) {
    cout << heap.first << endl;
    for (auto &metric : heap.second) {
      cout << metric.first << ": ";
      if (metric.first == "time")
        cout << metric.second / 1e6 << "s" << endl;
      else
        cout << (long long)(metric.second) << " operations" << endl;
    }
    cout << endl;
  }

  return 0;
}
