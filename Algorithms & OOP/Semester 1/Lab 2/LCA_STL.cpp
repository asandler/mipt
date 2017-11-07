#include <cstdio>
#include <vector>
#include <queue>

using namespace std;

struct item
{
	int depth;
	vector <int> parents, children;
};

item a[400010];
int n, m;

void create_tree()
{
	scanf("%d",&n);
	for (int j=0; j<n; ++j)
	{
        int x, y;
        scanf("%d%d",&x,&y);
        a[x].parents.push_back(y);
        a[y].children.push_back(x);
    }
    queue <int> Q;
    Q.push(0);
    while (!Q.empty())
    {
        int v = Q.front();
        for (vector<int>::const_iterator it = a[v].children.begin(); it!=a[v].children.end(); ++it)
        {
            Q.push(*it);
            a[*it].depth = a[v].depth + 1;
            size_t i = 0;
            while ( i < a[a[*it].parents[i]].parents.size() )
            {
                a[*it].parents.push_back(a[a[*it].parents[i]].parents[i]);
                i++;
            }
        }
        a[v].children.clear();
        Q.pop();
    }
}

int LCA(int x, int y)
{
    while ( a[x].depth != a[y].depth )
        if ( a[x].depth > a[y].depth )
        {
            size_t i = 0;
            while ( i < a[x].parents.size() && a[a[x].parents[i]].depth >= a[y].depth) i++;
            x = a[x].parents[i-1];
        }
        else
        {
            size_t i = 0;
            while ( i < a[y].parents.size() && a[a[y].parents[i]].depth >= a[x].depth) i++;
            y = a[y].parents[i-1];
        }
    while ( x != y )
    {
        size_t i = 0;
        while ( i < a[x].parents.size() && i < a[y].parents.size() && a[x].parents[i] != a[y].parents[i] ) i++;
        if (i>0) i--;
        x = a[x].parents[i];
        y = a[y].parents[i];
    }
    return x;
}

void solve()
{
    scanf("%d",&m);
    for (int i=0; i<m; ++i)
    {
        int x, y;
        scanf("%d%d",&x,&y);
        printf("%d\n",LCA(x,y));
    }
}

int main()
{
    create_tree();
    solve();
    return 0;
}