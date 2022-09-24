//
//  main.cpp
//  matrix multiplication
//
//  Created by Kimtaiyo Mech on 2/22/22.
//

#include <iostream>
#include <math.h>
#include <iomanip>
#include <random>
#include <vector>
#include <chrono>
using namespace std;
using namespace std::chrono;

typedef vector<vector<int>> Matrix;
enum matrices {A11, A12, A21, A22, B11, B12, B21, B22};
enum methods {classical_method, naiveDC_method, strassen_method};

Matrix classical(Matrix matrix1, Matrix matrix2);
Matrix naiveDC(Matrix matrix1, Matrix matrix2);
Matrix strassen(Matrix matrix1, Matrix matrix2);

Matrix initMatrix(int n, bool populate = true);
Matrix subtractMatrix(Matrix m1, Matrix m2);
Matrix addMatrix(Matrix m1, Matrix m2);

void printMatrix(Matrix matrix);
void combine(Matrix m11, Matrix m12, Matrix m21, Matrix m22, Matrix &m);
void split(Matrix m1, Matrix m2, Matrix *subMatrices);

bool compare(Matrix m1, Matrix m2, Matrix m3);
double testing(int n, int rounds, int method);


int LOWER_BOUND = -9, UPPER_BOUND = 9;
int ROUNDS = 5;
int min_n = 2, max_n = 512;

int main(int argc, const char * argv[]) {
        
    Matrix test1 =  {{2, 0, -1, 6},
                     {3, 7, 8, 0},
                     {-5, 1, 6, -2},
                     {8, 0, 2, 7}};

    Matrix test2 = {{0, 1, 6, 3},
                    {-2, 8, 7, 1},
                    {2, 0, -1, 0},
                    {9, 1, 6, -2}};
    Matrix result1 = classical(test1, test2);
    Matrix result2 = naiveDC(test1, test2);
    Matrix result3 = strassen(test1, test2);
    
    cout << "Sanity check\n";
    cout << "------------\n\n";
    cout << "Testing the 3 algorithms with:" << endl;
    cout << "Matrix 1:" << endl;
    printMatrix(test1);
    cout << "Matrix 2:" << endl;
    printMatrix(test2);
    cout << "\nResults:" << endl;
    cout << "Classical matrix multiplication:" << endl;
    printMatrix(result1);
    cout << "Naive divide-and-conquer matrix multiplication:" << endl;
    printMatrix(result2);
    cout << "Strassen’s matrix multiplication:" << endl;
    printMatrix(result3);

    cout << "\nResult: " << (compare(result1, result2, result3) ? "✅" : "❌") << endl;
    
    int choice = 1;
    while (choice != 3) {
        cout << "Menu:\n";
        cout << "1. Test algrotithm times\n";
        cout << "2. Test for any value of n\n";
        cout << "3. Exit\n";
        cout << "\nInput: ";
        cin >> choice;
        switch (choice) {
            case 1: {
                cout << "Algorithm times:\n";
                cout << "----------------\n\n";
                for (int i = min_n; i <= max_n; i*=2) {
                    cout <<  "Calculating the time it takes to operate on n=" << i << "...\n";
                    double classicalTime = testing(i, ROUNDS, classical_method);
                    double naiveTime = testing(i, ROUNDS, naiveDC_method);
                    double strassenTime = testing(i, ROUNDS, strassen_method);

                    cout << "Results:\n";
                    cout << "Classical: " << classicalTime << " microseconds\n";
                    cout << "Naive DC: " << naiveTime << " microseconds\n";
                    cout << "Strassen: " << strassenTime << " microseconds\n";
                    cout << endl;
                }
                break;
            }
            case 2: {
                int n = 1;
                cout << "Input any value of n: ";
                cin >> n;
                Matrix m1 = initMatrix(n);
                Matrix m2 = initMatrix(n);
                Matrix m3 = classical(m1, m2);
                Matrix m4 = naiveDC(m1, m2);
                Matrix m5 = strassen(m1, m2);
                cout << "\nTesting the 3 algorithms with:" << endl;
                cout << "Matrix 1:" << endl;
                printMatrix(m1);
                cout << "Matrix 2:" << endl;
                printMatrix(m2);
                cout << "\nResults:" << endl;
                cout << "Classical matrix multiplication:" << endl;
                printMatrix(m3);
                cout << "Naive divide-and-conquer matrix multiplication:" << endl;
                printMatrix(m4);
                cout << "Strassen’s matrix multiplication:" << endl;
                printMatrix(m5);
                break;
            }
            default: {
                choice = 3;
                break;
            }
        }
    }
    
    return 0;
}

