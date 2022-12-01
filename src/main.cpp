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

int pops = 0, pushes = 0;

struct Graph {
  int n, m;
  vector<vector<pair<int, int>>> adj;
  vector<int> dist;
  vector<int> parent;
  vector<bool> visited;

  Graph(int n, int m) {
    this->n = n;
    this->m = m;
    adj.resize(n);
    dist.resize(n);
    parent.resize(n);
    visited.resize(n);
  }

  void addEdge(int u, int v, int w) {
    adj[u].push_back({v, w});
    adj[v].push_back({u, w});
  }

  void dijkstra_fheap(int s, fHeap<int, int> &fheap) {
    dist.assign(n, INT_MAX);
    parent.assign(n, -1);
    visited.assign(n, false);

    dist[s] = 0;
    fHeap<int, int> pq;
    pq.enqueue(s, 0);
    while (!pq.isEmpty()) {
      int u = pq.dequeue();
      ++pops;
      if (visited[u]) {
        continue;
      }
      visited[u] = true;
      for (auto v : adj[u]) {
        if (dist[v.first] > dist[u] + v.second) {
          dist[v.first] = dist[u] + v.second;
          parent[v.first] = u;
          pq.enqueue(v.first, dist[v.first]);
          ++pushes;
        }
      }
    }
  }

  template<class Q>
  void dijkstra(int s, Q pq) {
    dist.assign(n, INT_MAX);
    parent.assign(n, -1);
    visited.assign(n, false);

    dist[s] = 0;
    pq.push({0, s});
    while (!pq.empty()) {
      int u = pq.top().second;
      int dist_u = pq.top().first;
      pq.pop();
      ++pops;
      if (visited[u]) {
        continue;
      }
      visited[u] = true;
      for (auto v : adj[u]) {
        if (dist[v.first] > dist[u] + v.second) {
          dist[v.first] = dist[u] + v.second;
          parent[v.first] = u;
          pq.push({dist[v.first], v.first});
          ++pushes;
        }
      }
    }
  }

  void printPath(int s, int v) {
    if (v == s) {
      cout << s << " ";
      return;
    }
    if (parent[v] == -1) {
      cout << "No path from " << s << " to " << v << endl;
      return;
    }
    printPath(s, parent[v]);
    cout << v << " ";
  }
};

int main(int argc, char *argv[]) {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL), cout.tie(NULL);

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

  unordered_map<string, unordered_map<string, double>> runtimes = {
    {"priority_queue", {
      {"push", 0},
      {"pop", 0},
      {"total", 0},
    }},
    {"boost_priority_queue", {
      {"push", 0},
      {"pop", 0},
      {"total", 0},
    }},
    {"fheap", {
      {"push", 0},
      {"pop", 0},
      {"total", 0},
    }},
    {"2_ary_heap", {
      {"push", 0},
      {"pop", 0},
      {"total", 0},
    }},
    {"3_ary_heap", {
      {"push", 0},
      {"pop", 0},
      {"total", 0},
    }},
    {"4_ary_heap", {
      {"push", 0},
      {"pop", 0},
      {"total", 0},
    }},
  };

  int exps = 3;
  cout << "Running global Dijkstra " << exps << " times" << endl;
  while(exps--)
  for (int i = 0; i < n; i++) {
    // // pq
    // {
    //   auto start = chrono::high_resolution_clock::now();
    //   priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    //   g.dijkstra(i, pq);
    //   auto end = chrono::high_resolution_clock::now();
    //   auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
    //   runtimes["priority_queue"]["total"] += duration.count();
    //   runtimes["priority_queue"]["push"] += pushes;
    //   runtimes["priority_queue"]["pop"] += pops;
    //   pushes = pops = 0;
    // }
    // fheap
    {
      auto start = chrono::high_resolution_clock::now();
      fHeap<int, int> fheap;
      g.dijkstra_fheap(i, fheap);
      auto end = chrono::high_resolution_clock::now();
      auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
      runtimes["fheap"]["total"] += duration.count();
      runtimes["fheap"]["push"] += pushes;
      runtimes["fheap"]["pop"] += pops;
      pushes = pops = 0;
    }
    // 4_ary_heap
    {
      auto start = chrono::high_resolution_clock::now();
      d_ary_heap<pair<int, int>, boost::heap::arity<4>, boost::heap::compare<std::greater<pair<int, int>>>> pq;
      g.dijkstra(i, pq);
      auto end = chrono::high_resolution_clock::now();
      auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
      runtimes["4_ary_heap"]["total"] += duration.count();
      runtimes["4_ary_heap"]["push"] += pushes;
      runtimes["4_ary_heap"]["pop"] += pops;
      pushes = pops = 0;
    }
    // 3_ary_heap
    {
      auto start = chrono::high_resolution_clock::now();
      d_ary_heap<pair<int, int>, boost::heap::arity<3>, boost::heap::compare<std::greater<pair<int, int>>>> pq;
      g.dijkstra(i, pq);
      auto end = chrono::high_resolution_clock::now();
      auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
      runtimes["3_ary_heap"]["total"] += duration.count();
      runtimes["3_ary_heap"]["push"] += pushes;
      runtimes["3_ary_heap"]["pop"] += pops;
      pushes = pops = 0;
    }
  }

  for (auto &heap : runtimes) {
    cout << heap.first << endl;
    for (auto &metric : heap.second) {
      cout << metric.first << ": ";
      if (metric.first == "total")
        cout << metric.second / 1e6 << "s" << endl;
      else
        cout << (long long)(metric.second) << " operations" << endl;
    }
  }

  return 0;
}
