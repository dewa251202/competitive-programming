// Created by Dewangga Putra S

#include <iostream>
#include <cstring>
using namespace std;

const int MAX_N = 100;
const int INF = 1E9;

// number of left and right vertices respectively
int N, M;

// edge informations: weight and its existence in the equality graph
int weight[MAX_N][MAX_N];
bool adj_matrix[MAX_N][MAX_N];

// labels for left and right vertices respectively
int label_U[MAX_N];
int label_V[MAX_N];

// visited vertices, used for finding alternating paths
bool vis_U[MAX_N];
bool vis_V[MAX_N];

// owned for left vertices, owner for right vertices
int owned[MAX_N];
int owner[MAX_N];

// set all labels such that weight[u][v] <= label_U[u] + label_V[v] for each u in left, v in right
void initialize_labels(){
    // set label_U[i] = maximum value of edge weight connected to vertex i
    for(int i = 0; i < N; i++){
        label_U[i] = weight[i][0];
        for(int j = 1; j < N; j++){
            label_U[i] = max(label_U[i], weight[i][j]);
        }
    }

    // set label_V[v] = 0 for each v in right
    memset(label_V, 0, sizeof(label_V));
}

// for each edge (u, v), check if label_U[u] + label_V[v] == weight[u][v] or not
// if yes, add it to the equality graph
void construct_equality_graph(){
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            adj_matrix[i][j] = (label_U[i] + label_V[j] == weight[i][j]);
        }
    }
}

// check if there is augmenting path or not, else we can get some alternating paths instead
// reference : https://scele.cs.ui.ac.id/pluginfile.php/63336/mod_resource/content/1/source-code/alternating_dfs.cpp
int augment(int u){
    if(vis_U[u]){
        return 0;
    }

    vis_U[u] = 1;
    for(int v = 0; v < N; v++){
        if(adj_matrix[u][v]){
            if(owner[v] == -1 || augment(owner[v])){
                owner[v] = u;
                owned[u] = v;
                return 1;
            }
            else{
                vis_V[v] = 1;
            }
        }
    }
    return 0;
}

// check if we can get augmenting path
bool can_augment(int root){
    memset(vis_U, 0, sizeof(vis_U));
    memset(vis_V, 0, sizeof(vis_V));

    return augment(root);
}

// Let S = visited vertices in left, T = visited vertices in right
// for each x in S and y in V\T(unvisited vertices in right),
// delta = minimum of: label_U[x] + label_V[y] - weight[x][y]
// it's guaranteed that delta exist
int compute_delta(){
    int delta = INF;

    for(int i = 0; i < N; i++){
        if(vis_U[i]){
            for(int j = 0; j < N; j++){
                if(!vis_V[j]){
                    delta = min(delta, label_U[i] + label_V[j] - weight[i][j]);
                }
            }
        }
    }

    return delta;
}

// for each x in set of vertices(all) update the labels
// if x in S, label_U[x] := label_U[x] - delta
// else if x in T, label_V[x] := label_V[x] + delta
// else don't update anything
void update_labels(int delta){
    for(int i = 0; i < N; i++){
        if(vis_U[i]){
            label_U[i] -= delta;
        }

        if(vis_V[i]){
            label_V[i] += delta;
        }
    }
}

// main algorithm
int hungarian_algorithm(){
    initialize_labels();
    construct_equality_graph();

    memset(owned, -1, sizeof(owned));
    memset(owner, -1, sizeof(owner));

    // set the matching set cardinality(MCBM) to 0
    // we're going to use root as iterator to find unmatched vertices in left
    int MCBM = 0;
    int root = 0;
    while(MCBM < N){
        while(owned[root] != -1){
            root++;
        }

        // update the equality graph until we can find an augmenting path
        while(!can_augment(root)){
            int delta = compute_delta();
            update_labels(delta);

            construct_equality_graph();
        }

        // increase the matching set cardinality
        MCBM++;
    }

    // because we inverted the weight, invert it back
    int ans = 0;
    for(int i = 0; i < N; i++){
        ans += -weight[owner[i]][i];
    }

    return ans;
}

// main program
int main(){
    // input
    cin >> N >> M;
    for(int i = 0; i < N; i++){
        for(int j = 0; j < M; j++){
            cin >> weight[i][j];
            // the hungarian algorithm written above actually finds maximum weight bipartite matching
            // we can invert the weight of each edge to trick it
            weight[i][j] = -weight[i][j];
        }
    }
    // in case of unbalanced matching, add some dummy vertices and edges with weight 0
    N = max(N, M);

    // run the algorithm
    cout << hungarian_algorithm() << '\n';

    return 0;
}
