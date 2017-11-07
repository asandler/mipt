#include <iostream>

using namespace std;

struct cell
{
    int i, j, num;
    cell *next, *pred;
};

int a[1010][1010], hyp[1010][4], n, m, i0, j0;
bool b[1010][1010];

void input()
{
    int h, k;
    cin >> n >> m >> i0 >> j0;
    for (int i=1; i<=n; ++i)
        for (int j=1; j<=m; ++j)
        {
            cin >> a[i][j];
            if (a[i][j] == 1)
                b[i][j] = true;
        }
    cin >> h;
    for (int i=2; i<h+2; ++i)
    {
        for (int j=0; j<4; ++j)
            cin >> hyp[i][j];
        a[hyp[i][0]][hyp[i][1]] = i;
    }
    cin >> k;
    for (int i=0; i<k; ++i)
    {
        int t1, t2;
        cin >> t1 >> t2;
        a[t1][t2] = -2;
    }
}

void output(cell* t)
{
    if (t == NULL) return;
    output(t->pred);
    cout << t->i << ' ' << t->j << endl;
}

void add(int i, int j, cell* &last, cell* t)
{
	cell *e = new cell;
	e->i = i;
	e->j = j;
	e->num = t->num + 1;
	e->pred = t;
	e->next = NULL;
	last->next = e;
	last = e;
}

void solve()
{
    cell* t = new cell;
    cell* last;
    t->j = j0;
    t->i = i0;
    t->num = 1;
    t->pred = NULL;
    last = t;
    while ( t!=NULL )
    {
        b[t->i][t->j] = true;
        if ( a[t->i][t->j] == -2 )
        {
            last = t;
            break;
        }
        if ( t->i > 1 && !b[t->i-1][t->j] )
        {
			add(t->i -1, t->j, last, t);
            b[last->i][last->j] = true;
        }
        if ( t->i < n && !b[t->i+1][t->j] )
        {
			add(t->i +1, t->j, last, t);
            b[last->i][last->j] = true;
        }
        if ( t->j > 1 && !b[t->i][t->j-1] )
        {
			add(t->i, t->j -1, last, t);
            b[last->i][last->j] = true;
        }
        if ( t->j < m && !b[t->i][t->j+1] )
        {
			add(t->i, t->j +1, last, t);
            b[last->i][last->j] = true;
        }
        if ( a[t->i][t->j] > 1 )
        {
            int _i = hyp[a[t->i][t->j]][2];
            int _j = hyp[a[t->i][t->j]][3];
            if (!b[_i][_j])
            {
                add(_i, _j, last, t);
                b[_i][_j] = true;
            }
        }
        t = t->next;
    }
    if ( t!=NULL ) {
        cout << last->num << endl;
        output(last);
    }
    else cout << "Impossible";
}

int main()
{
    input();
    solve();
    return 0;
}