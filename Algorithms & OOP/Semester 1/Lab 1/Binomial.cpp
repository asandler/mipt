#include <iostream>
using namespace std;

int a[63];

void input(int &n)
{
	cout << "This program calculates binomial coefficients\n";
	cout << "Enter n = ";
	cin >> n;
	a[0] = 1;
}

void calc(int n)
{
	for (int i=1; i<=n; ++i)
		for (int j=n; j>0; --j)
			a[j] = a[j] + a[j-1];
}

void main()
{
	int n;
	input(n);
	calc(n);
	for (int i=0;i<=n;++i)
		cout << a[i] <<" ";
	cout << endl;
}