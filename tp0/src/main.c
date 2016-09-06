#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <memory.h>
#include "../pgmFactory.h"

void printError(char *message);
int checkImaginaryNumber(char *argumentValue);
int checkNumber(char *argumentValue);

int main(int argc, char **argv) {

    /**************************  Default variables  **********************************/
    int resolutionWidth = 640;
    int resolutionHeight = 480;

    float centerRe = 0;
    float centerIm = 0;

    float cRe = 0.285;
    float cIm = 0.01;

    float rectangleWidth = 4;
    float rectangleHeigth = 4;

    char *output = NULL;
    /************************ input parameters **********************************/
    int parameter;
    int offsetResolution = 0;

    char *resolutionValue = NULL;
    char *centerValue = NULL;
    char *cValue = NULL;
    char *rectangleWidthValue = NULL;
    char *rectangleHeigthValue = NULL;
    char *outputValue = NULL;
    opterr = 0;

    while ((parameter = getopt (argc, argv, "r:c:C:w:H:o:")) != -1)
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
                rectangleHeigthValue = optarg;
                break;
            case 'o':
                outputValue = optarg;
                if (outputValue[0] == '-' && strlen(outputValue) == 1) {
                    break;
                }
                output = outputValue;
                break;
            case '?':
                if (optopt == 'r' || optopt == 'c' || optopt == 'C' || optopt == 'w' || optopt == 'H' || optopt == 'o')
                    fprintf (stderr, "Option -%c requires an argument.\n", optopt);
                else if (isprint (optopt))
                    fprintf (stderr, "Unknown argument `-%c'.\n", optopt);
                else
                    fprintf (stderr,
                             "Unknown option character `\\x%x'.\n",
                             optopt);
                return 1;
            default:
                abort ();
        }

    /* resolution values */
    if (resolutionValue != NULL) {
        size_t length = strlen(resolutionValue);
        for(int i = 0; i < length; i++) {
            if (isdigit(resolutionValue[i])) {
                continue;
            } else if (resolutionValue[i] == 'x' && offsetResolution == 0) {
                offsetResolution = i;
            } else {
                printError("fatal: invalid resolution specification.");
                return 1;
            }
        }
        if (offsetResolution == 0 || offsetResolution == length -1) {
            printError("fatal: invalid resolution specification.");
            return 1;
        }
        char rWidth[offsetResolution];
        strncpy(rWidth, resolutionValue, offsetResolution);
        resolutionWidth = atoi(rWidth);
        char rHeigth[length - offsetResolution - 1];
        strncpy(rHeigth, resolutionValue + offsetResolution + 1, length);
        resolutionHeight = atoi(rHeigth);
    }

    /* rectangle width value */
    if (rectangleWidthValue != NULL) {
        int isValid = checkNumber(rectangleWidthValue);
        if (isValid == 0) {
            float w = atof(rectangleWidthValue);
            rectangleWidth = w;
        } else {
            printError("fatal: invalid width rectangle specification.");
            return 1;
        }
    }

    /* rectangle heigth value */
    if (rectangleHeigthValue != NULL) {
        int isValid = checkNumber(rectangleHeigthValue);
        if (isValid == 0) {
            float h = atof(rectangleHeigthValue);
            rectangleHeigth = h;
        } else {
            printError("fatal: invalid heigth rectangle specification.");
            return 1;
        }

    }

    /* ouptut value */
    if (output != NULL) {
        FILE *fp;
        fp = fopen(output, "w");
        if (fp != NULL) {
            fclose(fp);
        } else {
            printError("fatal: cannot open output file.");
            return 1;
        }
    }

    /* c value */
    if (cValue != NULL) {
        int offset = checkImaginaryNumber(cValue);
        if (offset == -1) {
            printError("fatal: invalid C value argument.");
            return 1;
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
            return 1;
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
    printf("rectangle heigth= %f \n", rectangleHeigth);
    printf("center real= %f \n", centerRe);
    printf("center im= %f \n", centerIm);
    printf("c real= %f \n", cRe);
    printf("c im= %f \n", cIm);
    printf("output= %s \n", output);


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

void printError(char *message) { fprintf(stderr, "%s\n", message); }

/* funcion que devuelve -1 si el parametro tiene un error y el offset donde se divide el numero complejo entre real
 *                  e imaginario en caso de estar bien              */
int checkImaginaryNumber(char *argumentValue) {
    size_t length = strlen(argumentValue) - 1;
    if (argumentValue[length] != 'i') {
       return -1;
    }
    int point = 0;
    int cantSignos = 0;
    int offset = 0;
    for (int i = length - 1; i >= 0; i--) {
        if (isdigit(argumentValue[i])) {
            continue;
        } else if (argumentValue[i] == '.' && point == 0) {
            point = i;
            if (!(isdigit(argumentValue[i+1]) && isdigit(argumentValue[i-1]))) {
                return -1;
            }
        } else if ((argumentValue[i] == '+' || argumentValue[i] == '-') && cantSignos == 0) {
            cantSignos++;
            point = 0;
            offset = i;
        } else if ((argumentValue[i] == '+' || argumentValue[i] == '-') && cantSignos == 1) {
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
    for (int i = 0; i < strlen(argumentValue); i++) {
        if(isdigit(argumentValue[i])) {
            continue;
        } else if (argumentValue[i] == '.' && point == 0) {
            point = i;
            if (!(isdigit(argumentValue[i+1]) && isdigit(argumentValue[i-1]))) {
                return -1;
            }
        } else {
            return -1;
        }
    }
    return 0;
}