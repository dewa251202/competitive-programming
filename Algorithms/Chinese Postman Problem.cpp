#include <iostream>
#include <cstring>
#include <vector>
using namespace std;

const int MAX_V = 15;
const int INF = 1E9;

int V, E;
vector <int> odd_vertices;
int dist[MAX_V][MAX_V];
int deg[MAX_V];
int dp[1 << MAX_V];

void floyd_warshall(){
    for(int k = 0; k < V; k++){
        for(int i = 0; i < V; i++){
            for(int j = 0; j < V; j++){
                dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
            }
        }
    }
}

int min_cost_matching_dp(int mask){
    if(mask == 0) return 0;
    if(dp[mask] != -1) return dp[mask];
    dp[mask] = INF;
    for(int i : odd_vertices){
        for(int j : odd_vertices){
            if(i != j && (mask & (1 << i)) && (mask & (1 << j))){
                int temp = min_cost_matching_dp(mask ^ (1 << i) ^ (1 << j));
                dp[mask] = min(dp[mask], temp + dist[i][j]);
            }
        }
    }
    return dp[mask];
}

int main(){
    cin >> V >> E;

    memset(dp, -1, sizeof(dp));
    for(int i = 0; i < V; i++)
        for(int j = 0; j < V; j++)
            dist[i][j] = (i != j) * INF;

	int u, v, w;
	int sum_cost = 0;
    for(int i = 0; i < E; i++){
        cin >> u >> v >> w;
        deg[--u]++; deg[--v]++;
        sum_cost += w;
        dist[u][v] = dist[v][u] = min(dist[u][v], w);
    }

    int mask = 0;
    for(int i = 0; i < V; i++){
        if(deg[i] & 1){
            odd_vertices.push_back(i);
            mask |= (1 << i);
        }
    }

    floyd_warshall();

    cout << sum_cost + min_cost_matching_dp(mask) << '\n';
    return 0;
}
