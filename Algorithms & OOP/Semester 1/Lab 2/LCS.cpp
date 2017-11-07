#include <iostream>
#include <string>

using namespace std;

int a[2000][2000] = {0};
string s1,s2;
bool flag = false;

void output(int i, int j)
{
	if (i == 0 || j == 0) return;
	if ( s1[i-1] == s2[j-1] )
	{
		output(i-1, j-1);
		cout << s1[i-1];
		flag = true;
	}
	else 
		a[i][j-1] > a[i-1][j] ? output(i, j-1) : output(i-1, j);
}

int main()
{
	getline(cin,s1);
	getline(cin,s2);
	int n = s1.length();
	int m = s2.length();
	for (int i=0; i<n; i++)
		for (int j=0; j<m; j++)
			if ( s1[i] == s2[j] ) 
				a[i+1][j+1] = a[i][j] + 1;
			else a[i+1][j+1] = max(a[i+1][j], a[i][j+1]);
	output(n,m);
	if (!flag) cout << "Empty";
	return 0;
}