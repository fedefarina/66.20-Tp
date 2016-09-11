#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <memory.h>

void printError(char *message);

int checkImaginaryNumber(char *argumentValue);

int checkNumber(char *argumentValue);

/**
 * Devuelvo -1 si no se pudo escribir el archivo correctamente
 */
int createPGM(int cols, int rows, int maxVal, char *name, int matrix[rows][cols]) {

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
        return -1;
    }

    return 0;
}

int main(int argc, char **argv) {

    /**************************  Default variables  **********************************/
    int resolutionWidth = 640;
    int resolutionHeight = 480;

    double centerRe = 0;
    double centerIm = 0;

    double cRe = 0.285;
    double cIm = 0.01;

    double rectangleWidth = 4;
    double rectangleHeight = 4;

    char *output = NULL;
    /************************ input parameters **********************************/
    int parameter;
    int offsetResolution = 0;

    char *resolutionValue = NULL;
    char *centerValue = NULL;
    char *cValue = NULL;
    char *rectangleWidthValue = NULL;
    char *rectangleHeightValue = NULL;
    char *outputValue = NULL;
    opterr = 0;

    while ((parameter = getopt(argc, argv, "r:c:C:w:H:o:")) != -1)
        switch (parameter) {
            case 'r':
                resolutionValue = optarg;
                break;
            case 'c':
                centerValue = optarg;
                break;
            case 'C':
                cValue = optarg;
                break;
            case 'w':
                rectangleWidthValue = optarg;
                break;
            case 'H':
                rectangleHeightValue = optarg;
                break;
            case 'o':
                outputValue = optarg;
                if (outputValue[0] == '-' && strlen(outputValue) == 1) {
                    break;
                }
                output = outputValue;
                break;
            case '?':
                if (optopt == 'r' || optopt == 'c' || optopt == 'C' || optopt == 'w' || optopt == 'H' ||
                    optopt == 'o') {
                    fprintf(stderr, "Option -%c requires an argument.\n", optopt);
                } else if (isprint (optopt)) {
                    fprintf(stderr, "Unknown argument `-%c'.\n", optopt);
                } else {
                    fprintf(stderr, "Unknown option character `\\x%x'.\n", optopt);

                }
                exit(EXIT_FAILURE);
            default:
                exit(EXIT_FAILURE);
        }

    /* resolution values */
    if (resolutionValue != NULL) {
        size_t length = strlen(resolutionValue);
        for (int i = 0; i < length; i++) {
            if (isdigit(resolutionValue[i])) {
                continue;
            } else if (resolutionValue[i] == 'x' && offsetResolution == 0) {
                offsetResolution = i;
            } else {
                printError("fatal: invalid resolution specification.");
                exit(EXIT_FAILURE);
            }
        }
        if (offsetResolution == 0 || offsetResolution == length - 1) {
            printError("fatal: invalid resolution specification.");
            return 1;
        }
        char rWidth[offsetResolution];
        strncpy(rWidth, resolutionValue, offsetResolution);
        resolutionWidth = atoi(rWidth);
        char rHeight[length - offsetResolution - 1];
        strncpy(rHeight, resolutionValue + offsetResolution + 1, length);
        resolutionHeight = atoi(rHeight);
    }

    /* rectangle width value */
    if (rectangleWidthValue != NULL) {
        int isValid = checkNumber(rectangleWidthValue);
        if (isValid == 0) {
            double w = atof(rectangleWidthValue);
            rectangleWidth = w;
        } else {
            printError("fatal: invalid width rectangle specification.");
            exit(EXIT_FAILURE);
        }
    }

    /* rectangle heigth value */
    if (rectangleHeightValue != NULL) {
        int isValid = checkNumber(rectangleHeightValue);
        if (isValid == 0) {
            double h = atof(rectangleHeightValue);
            rectangleHeight = h;
        } else {
            printError("fatal: invalid heigth rectangle specification.");
            exit(EXIT_FAILURE);
        }
    }

    /* ouptut value */
    if (output != NULL) {
        FILE *fp = fopen(output, "w");
        if (fp != NULL) {
            fclose(fp);
        } else {
            printError("fatal: cannot open output file.");
            exit(EXIT_FAILURE);
        }
    }


    /* c value */
    if (cValue != NULL) {
        int offset = checkImaginaryNumber(cValue);
        if (offset == -1) {
            printError("fatal: invalid C value argument.");
            exit(EXIT_FAILURE);
        }
        char cAuxReal[offset];
        strncpy(cAuxReal, cValue, offset);
        cRe = atof(cAuxReal);
        size_t length = strlen(cValue);
        char cAuxIm[length - offset - 1];
        strncpy(cAuxIm, cValue + offset, length);
        cIm = atof(cAuxIm);
    }

    /* center value */
    if (centerValue != NULL) {
        int offset = checkImaginaryNumber(centerValue);
        if (offset == -1) {
            printError("fatal: invalid c value argument.");
            exit(EXIT_FAILURE);
        }
        char centerAuxReal[offset];
        strncpy(centerAuxReal, centerValue, offset);
        centerRe = atof(centerAuxReal);
        size_t length = strlen(centerValue);
        char centerAuxIm[length - offset - 1];
        strncpy(centerAuxIm, centerValue + offset, length);
        centerIm = atof(centerAuxIm);
    }

    /* temporal para ver los valores que quedan */
    printf("resolution width= %d \n", resolutionWidth);
    printf("resolution heigth= %d \n", resolutionHeight);
    printf("rectangle width= %f \n", rectangleWidth);
    printf("rectangle heigth= %f \n", rectangleHeight);
    printf("center real= %f \n", centerRe);
    printf("center im= %f \n", centerIm);
    printf("c real= %f \n", cRe);
    printf("c im= %f \n", cIm);
    printf("output= %s \n", output);



    //fixme sacar esto cuando esté el cálculo de la matriz
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

    return EXIT_SUCCESS;
}


void printError(char *message) { fprintf(stderr, "%s\n", message); }

/* funcion que devuelve -1 si el parametro tiene un error y el offset donde se divide el numero complejo entre real
 *                  e imaginario en caso de estar bien              */
int checkImaginaryNumber(char *argumentValue) {
    size_t length = strlen(argumentValue) - 1;
    if (argumentValue[length] != 'i') {
        return -1;
    }
    int point = 0;
    int amountOfSigns = 0;
    int offset = 0;
    for (size_t i = length - 1; i >= 0; i--) {
        if (isdigit(argumentValue[i])) {
            continue;
        } else if (argumentValue[i] == '.' && point == 0) {
            point = (int) i;
            if (!(isdigit(argumentValue[i + 1]) && isdigit(argumentValue[i - 1]))) {
                return -1;
            }
        } else if ((argumentValue[i] == '+' || argumentValue[i] == '-') && amountOfSigns == 0) {
            amountOfSigns++;
            point = 0;
            offset = (int) i;
        } else if ((argumentValue[i] == '+' || argumentValue[i] == '-') && amountOfSigns == 1) {
            if (i != 0) {
                return -1;
            }
        } else {
            return -1;
        }
    }
    return offset;
}

/* funcion que devuelve -1 si el parametro tiene un error y 0 si es un argumento correcto */
int checkNumber(char *argumentValue) {
    int point = 0;
    for (unsigned int i = 0; i < strlen(argumentValue); i++) {
        if (isdigit(argumentValue[i])) {
            continue;
        } else if (argumentValue[i] == '.' && point == 0) {
            point = i;
            if (!(isdigit(argumentValue[i + 1]) && isdigit(argumentValue[i - 1]))) {
                return -1;
            }
        } else {
            return -1;
        }
    }
    return 0;
}