#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"
#include <sys/time.h>
#include <iostream>

using namespace std;

const int MATRIX_SIZE = 2000;

int main(int argc, char** argv) {
    MPI::Init(argc, argv);
    int rank = MPI::COMM_WORLD.Get_rank();
    int size = MPI::COMM_WORLD.Get_size();
    if (rank == 0) {
        int* a = new int[MATRIX_SIZE * MATRIX_SIZE];
        int* b = new int[MATRIX_SIZE * MATRIX_SIZE];
        int* ans = new int[MATRIX_SIZE * MATRIX_SIZE];

        MPI::Status status;
        srand(0);

        // Generating matrix, output A

        for (int i = 0; i < MATRIX_SIZE; i++) {
            for (int j = 0; j < MATRIX_SIZE; j++) {
                int ij = i * MATRIX_SIZE + j;
                a[ij] = rand() % 100;
                b[ij] = rand() % 100;
                // cout << a[ij] << ' ';
            }
            // cout << endl;
        }

        // Output B, transposing B

        for (int i = 0; i < MATRIX_SIZE; i++) {
            for (int j = 0; j < MATRIX_SIZE; j++) {
                int ij = i * MATRIX_SIZE + j, ji = j * MATRIX_SIZE + i, temp;
                // cout << b[ij] << ' ';
                temp = b[ij];
                b[ij] = b[ji];
                b[ji] = temp;
            }
            // cout << endl;
        }

        if (size == 1) {
            for (int i = 0; i < MATRIX_SIZE; i++) {
                for (int j = 0; j < MATRIX_SIZE; j++) {
                    ans[i * MATRIX_SIZE + j] = 0;
                    for (int k = 0; k < MATRIX_SIZE; k++) {
                        ans[i * MATRIX_SIZE + j] += a[i * MATRIX_SIZE + k] * b[j * MATRIX_SIZE + k];
                    }
                }
            }
        } else {
            int _div = MATRIX_SIZE / (size - 1);
            int _mod = MATRIX_SIZE % (size - 1);
            for (int i = 1; i < size; i++) {
                MPI::COMM_WORLD.Send(b, MATRIX_SIZE * MATRIX_SIZE, MPI::INT, i, i);
            }
            for (int i = 1; i < size - 1; i++) {
                MPI::COMM_WORLD.Send(a + (i - 1) * _div * MATRIX_SIZE, _div * MATRIX_SIZE, MPI::INT, i, 54322);
            }
            MPI::COMM_WORLD.Send(a + (size - 1) * _div * MATRIX_SIZE, (_div + _mod) * MATRIX_SIZE, MPI::INT, size - 1, 54322);

            int cnt = 0, who = 0, t = 0;
            while (t < size - 1) {
                MPI::COMM_WORLD.Recv(&who, 1, MPI::INT, MPI_ANY_SOURCE, 1234, status);
                cnt = (who == size - 1) ? _div + _mod : _div;
                MPI::COMM_WORLD.Recv(ans + (who - 1) * _div * MATRIX_SIZE, cnt * MATRIX_SIZE, MPI::INT, MPI_ANY_SOURCE, who + 1235, status);
                t++;
            }
        }

        //Output answer

        /*
           for (int i = 0; i < MATRIX_SIZE; i++) {
           for (int j = 0; j < MATRIX_SIZE; j++) {
           cout << ans[i * MATRIX_SIZE + j] << ' ';
           }
           cout << endl;
           }
         */
        delete a;
        delete b;
        delete ans;
    } else {
        int* b = new int[MATRIX_SIZE * MATRIX_SIZE];
        int _div = MATRIX_SIZE / (size - 1);
        int _mod = MATRIX_SIZE % (size - 1);
        int cnt = (rank == size - 1) ? _div + _mod : _div;
        int* v = new int[cnt * MATRIX_SIZE];
        int* ans = new int[cnt * MATRIX_SIZE];

        MPI::Status status;

        MPI::COMM_WORLD.Recv(b, MATRIX_SIZE * MATRIX_SIZE, MPI::INT, 0, rank, status);
        MPI::COMM_WORLD.Recv(v, cnt * MATRIX_SIZE, MPI::INT, MPI_ANY_SOURCE, 54322, status);

        for (int row = 0; row < cnt; row++) {
            for (int i = 0; i < MATRIX_SIZE; i++) {
                ans[row * MATRIX_SIZE + i] = 0;
                for (int j = 0; j < MATRIX_SIZE; j++) {
                    ans[row * MATRIX_SIZE + i] += v[row * MATRIX_SIZE + j] * b[i * MATRIX_SIZE + j];
                }
            }
        }
        MPI::COMM_WORLD.Send(&rank, 1, MPI::INT, 0, 1234);
        MPI::COMM_WORLD.Send(ans, cnt * MATRIX_SIZE, MPI::INT, 0, rank + 1235);
        delete b;
        delete v;
        delete ans;
    }
    MPI_Finalize();
    return 0;
}
