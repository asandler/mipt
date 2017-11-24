#include "graph.h"
#include <queue>

using namespace std;

int main() {
    auto_ptr<TGraph> G(new TListGraph());
    queue<string> Q;
    map<string, bool> Visited;
    G->AddEdge("a", "b", 1);
    G->AddEdge("a", "d", 3);
    G->AddEdge("b", "c", 2);
    G->AddEdge("c", "d", 4);
    Q.push("a");
    Visited[Q.front()] = true;
    while (!Q.empty()) {
        cout << Q.front() << endl;
        for (TEdgeIterator it = G->ListNeighbours(Q.front()); it.IsValid(); it.GetNext())
            if (!Visited[it.To()]) {
                Q.push(it.To());
                Visited[it.To()] = true;
            }
        Q.pop();
    }
    for (TEdgeIterator it = G->ListNeighbours("a"); it.IsValid(); it.GetNext())
        cout << it.To() << "^_^" << it.Weight() << endl;
    return 0;
}