Matrix initMatrix(int dimension, bool populate) {

    int n = pow(2, ceil(log2(dimension)));

    Matrix matrix;
    if (!populate) {
        for (int i = 0; i < n; i++) {
            vector<int> temp(n);
            matrix.push_back(temp);
        }
    }
    else {
        random_device dev;
        mt19937 rng(dev());
        uniform_int_distribution<mt19937::result_type> dist(LOWER_BOUND,UPPER_BOUND);

        if (dimension == n) {            // expansion not required so just populate it
            for (int i = 0; i < dimension; i++) {
                vector<int> temp;
                for (int j = 0; j < dimension; j++) {
                    temp.push_back(dist(rng));
                }
                matrix.push_back(temp);
            }
        }
        else {                          // expansion into power of 2 of n and populate
            for (int i = 0; i < n; i++) {
                vector<int> temp(n);
                if (i < dimension) {
                    for (int j = 0; j < n; j++) {
                        if (j < dimension) {
                            temp.at(j) = dist(rng);
                        }
                    }
                    matrix.push_back(temp);
                }
                else {
                    matrix.push_back(temp);
                }
            }
        }
    }
    return matrix;
}

void printMatrix(Matrix matrix) {
    int n = (int)matrix.size();
    int width = (int)to_string(matrix[0][0]).length()+1;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (j == 0)
                cout << "| " << setw(width) << left << matrix[i][j];
            else if (j == n-1)
                cout << " " << setw(width) << left << matrix[i][j] << "|\n";
            else
                cout << " " << setw(width) << left << matrix[i][j];
        }
    }
}

Matrix subtractMatrix(Matrix m1, Matrix m2) {
    Matrix temp;
    int n = (int)m1.size();
    for (int i = 0; i < n; i++) {
        vector<int> vec;
        for (int j = 0; j < n; j++) {
            vec.push_back(m1[i][j] - m2[i][j]);
        }
        temp.push_back(vec);
    }
    return temp;
}

Matrix addMatrix(Matrix m1, Matrix m2) {
    Matrix temp;
    int n = (int)m1.size();
    for (int i = 0; i < n; i++) {
        vector<int> vec;
        for (int j = 0; j < n; j++) {
            vec.push_back(m1[i][j] + m2[i][j]);
        }
        temp.push_back(vec);
    }
    return temp;
}

void combine(Matrix m11, Matrix m12, Matrix m21, Matrix m22, Matrix &m) {
    int k = (int)m.size()/2;

    for (int i = 0; i < k; i++) {
        for (int j = 0; j < k; j++) {
            m[i][j] = m11[i][j];
            m[i][k+j] = m12[i][j];
            m[k+i][j] = m21[i][j];
            m[k+i][k+j] = m22[i][j];
        }
    }
}

// returns the size of the sub matrices
void split(Matrix m1, Matrix m2, Matrix *subMatrices) {
    int k = (int)m1.size()/2;

    for (int i = 0; i < k; i++) {
        for (int j = 0; j < k; j++) {
            subMatrices[A11][i][j] = m1[i][j];
            subMatrices[A12][i][j] = m1[i][k+j];
            subMatrices[A21][i][j] = m1[k+i][j];
            subMatrices[A22][i][j] = m1[k+i][k+j];
            subMatrices[B11][i][j] = m2[i][j];
            subMatrices[B12][i][j] = m2[i][k+j];
            subMatrices[B21][i][j] = m2[k+i][j];
            subMatrices[B22][i][j] = m2[k+i][k+j];
        }
    }
}

bool compare(Matrix m1, Matrix m2, Matrix m3) {
    int n = (int)m1.size();
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (m1[i][j] != m2[i][j] || m1[i][j] != m3[i][j]) {
                return false;
            }
        }
    }
    return true;
}

double testing(int n, int rounds, int method) {
    double total = 0.0;
    
    for (int j = 0; j < rounds; j++) {
        Matrix m1 = initMatrix(n);
        Matrix m2 = initMatrix(n);
        
        auto start = high_resolution_clock::now();
        switch (method) {
            case classical_method: {
                Matrix classic = classical(m1, m2);
                break;
            }
            case naiveDC_method: {
                Matrix naive = naiveDC(m1, m2);
                break;
            }
            case strassen_method: {
                Matrix strass = strassen(m1, m2);
                break;
            }
            default:
                break;
        }
        auto stop = high_resolution_clock::now();
        
        duration<double, std::micro> duration = stop - start;
        total += duration.count();
    }

    return total/rounds;
}

