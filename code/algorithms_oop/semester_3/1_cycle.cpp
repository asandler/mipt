#include <queue>
#include <string>
#include <vector>
#include <set>
#include "graph.h"

using namespace std;

class ICyclerFinder {
public:
    ICyclerFinder() {
    }
    virtual void Init() = 0;
    virtual bool CycleExist(vector<string>&) = 0;
    virtual ~ICyclerFinder() {
    }
};

class SCC_Solution: public ICyclerFinder {
public:
    SCC_Solution(TGraph* g)
        : G(g)
    {
    }
    void Init() {
        int cc = 1;
        set<string> visited;
        list<string> sorted;
        for (TVertexIterator it = G->ListVertices(); it.IsValid(); it.GetNext()) {
            if (visited.find(it.Get()) == visited.end()) {
                DFS(it.Get(), G, cc, false, visited, sorted);
            }
        }
        visited.clear();
        for (list<string>::iterator it = sorted.begin(); it != sorted.end(); it++) {
            if (visited.find(*it) == visited.end()) {
                DFS(*it, &N, cc, true, visited, sorted);
                cc++;
            }
        }
    }
    bool CycleExist(vector<string>& A) {
        for (std::vector<string>::iterator it = A.begin(); it != A.end(); it++)
            if (Conn_Comp[*it] != Conn_Comp[A[0]])
                return false;
        return true;
    }

private:
    void DFS(const string& s, TGraph* GG, int cc, bool answer, set<string>& visited, list<string>& sorted) {
        visited.insert(s);
        for (TEdgeIterator it = GG->ListNeighbours(s); it.IsValid(); it.GetNext()) {
            if (!answer) {
                N.AddEdge(it.To(), it.From(), it.Weight());
            }
            if (visited.find(it.To()) == visited.end()) {
                DFS(it.To(), GG, cc, answer, visited, sorted);
            }
        }
        if (answer)
            Conn_Comp[s] = cc;
        else
            sorted.push_front(s);
    }

    TGraph* G;
    TListGraph N;
    map<string, int> Conn_Comp;
};

void input(TGraph* G, vector<string>& R) {
    string s, t;
    int w, E, n;
    cin >> E;
    for (int i = 0; i < E; i++) {
        cin >> s >> t >> w;
        G->AddEdge(s, t, w);
    }
    cin >> n;
    for (int i = 0; i < n; i++) {
        cin >> s;
        R.push_back(s);
    }
}

int main() {
    auto_ptr<TGraph> G(new TListGraph());
    vector<string> Request;
    input(G.get(), Request);
    ICyclerFinder* Solve = new SCC_Solution(G.get());
    Solve->Init();
    if (Solve->CycleExist(Request))
        cout << "Cycle exists" << endl;
    else
        cout << "Cycle doesn't exist" << endl;
    return 0;
}
