//
// Created by federicofarina on 9/3/16.
//

#ifndef TP0_PGMFACTORY_H
#define TP0_PGMFACTORY_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

bool createPGM(int cols, int rows, int maxVal, char *name, int matrix[rows][cols]) {


    FILE *fp;
    fp = fopen(name, "w");

    if (fp != NULL) {
        fputs("P2", fp);
        fprintf(fp, "\n# %s \n", name);
        fprintf(fp, "%d ", cols);
        fprintf(fp, "%d\n", rows);
        fprintf(fp, "%d\n", maxVal);

        for (int j = 0; j < cols; j++) {
            for (int i = 0; i < rows; i++) {
                fprintf(fp, "%d", matrix[i][j]);
                //Avoid add an space after last column
                if (i != rows - 1) {
                    fputs(" ", fp);
                }
            }
            fputs("\n", fp);
        }

        fclose(fp);
    } else {
        return false;
    }

    return true;
}

#endif //TP0_PGMFACTORY_H
