#pragma once

#include "graph.hpp"

using namespace std;

int pushCount = 0;
int popCount = 0;

template <typename T>
void Dijkstra(Graph &g, int s, T &pq) {
  pushCount = popCount = 0;

  g.dist.assign(g.n, INT_MAX);
  g.prev.assign(g.n, -1);
  g.visited.assign(g.n, false);
  
  g.dist[s] = 0;
  pq.push({0, s});

  while (!pq.empty()) {
    auto [d, u] = pq.top();
    pq.pop();
    popCount++;

    if (g.visited[u]) continue;
    g.visited[u] = true;

    for (auto [v, w] : g.adj[u]) {
      if (g.dist[v] > g.dist[u] + w) {
        g.dist[v] = g.dist[u] + w;
        g.prev[v] = u;
        pq.push({g.dist[v], v});
        pushCount++;
      }
    }
  }
}