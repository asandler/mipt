#include "task.h"
#include <iostream>

using namespace std;

int main()
{
	Task<12> task;
	cin >> task;
	task.Solve(true, true); // вывод по мере готовности с выводом графа в файлы
	// cout << task; // вывод по завершению вычисления
	return 0;
}
