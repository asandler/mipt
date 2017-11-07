#include <cstdio>
#include <queue>

using namespace std;

int parents[400000][20];
int depth[400000];
int n, m;

void create_tree()
{
    scanf("%d",&n);
    for (int j=0; j<n; ++j)
    {
        int x, y;
        scanf("%d%d",&x,&y);
        parents[x][0] = y;
		depth[x] = depth[y] + 1;
		for (int i=1; i<20; ++i)
			parents[x][i] = parents[ parents[x][i-1] ][i-1];
    }
}

int LCA(int x, int y)
{
    while ( depth[x] != depth[y] )
        if ( depth[x] > depth[y] )
        {
            int i = 0;
            while ( i < 20 && depth[parents[x][i]] >= depth[y] ) i++;
            x = parents[x][i-1];
        }
        else
        {
            int i = 0;
            while ( i < 20 && depth[parents[y][i]] >= depth[x]) i++;
            y = parents[y][i-1];
        }
    while ( x != y )
    {
        int i = 0;
        while ( i < 20 && parents[x][i] != parents[y][i] ) i++;
        if (i>0) i--;
        x = parents[x][i];
        y = parents[y][i];
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