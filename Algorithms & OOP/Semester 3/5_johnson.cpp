#include "graph.h"
#include "heap.h"

using namespace std;

class VertexComparator {
public:
        int operator ()(pair<string, double> a, pair<string, double> b) {
                return a.second > b.second;
        }
};

void input(TGraph* G) {
        string s, t;
        int w, E;
        cin >> E;
        for (int i = 0; i < E; i++) {
                cin >> s >> t >> w;
                G->AddEdge(s, t, w);
        }
}

map <string, double> Ford_Bellman(TGraph* G, const string& start) {
        map <string, double> answer;
        for (TVertexIterator it = G->ListVertices(); it.IsValid(); it.GetNext())
                answer[it.Get()] = inf;
        answer[start] = 0;
        for (int i = 0; i < G->VertexCount() - 1; i++) {
                for (TVertexIterator it = G->ListVertices(); it.IsValid(); it.GetNext()) {
                        for (TEdgeIterator jt = G->ListNeighbours(it.Get()); jt.IsValid(); jt.GetNext()) {
                                answer[jt.To()] = min(answer[jt.To()], answer[jt.From()] + jt.Weight());
                        }
                }
        }
        return answer;
}

double Init(TGraph* G) {
        for (TVertexIterator it = G->ListVertices(); it.IsValid(); it.GetNext()) {
                G->AddEdge("", it.Get(), 0);
        }
        map <string, double> m = Ford_Bellman(G, "");
        for (TVertexIterator it = G->ListVertices(); it.IsValid(); it.GetNext()) {
                for (TEdgeIterator jt = G->ListNeighbours(it.Get()); jt.IsValid(); jt.GetNext()) {
                        G->SetWeight(jt.From(), jt.To(), jt.Weight() + m[jt.From()] - m[jt.To()]);
                }
        }
}

map <string, double> Dijkstra(TGraph* G, const string& start) {
        map <string, double> answer;
        map <string, bool> visited;
        for (TVertexIterator it = G->ListVertices(); it.IsValid(); it.GetNext()) {
                answer[it.Get()] = inf;
        }
        answer[start] = 0;
        THeap < pair<string, double>, VertexComparator > H;
        H.Push(make_pair(start, 0));
        visited[start] = true;
        while (!H.Empty() && H.Top().second != inf) {
                pair <string, double> cur = H.Pop();
                visited[cur.first] = true;
                for (TEdgeIterator it = G->ListNeighbours(cur.first); it.IsValid(); it.GetNext()) {
                        if (!visited[it.To()] && answer[it.To()] > answer[it.From()] + it.Weight()) {
                                answer[it.To()] = answer[it.From()] + it.Weight();
                                H.Update(make_pair(it.To(), answer[it.To()]), H.Find(make_pair(it.To(), answer[it.To()])));
                        }
                }
        }
        return answer;
}

map <pair<string, string>, double> Johnson(TGraph* G) {
        map <pair<string, string>, double> answer;
        for (TVertexIterator it = G->ListVertices(); it.IsValid(); it.GetNext()) {
                for (TVertexIterator jt = G->ListVertices(); jt.IsValid(); jt.GetNext()) {
                        answer[make_pair(it.Get(), jt.Get())] = inf;
                }
        }
        for (TVertexIterator it = G->ListVertices(); it.IsValid(); it.GetNext()) {
                map <string, double> ans = Dijkstra(G, it.Get());
                for (map <string, double>::iterator jt = ans.begin(); jt != ans.end(); jt++) {
                        answer[make_pair(it.Get(), jt->first)] = jt->second;
                }
        }
        return answer;
}

int main() {
	auto_ptr <TGraph> G (new TMapGraph());
        input(G.get());
        Init(G.get());
        map <pair<string, string>, double> answer = Johnson(G.get());
        int n;
        string s, f;
        cin >> n;
        for (int i = 0; i < n; i++) {
                cin >> s >> f;
                cout << s << " --> " << f << ": " << answer[make_pair(s, f)] << endl;
        }
        return 0;
}
