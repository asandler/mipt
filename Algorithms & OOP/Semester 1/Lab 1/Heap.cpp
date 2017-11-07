#include <iostream>

using namespace std;

const int heap_size = 10;
int heap[heap_size] = {0, 5, 1, 3, 6, 7, 315, 1245, 40, -1};

void output(int *a, int size)
{
	for (int i=0; i<size; ++i)
		cout << a[i] << ' ';
	cout << endl;
}

void heap_down(int t, int size)
{
	while (true)
	{
		int child = 2*t+1;
		if (child>=size) break;
		if ((child+1<size)&&(heap[child+1]>heap[child]))
			child++;
		if (heap[t]<heap[child])
			swap(heap[t],heap[child]);
		t = child;
	}
}

void make_heap(int *a, int size)
{
	for (int i=size-1; i>=0; --i)
		heap_down(i,size);
}

void heapify(int *a, int size)
{
	for (int i=0; i<size; ++i)
	{
		swap(a[0],a[size-i-1]);
		heap_down(0,size-i-1);
	}
}


int main()
{
	make_heap(heap, heap_size);
	heapify(heap, heap_size);
	output(heap, heap_size);
	return 0;
}