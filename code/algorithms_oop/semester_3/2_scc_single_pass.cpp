#include <queue>
#include <string>
#include <vector>
#include <set>
#include "graph.h"

using namespace std;

class IConnectFinder {
public:
    IConnectFinder() {
    }
    virtual void Init(TGraph*) = 0;
    virtual bool ConnectFinder(const vector<string>&) = 0;
    virtual ~IConnectFinder() {
    }
};

class TEdgeConnect: public IConnectFinder {
public:
    virtual void Init(TGraph* G) {
        int time = 1;
        map<string, pair<int, int>> visited;

        max_color = 0;
        stack.clear();
        for (TVertexIterator it = G->ListVertices(); it.IsValid(); it.GetNext()) {
            if (visited[it.Get()].first == 0 && visited[it.Get()].second == 0) {
                DFS(it.Get(), NULL, G, time, visited);
            }
        }

        for (map<string, int>::iterator it = colors.begin(); it != colors.end(); it++)
            cout << it->first << ' ' << it->second << endl;
    }
    virtual bool ConnectFinder(const vector<string>& A) {
        if (A.size() <= 1)
            return true;
        for (vector<string>::const_iterator it = A.begin(); it != A.end(); it++)
            if (colors[*it] != colors[A[0]])
                return false;
        return true;
    }

private:
    void DFS(const string& s, const string* parent, TGraph* G, int& time, map<string, pair<int, int>>& visited) {
        time++;
        visited[s] = make_pair(time, time);
        stack.push_back(s);
        for (TEdgeIterator it = G->ListNeighbours(s); it.IsValid(); it.GetNext()) {
            if (parent != NULL && it.To() == *parent)
                continue;
            if (visited.find(it.To()) == visited.end()) {
                DFS(it.To(), &s, G, time, visited);
                visited[s].second = min(visited[s].second, visited[it.To()].second);
            } else {
                visited[s].second = min(visited[s].second, visited[it.To()].first);
            }
        }
        if (visited[s].first == visited[s].second) {
            max_color++;
            while (!stack.empty() && stack.back() != s) {
                colors[stack.back()] = max_color;
                stack.pop_back();
            }
            colors[s] = max_color;
            if (!stack.empty())
                stack.pop_back();
        }
    }

    map<string, int> colors;
    vector<string> stack;
    int max_color;
};

class TVertexConnect: public IConnectFinder {
public:
    virtual void Init(TGraph* G) {
        int time = 0;
        map<string, pair<int, int>> visited;

        max_color = 0;
        for (TVertexIterator it = G->ListVertices(); it.IsValid(); it.GetNext()) {
            if (visited[it.Get()].first == 0 && visited[it.Get()].second == 0) {
                DFS(it.Get(), NULL, G, time, visited);
            }
        }

        for (map<pair<string, string>, int>::iterator it = colors.begin(); it != colors.end(); it++)
            cout << it->first.first << ' ' << it->first.second << ' ' << it->second << endl;
    }
    virtual bool ConnectFinder(const vector<string>& A) {
        if (A.size() <= 1)
            return true;
        for (vector<string>::const_iterator it = A.begin(); it != A.end(); it++)
            for (vector<string>::const_iterator jt = A.begin(); jt != A.end(); jt++)
                if (*it != *jt && colors[make_pair(*it, *jt)] != colors[make_pair(A[0], A[1])])
                    return false;
        return true;
    }

private:
    void DFS(const string& s, const string* parent, TGraph* G, int& time, map<string, pair<int, int>>& visited) {
        time++;
        visited[s] = make_pair(time, time);
        for (TEdgeIterator it = G->ListNeighbours(s); it.IsValid(); it.GetNext()) {
            if (parent != NULL && it.To() == *parent)
                continue;
            if (visited.find(it.To()) == visited.end()) {
                stack.push_back(make_pair(s, it.To()));
                DFS(it.To(), &s, G, time, visited);
                if (visited[it.To()].second >= visited[s].first) {
                    max_color++;
                    while (stack.back() != make_pair(s, it.To())) {
                        colors[stack.back()] = max_color;
                        colors[make_pair(stack.back().second, stack.back().first)] = max_color;
                        stack.pop_back();
                    }
                    colors[make_pair(s, it.To())] = max_color;
                    colors[make_pair(it.To(), s)] = max_color;
                    stack.pop_back();
                }
                if (visited[it.To()].second < visited[s].second)
                    visited[s].second = visited[it.To()].second;
            } else {
                if (visited[it.To()].first < visited[s].first)
                    stack.push_back(make_pair(s, it.To()));
                if (visited[s].second > visited[it.To()].first)
                    visited[s].second = visited[it.To()].second;
            }
        }
    }

    map<pair<string, string>, int> colors;
    vector<pair<string, string>> stack;
    int max_color;
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
    //IConnectFinder* Solve = new TVertexConnect;
    IConnectFinder* Solve = new TEdgeConnect;
    Solve->Init(G.get());
    if (Solve->ConnectFinder(Request))
        cout << "Connected" << endl;
    else
        cout << "Not connected" << endl;
    return 0;
}
