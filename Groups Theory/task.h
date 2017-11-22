#pragma once
#include <iostream>
#include <map>
#include <queue>
#include <string.h>
#include <stdio.h>
#include "permutation.h"
#include "factorial.h"
#include "bitarray.h"
#include "queue.h"

template<size_t N>
class Task
{
	public:
		Task();
		void Solve(bool, bool);
		template<size_t _N>
		friend std::istream& operator>>(std::istream&, Task<_N>&);
		template<size_t _N>
		friend std::ostream& operator<<(std::ostream&, const Task<_N>&);
	private:
		int Edges[N * N][2];
		int E;
		size_t Growth[N * N];
};

template<size_t N>
Task<N>::Task()
{
	E = 0;
	memset(Growth, 0, sizeof(Growth));
}

template<size_t N>
void Task<N>::Solve(bool output, bool file_output)
{
	Factorial fact(N);
	BitArray Visited(fact[N]);
	//std::queue <size_t> Q;
	Queue<unsigned int> Q(fact[N]);
	Permutation start(N);
	int depth = 1;
	size_t layer_end = 0;
	memset(Growth, 0, sizeof(Growth));
	Q.push(start.Hash());
	Visited[start.Hash()] = true;
	FILE *output_tree = NULL;
	char filename[100];
	if (file_output)
	{
		sprintf(filename, "output.%d.depth.1", (int)N);
		output_tree = fopen(filename, "w+");
	}
	while (!Q.empty()) {
		Permutation top(N, Q.front());
		for (int i = 0; i < E; i++) {
			Permutation cur = top.Swap(Edges[i][0], Edges[i][1]);
			if (!Visited[cur.Hash()]) {
				Visited[cur.Hash()] = true;
				Q.push(cur.Hash());
				Growth[depth]++;
				if (file_output)
				{
					fprintf(output_tree, "%ld-%ld\n", (long)top.Hash(), (long)cur.Hash());
				}
			}
		}
		Q.pop();
		if (top.Hash() == layer_end && !Q.empty())
		{
			if (file_output && depth > 0)
			{
				fclose(output_tree);
				sprintf(filename, "output.%d.depth.%d", (int)N, (int)depth + 1);
				output_tree = fopen(filename, "w+");
			}
			if (output)
			{
				std::cout << depth << "\t" << Growth[depth] << std::endl;
			}
			layer_end = Q.back();
			depth++;
		}
	}
	if (file_output)
	{
		fclose(output_tree);
	}
}

template<size_t N>
std::istream& operator>>(std::istream& is, Task<N>& t)
{
	is >> t.E;
	for (int i = 0; i < t.E; i++)
		is >> t.Edges[i][0] >> t.Edges[i][1];
	return is;
}

template<size_t N>
std::ostream& operator<<(std::ostream& os, const Task<N>& t)
{
	for (size_t i = 1; i < N * (N - 1) / 2 + 1; i++)
	{
		if (t.Growth[i] > 0)
		{
			os << i << "\t" << t.Growth[i] << std::endl;
		}
	}
	return os;
}
