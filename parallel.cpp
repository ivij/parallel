#include <iostream>
#include <time.h>
#include <chrono>
#include <stdio.h>
#include <fstream>
#include <cstdlib>
#include <string>
#include <vector>
#include <ctime>
#include <pthread.h>
#include <thread>

using namespace std;
using namespace std::chrono;
#define NUM_THREADS 10

struct MultipleThreadTask
{
	int **A, **B, **C;
	int start;
	int end;
    	int power;
};



void *matrixmultiplication(void* arg)
{
    MultipleThreadTask *task = (struct MultipleThreadTask *)arg;

	for (int i = task-> start; i < task-> end ; i++)
	{ 
		for (int j = 0; j < task-> power; j++)
		{
			for (int k = 0; k < task-> power; k++)
			{
				task->C[i][j] += task->A[i][k] * task->B[k][j];
			}
		}
	}
}

int main(void)
{
    pthread_t threads[NUM_THREADS];
    int power = 1000;
    srand(time(0));

    int **A,**B,**C;

    A = (int **)  malloc(power * sizeof(int*));
    B = (int **)  malloc(power * sizeof(int*));
    C = (int **)  malloc(power * sizeof(int*));

	for (int i = 0; i < power; i++)
	{
		A[i] = (int*) malloc(power * sizeof(int));
		B[i] = (int*) malloc(power * sizeof(int));
		C[i] = (int*) malloc(power * sizeof(int));
	}


	for( int i = 0; i < power; ++i)
    {
        for( int j = 0;  j < power; ++j)
        {
            A[i][j] = rand() % 100;
        }
    }

    for( int i = 0; i < power; ++i)
    {
        for( int j = 0;  j < power; ++j)
        {
            B[i][j] = rand() % 100;
        }
    }

    for( int i = 0; i < power; ++i)
    {
        for( int j = 0;  j < power; ++j)
        {
            C[i][j] = 0;
        }
    }
	auto startTime = high_resolution_clock::now();


	for (size_t i = 0; i < NUM_THREADS; i++)
	{
        struct MultipleThreadTask *task = (struct MultipleThreadTask*)malloc(sizeof(struct MultipleThreadTask));

        task->A = A;
        task->B = B;
        task->C = C;
        task->power = power;
        task->start = (power/NUM_THREADS)*i;
        task->end = (i+1) == NUM_THREADS ? power : ((power/NUM_THREADS)*(i + 1));
		pthread_create(&threads[i], NULL, matrixmultiplication, (void*)task);
	}

	for (size_t i = 0; i < NUM_THREADS; i++)
	{
		pthread_join(threads[i], NULL);
	}

	auto stopTime = high_resolution_clock::now();
	auto TotalTime = duration_cast<microseconds>(stopTime - startTime);

	remove("paralleloutput.txt");
	ofstream seqout;
	seqout.open ("paralleloutput.txt");
	seqout << "Matrix (N * N): " << power << " * " << power << " / Time Taken: "<< TotalTime.count() << "ms";
	seqout.close();
    cout << "Matrix (N * N): " << power << " * " << power << " / Time Taken:"<< TotalTime.count() << "ms";

    return 0;
}
