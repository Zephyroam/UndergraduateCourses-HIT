/* 
 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 */ 
#include <stdio.h>
#include "cachelab.h"
int is_transpose(int M, int N, int A[N][M], int B[M][N]);

/* 
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded. 
 */

void swap(int *p1,int *p2)
{
    int temp;
    temp = *p1;
    *p1 = *p2;
    *p2 = temp;
}

char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{
    int tmp[8];
    if (N == 32)
    {
        for (int i = 0; i < N; i += 8)
            for (int j = 0; j < M; j += 8)
            {
                for (int i1 = i; i1 < i + 8; i1++)
                {
                    for (int j1 = j; j1 < j + 8; j1++)
                        tmp[j1 - j] = A[i1][j1];
                    for (int j1 = j; j1 < j + 8; j1++)
                        B[j1][i1] = tmp[j1 - j];
                }
            }
    }
    else if (N == 64)
    {

       for (int i = 0; i < N; i += 8) 
       {
            for (int j = 0; j < M; j += 8)
            {
                for (int k = 0; k < 8 / 2; k++) 
                {
                    
                    tmp[0] = A[k + i][j];
                    tmp[1] = A[k + i][j + 1];
                    tmp[2] = A[k + i][j + 2];
                    tmp[3] = A[k + i][j + 3];
                    tmp[4] = A[k + i][j + 4];
                    tmp[5] = A[k + i][j + 5];
                    tmp[6] = A[k + i][j + 6];
                    tmp[7] = A[k + i][j + 7];

                    B[j + 0][k + i] = tmp[0];
                    B[j + 1][k + i] = tmp[1];
                    B[j + 2][k + i] = tmp[2];
                    B[j + 3][k + i] = tmp[3];

                    B[j + 0][k + 4 + i] = tmp[4];
                    B[j + 1][k + 4 + i] = tmp[5];
                    B[j + 2][k + 4 + i] = tmp[6];
                    B[j + 3][k + 4 + i] = tmp[7];
                }
                for (int k = 0; k < 8 / 2; k++) 
                {
                    tmp[0] = A[i + 4][j + k], tmp[4] = A[i + 4][j + k + 4];
                    tmp[1] = A[i + 5][j + k], tmp[5] = A[i + 5][j + k + 4];
                    tmp[2] = A[i + 6][j + k], tmp[6] = A[i + 6][j + k + 4]; 
                    tmp[3] = A[i + 7][j + k], tmp[7] = A[i + 7][j + k + 4];
                    
                    swap(&B[j + k][i + 4], &tmp[0]);
                    swap(&B[j + k][i + 5], &tmp[1]);
                    swap(&B[j + k][i + 6], &tmp[2]);
                    swap(&B[j + k][i + 7], &tmp[3]);
                    
                    B[j + k + 4][i + 0] = tmp[0], B[j + k + 4][i + 4 + 0] = tmp[4];
                    B[j + k + 4][i + 1] = tmp[1], B[j + k + 4][i + 4 + 1] = tmp[5];
                    B[j + k + 4][i + 2] = tmp[2], B[j + k + 4][i + 4 + 2] = tmp[6];
                    B[j + k + 4][i + 3] = tmp[3], B[j + k + 4][i + 4 + 3] = tmp[7];
                }
            }
        }
    }
    else
    {
        for (int i = 0; i < N; i += 8)
            for (int j = 0; j < M; j += 8)
            {
                for (int i1 = i; i1 < i + 8 && i1 < N; i1++)
                {
                    for (int j1 = j; j1 < j + 8 && j1 <M; j1++)
                        tmp[j1 - j] = A[i1][j1];
                    for (int j1 = j; j1 < j + 8 && j1 <M; j1++)
                        B[j1][i1] = tmp[j1 - j];
                }
            }
    }
    


}

/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions()
{
    /* Register your solution function */
    registerTransFunction(transpose_submit, transpose_submit_desc); 

}

/* 
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}

