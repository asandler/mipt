#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"
#include <sys/time.h>
#include <iostream>

using namespace std;

const int MATRIX_SIZE = 960;


int main(int argc, char** argv) {
	MPI::Init(argc, argv);
	int rank = MPI::COMM_WORLD.Get_rank();
	int size = MPI::COMM_WORLD.Get_size();
        int _div = MATRIX_SIZE / size;
        int *query = new int[_div * MATRIX_SIZE];
        int *answer = new int[_div * MATRIX_SIZE];
        int *a = new int[MATRIX_SIZE * MATRIX_SIZE];
        int *b = new int[MATRIX_SIZE * MATRIX_SIZE];
        int *ans = new int[MATRIX_SIZE * MATRIX_SIZE];
        MPI::Status status;

        if (rank == 0) {
                srand(0);
                for (int i = 0; i < MATRIX_SIZE; i++) {
                	for (int j = 0; j < MATRIX_SIZE; j++) {
                                int ij = i * MATRIX_SIZE + j;
                        	a[ij] = rand() % 100;
                        	b[ij] = rand() % 100;
	//			cout << a[ij] << ' ';
			}
	//		cout << endl;
                }
                for (int i = 0; i < MATRIX_SIZE; i++) {
                        for (int j = 0; j < MATRIX_SIZE; j++) {
                                int ij = i * MATRIX_SIZE + j, ji = j * MATRIX_SIZE + i, temp;
	//			cout << b[ij] << ' ';
                                temp = b[ij];
                                b[ij] = b[ji];
                                b[ji] = temp;
                        }
	//		cout << endl;
                }
        }
        MPI_Bcast(b, MATRIX_SIZE * MATRIX_SIZE, MPI::INT, 0, MPI::COMM_WORLD);
        MPI_Scatter(a, _div * MATRIX_SIZE, MPI::INT, query, _div * MATRIX_SIZE, MPI::INT, 0, MPI::COMM_WORLD);
        for (int row = 0; row < _div; row++) {
                for (int i = 0; i < MATRIX_SIZE; i++) {
                        answer[row * MATRIX_SIZE + i] = 0;
                        for (int j = 0; j < MATRIX_SIZE; j++) {
                                answer[row * MATRIX_SIZE + i] += query[row * MATRIX_SIZE + j] * b[i * MATRIX_SIZE + j];
                        }
                }
        }
        MPI_Gather(answer, _div * MATRIX_SIZE, MPI::INT, ans, _div * MATRIX_SIZE, MPI::INT, 0, MPI::COMM_WORLD);
        /*
        if (rank == 0) {
                for (int i = 0; i < MATRIX_SIZE; i++) {
                        for (int j = 0; j < MATRIX_SIZE; j++) {
                                cout << ans[i * MATRIX_SIZE + j] << ' ';
                        }
                        cout << endl;
                }
        }
        */
        delete answer;
        delete query;
        delete a;
        delete b;
        delete ans;
	MPI_Finalize();
	return 0;
}
