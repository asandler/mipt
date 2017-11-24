#include <queue>
#include <string>
#include <vector>
#include <set>
#include "graph.h"

using namespace std;

class IConnectFinder {
public:
        IConnectFinder() {}
        virtual void Init(TGraph *) = 0;
        virtual bool ConnectFinder(const vector<string> &) = 0;
        virtual ~IConnectFinder() {}
};

class TEdgeConnect : public IConnectFinder {
public:
        virtual void Init(TGraph *G) {
                int time = 0;
                map <string, pair<int, int> > visited;

                for (TVertexIterator it = G->ListVertices(); it.IsValid(); it.GetNext()) {
                        if (visited.find(it.Get()) == visited.end()) {
                                DFS(it.Get(), NULL, G, time, visited);
                        }
                }
                for (map<string, pair<int, int> >::iterator it = visited.begin(); it != visited.end(); it++)
                        cout << it->first << ' ' << it->second.first << ' ' << it->second.second << endl;

                max_color = 0;
                colors.clear();
                for (TVertexIterator it = G->ListVertices(); it.IsValid(); it.GetNext()) {
                        if (colors[it.Get()] == 0) {
                                max_color++;
                                paint(it.Get(), max_color, colors, G, visited);
                        }
                }
        }
        virtual bool ConnectFinder(const vector<string> &A) {
                for (vector<string>::const_iterator it = A.begin(); it != A.end(); it++)
                        if (colors[*it] != colors[A[0]])
                                return false;
                return true;
        }
private:
        void DFS(const string &s, const string *parent, TGraph* G, int &time, map<string, pair<int, int> > &visited) {
                time++;
                visited[s] = make_pair(time, time);
                for (TEdgeIterator it = G->ListNeighbours(s); it.IsValid(); it.GetNext()) {
                        if (visited.find(it.To()) == visited.end()) {
                                DFS(it.To(), &s, G, time, visited);
                                visited[s].second = min(visited[s].second, visited[it.To()].second);
                        } else {
                                if (parent != NULL && it.To() != *parent)
                                        visited[s].second = min(visited[s].second, visited[it.To()].first);
                        }
                }
        }
        void paint(const string &s, int &color, map<string, int> &colors, TGraph *G, map <string, pair<int, int> > &visited) {
                colors[s] = color;
                for (TEdgeIterator it = G->ListNeighbours(s); it.IsValid(); it.GetNext()) {
                        if (colors[it.To()] == 0) {
                                if (visited[it.To()].first == visited[it.To()].second) {
                                        max_color++;
                                        paint(it.To(), max_color, colors, G, visited);
                                } else {
                                        paint(it.To(), color, colors, G, visited);
                                }
                        }
                }
        }

        map<string, int> colors;
        int max_color;
};

class TVertexConnect : public IConnectFinder {
public:
        virtual void Init(TGraph *G) {
                int time = 0;
                map <string, pair<int, int> > visited;

                for (TVertexIterator it = G->ListVertices(); it.IsValid(); it.GetNext()) {
                        if (visited.find(it.Get()) == visited.end()) {
                                DFS(it.Get(), NULL, G, time, visited);
                        }
                }

                max_color = 0;
                set<string> used;
                colors.clear();
                for (TVertexIterator it = G->ListVertices(); it.IsValid(); it.GetNext()) {
                        if (used.find(it.Get()) == used.end()) {
                                max_color++;
                                paint(it.Get(), NULL, max_color, used, G, visited);
                        }
                }

                map<pair<string, string>, int> _colors = colors;
                for (map<pair<string, string>, int>::iterator it = _colors.begin(); it != _colors.end(); it++) {
                        colors[make_pair(it->first.second, it->first.first)] = it->second;
                }
                for (map<pair<string, string>, int>::iterator it = colors.begin(); it != colors.end(); it++) {
                        cout << it->first.first << ' ' << it->first.second << ' ' << it->second << endl;
                }
        }
        virtual bool ConnectFinder(const vector<string> &A) {
                if (A.size() <= 1)
                        return true;
                for (vector<string>::const_iterator it = A.begin(); it != A.end(); it++)
                        for (vector<string>::const_iterator jt = A.begin(); jt != A.end(); jt++)
                                if (*it != *jt && colors[make_pair(*it, *jt)] != colors[make_pair(A[0], A[1])])
                                        return false;
                return true;
        }
private:
        void DFS(const string &s, const string *parent, TGraph* G, int &time, map<string, pair<int, int> > &visited) {
                time++;
                visited[s] = make_pair(time, time);
                for (TEdgeIterator it = G->ListNeighbours(s); it.IsValid(); it.GetNext()) {
                        if (visited.find(it.To()) == visited.end()) {
                                DFS(it.To(), &s, G, time, visited);
                                visited[s].second = min(visited[s].second, visited[it.To()].second);
                        } else {
                                if (parent != NULL && it.To() != *parent)
                                        visited[s].second = min(visited[s].second, visited[it.To()].first);
                        }
                }
        }

        void paint(const string &s, const string *parent, int &color, set<string> &used, TGraph *G, map <string, pair<int, int> > &visited) {
                used.insert(s);
                for (TEdgeIterator it = G->ListNeighbours(s); it.IsValid(); it.GetNext()) {
                        if (parent != NULL && it.To() == *parent)
                                continue;
                        if (used.find(it.To()) == used.end()) {
                                if (visited[it.To()].second >= visited[s].first) {
                                        colors[make_pair(s, it.To())] = ++max_color;
                                        paint(it.To(), &s, max_color, used, G, visited);
                                } else {
                                        colors[make_pair(s, it.To())] = color;
                                        paint(it.To(), &s, color, used, G, visited);
                                }
                        } else {
                                if (visited[it.To()].first <= visited[s].first) {
                                        colors[make_pair(s, it.To())] = color;
                                }
                        }
                }
        }

        map<pair<string, string>, int> colors;
        int max_color;
};

void input(TGraph* G, vector<string> &R) {
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
	auto_ptr <TGraph> G (new TListGraph());
        vector <string> Request;
        input(G.get(), Request);
        IConnectFinder* Solve = new TVertexConnect;
        //IConnectFinder* Solve = new TEdgeConnect;
        Solve->Init(G.get());
        if (Solve->ConnectFinder(Request))
                cout << "Connected" << endl;
        else
                cout << "Not connected" << endl;
        return 0;
}
