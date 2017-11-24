#include <iostream>
#include <string>

using namespace std;

int b[100][2], a[100], c[100];

int bs(int l, int r, int t)
{
	if (l==r)
		if (t>b[l][0]) return l+1;
		else return l;
	int x = (l + r) /2;
	b[x][0] < t ? bs(x+1,r,t) : bs(l,x,t);
}


void print_mis(int t)
{
	if (t<0) return;
	print_mis(c[t]);
	cout << a[t] << ' ';
}


int main()
{
	int n, r = 1, t = 0;
	cout << "This program counts maximal increasing subsequience in entered sequience" << endl;
	cout << "Enter integer n (length of sequience)>> ";
	cin >> n;
	if (n<1)
	{
		cout << "There are no any subsequiences" << endl;
		return 0;
	}
	cout << "Enter a[i] >>";
	for (int i=0; i<n; ++i)
		cin >> a[i];
	b[0][0] = -1;
	b[0][1] = -1;
	b[1][0] = a[0];
	b[1][1] = 0;
	c[0] = -1;
	for (int i=1; i<n; ++i)
	{
		t = bs(1,r,a[i]);
		b[t][0] = a[i];
		b[t][1] = i;
		if (t>r) r++;
		c[i] = b[t-1][1];
	}
	cout << r << endl;
	t = b[r][1];
	print_mis(t);
	return 0;
}