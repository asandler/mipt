#include <iostream>
#include <string>
#include <map>
#include <queue>

using namespace std;

int d;

map <string, queue<string> > M;
map <string, int> A;

queue <string> T;

bool input()
{
	cin >> d;
	string asdfgh, nk;
	int kpm;
	for (int i=0; i<d; ++i)
	{
		cin >> asdfgh >> kpm;
		if (A[asdfgh] == 1) return false;
		A[asdfgh] = 1;
		T.push(asdfgh);
		for (int i=0; i<kpm; ++i)
		{
			cin >> nk;
			M[asdfgh].push(nk);
		}
	}
	return true;
}

bool DFS(string s)
{
	if (A[s] == 1) return false;
	A[s] = 1;
	while (!M[s].empty())
	{
		if (!DFS(M[s].front()) ) return false;
		M[s].pop();
	}
	A[s] = 2;
	return true;
}

string solve()
{
	A.clear();
	while ( !T.empty() )
	{
		while ( !T.empty() && A[T.front()] == 2 )
			T.pop();
		if ( T.empty() ) break;
		if ( !DFS(T.front()) ) return "NOT CORRECT";
	}
	return "CORRECT";
}

int main()
{
	!input() ? cout << "NOT CORRECT" :
	cout << solve();
}