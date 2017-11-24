#include <iostream>
#include <algorithm>

using namespace std;

struct segment
{
	int left, right;
};

segment a[100000];
int b[1000000];

int comp(const void * a, const void * b)
{
	const segment* name1 = (const segment*) a;
	const segment* name2 = (const segment*) b;
	if (name1->left > name2->left || (name1->left == name2->left && name1->right < name2->right) ) return 1;
	else
		if (name1->left == name2->left && name1->right == name2->right ) return 0;
		else return -1;
}

int fill_array(int &start, int &up_bound, int n)
{
	int num = start, new_bound = up_bound;
	while ( start < n && a[start].left <= up_bound )
	{
		if ( a[start].right > new_bound )
		{
			new_bound = a[start].right;
			num = start;
		}
		start++;
	}
	if ( new_bound > up_bound )
	{
		up_bound = new_bound;
		return num;
	}
	else return -1;
}

void solve(int x, int y, int n)
{
	int up_bound = x, low_bound = 0, res = 0, i = 0;
	while ( res > -1 && up_bound < y )
	{
		res = fill_array(low_bound, up_bound, n);
		if ( res > -1 ) b[i++] = res;
	}
	if ( up_bound >= y )
		for (int j=0; j<i; ++j)
			cout << '[' << a[b[i]].left << ':' << a[b[i]].right << ']' << endl;
	else 
		cout << "net takoi sistemb|" << endl;
}

int main()
{
	int n, x, y;
	cin >> n;
	for (int i=0; i<n; ++i)
		cin >> a[i].left >> a[i].right;
	cout << "enter left and right bounds of covering segment:" << endl;
	cin >> x >> y;
	qsort(a,n,sizeof(a[0]),&comp);
	solve(x,y,n);
	system("pause");
	return 0;
}