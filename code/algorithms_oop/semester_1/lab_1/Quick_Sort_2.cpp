#include <iostream>
#include <string>
using namespace std;

template <typename T>
bool compare(const T &a, const T &b)
{
    return (a < b);
}

template <typename T>
void Swap(T &a, T &b)
{
    T t = a;
    a = b;
    b = t;
}

template <typename T>
int partition(T *a, int size, T x, bool (*cmp)(const T &, const T &))
{
	int i=0, j=size-1;

	do
	{
		while ((*cmp)(a[i],x)) i=i+1;
		while ((*cmp)(x,a[j])) j=j-1;

		if (i<=j)
		{
			Swap(a[i],a[j]);
			++i;
			--j;
		}

	} while (i<=j);

	return i;	
}

template <typename T>
void output(T *a, int t)
{
	for (int i=0;i<t;i++)
		cout << a[i]<<" ";
	cout << endl;
}

template <typename T>
void quick_sort(T *a, int size)
{
	if (size == 1) return;
	int t=partition(a, size, a[(size-1)/2], &compare<T>);
	quick_sort(a, t);
	quick_sort(&a[t], size-t);	
}

int main()
{	
    string b[10] = {"457", "erysdf", "ew", "srew", "swap", "1", "0", "i", "==", "*=",};
    quick_sort(b,10);
    output(b,10);
	return 0;
}