Matrix classical(Matrix matrix1, Matrix matrix2) {
    Matrix matrix3;
    int n = (int)matrix1.size();
    for (int i = 0; i < n; i++) {
        vector<int> temp(n);
        for (int j = 0; j < n; j++) {
            for (int k = 0; k < n; k++) {
                temp.at(j) += matrix1[i][k] * matrix2[k][j];
            }
        }
        matrix3.push_back(temp);
    }
    return matrix3;
}

Matrix naiveDC(Matrix A, Matrix B) {
    int n = (int)A.size();
    Matrix C = initMatrix(n, 0);

    if (n == 1) {
        C[0][0] = A[0][0] * B[0][0];
        return C;
    }

    Matrix subMatrices[8];
    for (int i = 0; i < 8; i++) {
        subMatrices[i] = initMatrix(n/2, 0);
    }
    split(A, B, subMatrices);

    Matrix P1 = naiveDC(subMatrices[A11], subMatrices[B11]);    // (A11)(B11)
    Matrix P2 = naiveDC(subMatrices[A12], subMatrices[B21]);    // (A12)(B21)
    Matrix P3 = naiveDC(subMatrices[A11], subMatrices[B12]);    // (A11)(B12)
    Matrix P4 = naiveDC(subMatrices[A12], subMatrices[B22]);    // (A12)(B22)
    Matrix P5 = naiveDC(subMatrices[A21], subMatrices[B11]);    // (A21)(B11)
    Matrix P6 = naiveDC(subMatrices[A22], subMatrices[B21]);    // (A22)(B21)
    Matrix P7 = naiveDC(subMatrices[A21], subMatrices[B12]);    // (A21)(B12)
    Matrix P8 = naiveDC(subMatrices[A22], subMatrices[B22]);    // (A22)(B22)

    Matrix C11 = addMatrix(P1, P2); // C11 = (A11)(B11) + (A12)(B21)
    Matrix C12 = addMatrix(P3, P4); // C12 = (A11)(B12) + (A12)(B22)
    Matrix C21 = addMatrix(P5, P6); // C21 = (A21)(B11) + (A22)(B21)
    Matrix C22 = addMatrix(P7, P8); // C22 = (A21)(B12) + (A22)(B22)

    combine(C11, C12, C21, C22, C);

    return C;
}

Matrix strassen(Matrix A, Matrix B) {
    int n = (int)A.size();
    Matrix C = initMatrix(n, 0);

    if (n == 1) {
        C[0][0] = A[0][0] * B[0][0];
        return C;
    }

    Matrix subs[8];
    for (int i = 0; i < 8; i++) {
        subs[i] = initMatrix(n/2, 0);
    }
    split(A, B, subs);

    Matrix P1 = strassen(subs[A11], subtractMatrix(subs[B12], subs[B22]));                        // P1 = A11(B12-B22)
    Matrix P2 = strassen(addMatrix(subs[A11], subs[A12]), subs[B22]);                             // P2 = (A11+A12)B22
    Matrix P3 = strassen(addMatrix(subs[A21], subs[A22]), subs[B11]);                             // P3 = (A21+A22)B11
    Matrix P4 = strassen(subs[A22], subtractMatrix(subs[B21], subs[B11]));                        // P4 = A22(B21-B11)
    Matrix P5 = strassen(addMatrix(subs[A11], subs[A22]), addMatrix(subs[B11], subs[B22]));       // P5 = (A11+A22)(B11+B22)
    Matrix P6 = strassen(subtractMatrix(subs[A12], subs[A22]), addMatrix(subs[B21], subs[B22]));  // P6 = (A12-A22)(B21+B22)
    Matrix P7 = strassen(subtractMatrix(subs[A11], subs[A21]), addMatrix(subs[B11], subs[B12]));  // P7 = (A11-A21)(B11+B12)

    Matrix C11 = addMatrix(P6, addMatrix(P5, subtractMatrix(P4, P2)));         // C11 = -P2 + P4 + P5 +P6
    Matrix C12 = addMatrix(P1, P2);                                            // C12 = P1 + P2
    Matrix C21 = addMatrix(P3, P4);                                            // C21 = P3 + P4
    Matrix C22 = subtractMatrix(addMatrix(P5, subtractMatrix(P1, P3)), P7);    // C22 = P1 - P3 + P5 - P7

    combine(C11, C12, C21, C22, C);

    return C;
}
