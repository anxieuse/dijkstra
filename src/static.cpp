#include <bits/stdc++.h>

using namespace std;

/*
Very fast implementation of DIjkstra's algorithm
*/

const int MAXN = 100000;
const int MAXM = 100000;
const int INF = 1000000000;

int id, n, m, s, t, dems;
int a[MAXM], b[MAXM], w[MAXM];
vector<pair<int, int>> g[MAXN];

int d[MAXN];
bool used[MAXN];

void dijkstra(int s) {
  for (int i = 0; i < n; i++) {
    d[i] = INF;
    used[i] = false;
  }
  d[s] = 0;
  for (int i = 0; i < n; i++) {
    int v = -1;
    for (int j = 0; j < n; j++) {
      if (!used[j] && (v == -1 || d[j] < d[v])) {
        v = j;
      }
    }
    if (d[v] == INF) {
      break;
    }
    used[v] = true;
    for (auto edge : g[v]) {
      int to = edge.first;
      int len = edge.second;
      if (d[v] + len < d[to]) {
        d[to] = d[v] + len;
      }
    }
  }
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL), cout.tie(NULL);
  
  cin >> n >> m >> dems;
  for (int i = 0; i < m; i++) {
    cin >> a[i] >> b[i] >> w[i];
    g[a[i]].push_back({b[i], w[i]});
    g[b[i]].push_back({a[i], w[i]});
  }
  cout << "Running Dijkstra" << endl;
  auto start = chrono::high_resolution_clock::now();
  for (int i = 0; i < dems; i++) {
    cin >> id >> s >> t;
    --s, --t;
    dijkstra(s);
  }
  auto end = chrono::high_resolution_clock::now();
  auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
  cout << "Time taken to run Dijkstra: " << duration.count() << " microseconds" << endl;
  return 0;
}