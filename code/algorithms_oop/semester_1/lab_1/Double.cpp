#include <iostream>

using namespace std;

void print_byte(unsigned char t)
{
	for (int i=7; i>=0; --i)
		((t)&(1<<i)) ? cout << 1 : cout << 0;
	cout << ' ';
}

void print_bytes(const unsigned char* mem, int size)
{
	for (int i=size-1; i>=0; --i)
		print_byte(mem[i]);
	cout << endl;
}

int main()
{
	double d;
	cin >> d;
	print_bytes(reinterpret_cast<const unsigned char*>(&d), sizeof(d));
	return 0;
}