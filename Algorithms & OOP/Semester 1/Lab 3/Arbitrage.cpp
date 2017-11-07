#include <iostream>
#include <cmath>

using namespace std;

double a[100][100];
int n;

void input()
{
	cin >> n;
	for (int i=0; i<n; ++i)
		for (int j=0; j<n; ++j)
		{
			cin >> a[i][j];
			a[i][j] = log(a[i][j]);
		}
}

void step(int k)
{
	double d[100][100];
	for (int i=0; i<n; ++i)
		for (int j=0; j<n; ++j)
			d[i][j] = max(a[i][k] + a[k][j], a[i][j]);
	for (int i=0; i<n; ++i)
		for (int j=0; j<n; ++j)
			a[i][j] = d[i][j];
}

void solve()
{
	for (int k=0; k<n; ++k)
		step(k);
	bool flag = false;
	for (int i=0; i<n; ++i)
		for (int j=0; j<n; ++j)
			if (a[i][j] + a[j][i] > 0)
			{
				flag = true;
				break;
			}
	flag ? cout << "YES" << endl : cout << "NO" << endl;
}

int main()
{
	input();
	solve();
	return 0;
}