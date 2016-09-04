#include <stdio.h>
#include <stdlib.h>
#include "../pgmFactory.h"

int main(int argc, char **argv) {
    int rows = 3;
    int cols = 5;

    int matrix[rows][cols];


    int count = 0;

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrix[i][j] = ++count;
        }
    }

    createPGM(5, 3, 255, "prueba.pgm", matrix);
    return 0;
}