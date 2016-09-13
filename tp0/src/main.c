#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <memory.h>
#include <math.h>

void printError(char *message);

int checkImaginaryNumber(char *argumentValue);

int checkNumber(char *argumentValue);

/**
 * Devuelvo -1 si no se pudo escribir el archivo correctamente, 0 en otro caso.
 */
void createPGM(int cols, int rows, int maxVal, FILE *fp, int matrix[rows][cols]) {
    fprintf(fp, "%s\n", "P2");
    fprintf(fp, "%d ", cols);
    fprintf(fp, "%d\n", rows);
    fprintf(fp, "%d\n", maxVal);
    int j = 0;
    for (; j < cols; j++) {
        int i = 0;
        for (; i < rows; i++) {
            fprintf(fp, "%d", matrix[i][j]);
            //Avoid add an space after last column
            if (i != rows - 1) {
                fputs(" ", fp);
            }
        }
        fputs("\n", fp);
    }

    fclose(fp);
}

float module(float x, float y) {
    return sqrt(x * x + y * y);
}

float calculateX(float x, float y, float c) {
    return x * x - y * y + c;
}

float calculateY(float x, float y, float c) {
    return 2 * x * y + c;
}

int** doCalculo(float xCenter, float yCenter, float height, float width, float resWidth, float resHeight, float cX, float cY, int iterations) {

    float xArray[(int)resWidth];
    float yArray[(int)resHeight];

    float coeficientWidth = width / resWidth;
    float coeficientHeight = height / resHeight;

    float x = xCenter - width / 2;
    float y = yCenter - height / 2;

    int i = 0;
    while(x < xCenter + width / 2) {
        xArray[i] = x + coeficientWidth / 2;
        x += coeficientWidth;
        i++;
    }
    i = 0;
    while(y < yCenter + height / 2) {
        yArray[i] = y + coeficientHeight / 2;
        y += coeficientHeight;
        i++;
    }

    int **output = malloc(resWidth * sizeof(int*)); //[(int)resWidth][(int)resHeight];
    for (i = 0; i < resWidth; i++) {
        output[i] = malloc(resHeight * sizeof(int));
    }

    for (i = 0; i< resWidth; i++) {
        int j = 0;
        for (; j< resHeight; j++) {

            float zReal = xArray[i];
            float zIm = yArray[j];

            int k = 0;
            for (; j < iterations; k++) {
                if (module(zReal, zIm) > 2) {
                    break;
                }
                float aux = calculateX(zReal, zIm, cX);
                zIm = calculateY(zReal, zIm, cY);
                zReal = aux;
            }
            output[i][j] = k;
        }
    }
    printf("salio\n");
    return output;
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
        int i = 0;
        for (; i < length; i++) {
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


    FILE *fp;

    /* ouptut value */
    if (output != NULL) {
        fp = fopen(output, "w");
        if (fp == NULL) {
            printError("fatal: cannot open output file.");
            exit(EXIT_FAILURE);
        }
    } else {//stdout
        fp = stdout;
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


    int **matrix;

    matrix = doCalculo(centerRe, centerIm, rectangleHeight, rectangleWidth, resolutionWidth, resolutionHeight, cRe, cIm, 256);

    createPGM(resolutionWidth, resolutionHeight, 255, fp, matrix);

   // fclose(fp);

    free(matrix);

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
    int i = length - 1;
    for (; i >= 0; i--) {
        if (isdigit(argumentValue[i])) {
            continue;
        } else if (argumentValue[i] == '.' && point == 0) {
            point = i;
            if (!(isdigit(argumentValue[i + 1]) && isdigit(argumentValue[i - 1]))) {
                return -1;
            }
        } else if ((argumentValue[i] == '+' || argumentValue[i] == '-') && amountOfSigns == 0) {
            amountOfSigns++;
            point = 0;
            offset = i;
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
    size_t length = strlen(argumentValue);
    unsigned int i = 0;
    for (; i < length; i++) {
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
