#include <iostream>
using namespace std;


void swap(int& i, int& j)
{
	int t=i;
	i=j;
	j=t;
}


int partition(int *array, int size, int x)
{
	int i=0, j=size-1;

	do
	{
		while (array[i]<x) i=i+1; 
		while (array[j]>x) j=j-1;

		if (i<=j) 
		{
			swap(array[i],array[j]);
			++i;
			--j;
		}

	} while (i<=j);

	return i;	
}


void output(int *array, int t)
{
	for (int i=0;i<t;i++)
		cout << array[i]<<" ";
	cout << endl;
}


void quick_sort(int *array, int size)
{
	if (size == 1) return;
	int t=partition(array, size, array[(size-1)/2]);
	quick_sort(array, t);
	quick_sort(&array[t], size-t);	
}

int sorted(int *array, int t)
{
    int p=1;
    for (int i=0; i<t-1; ++i)
        if (array[i]>array[i+1]) p=0;
    return p;
}


int gen_perm(int *a, int t)
{
    int i=t-1;
    while ((i>0)&&(a[i]<a[i-1]))
        --i;
    if (i>0)
    {
        int j, ind=t, min=10000;
        for (j=t-1;j>i-1;--j)
            if ((a[j]<min)&&(a[j]>a[i-1]))
            {
                min=a[j];
                ind=j;
            }
        swap(a[i-1],a[ind]);
        for (i=i;i<t;++i)
            for (j=i+1;j<t;++j)
                if (a[i]>a[j]) swap(a[i],a[j]);
        return 1;
    }
    else return 0;
}

void init_array(int *array, int t)
{
    for (int i=0; i<t; ++i)
        array[i]=i;
}

void copy_array(int *a, int *b, int t)
{
    for (int i=0; i<t; ++i)
        a[i]=b[i];
}

// perebor vseh perestanovok dlini 10 i sortirovka ih. Esli vse sortiruutsya pravilno, to programma vivodit
// 10! i pishet "xorowo"
int main()
{	
	int a[10], b[10], y=0, size = sizeof(a) / sizeof (a[0]);
    init_array(b, size);
    int p=1;
    int p0=1;
    while ((p)&&(p0))
    {
        copy_array(a,b, size);
        quick_sort(a, size);
        p0=sorted(a, size);
        p=gen_perm(b, size);
        if (p0==0) cout << "ploxo";
        ++y;
		if ((y % 10000) == 0)
			cout << y << endl;
	}
    cout << "xorowo " << y;
	return 0;
}