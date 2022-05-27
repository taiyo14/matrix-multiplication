//
//  main.cpp
//  matrix multiplication
//
//  Created by Kimtaiyo Mech on 2/22/22.
//

#include <iostream>
#include <math.h>
using namespace std;

typedef int *  Array;
typedef int ** Matrix;

void naiveMatrix(Matrix matrix1, Matrix matrix2, Matrix *matrix3, int row, int col);
void initMatrix(Matrix *matrix, int row, int col, int arr[] = nullptr);
void printMatrix(Matrix matrix, int row, int col);

int main(int argc, const char * argv[]) {
    
    int row1 = 3, col1 = 3;
    int arr1[9] = {1,2,3,4,5,6,7,8,9}, arr2[9] = {10,11,12,13,14,15,16,17,18};
    
    Matrix matrix1 = nullptr;
    Matrix matrix2 = nullptr;
    Matrix matrix3 = nullptr;
    
    initMatrix(&matrix1, row1, col1, arr1);
    initMatrix(&matrix2, row1, col1, arr2);
    initMatrix(&matrix3, row1, col1);
    printMatrix(matrix1, row1, col1);
    printMatrix(matrix2, row1, col1);
    
    cout << endl;
    naiveMatrix(matrix1, matrix2, &matrix3, row1, col1);
    printMatrix(matrix3, row1, col1);
    
    
    return 0;
}

void initMatrix(Matrix *matrix, int row, int col, int arr[]) {
    
    *matrix = new int*[row];
    
    int k = 0;
    
    for (int i = 0; i < row; i++) {
        (*matrix)[i] = new int[col];
        if (arr != nullptr) {
            for (int j = 0; j < col; j++) {
                (*matrix)[i][j] = arr[k];
                k++;
            }
        }
    }
}

void printMatrix(Matrix matrix, int row, int col) {
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            if (j == 0)
                cout << "|" << matrix[i][j];
            else if (j == col-1)
                cout << " " << matrix[i][j] << "|\n";
            else
                cout << " " << matrix[i][j];
        }
    }
}

void naiveMatrix(Matrix matrix1, Matrix matrix2, Matrix *matrix3, int row, int col) {
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            (*matrix3)[i][j] = 0;
            for (int k = 0; k < col; k++) {
                (*matrix3)[i][j] += matrix1[i][k] * matrix2[k][j];
            }
        }
    }
}
