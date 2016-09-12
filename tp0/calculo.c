#include <string.h>
#include <stdlib.h>

float module(float x, float y) {
    return sqrt(x * x + y * y);
}

float calculateX(float x, float y, float c) {
    return x * x - y * y + c;
}

float calculateY(float x, float y, float c) {
    return 2 * x * y + c;
}

int* doCalculo(float xCenter, flaot yCenter, int height, int width, char *res, float cX, float cY, int iterations) {

    char *resWidthStr;
    resWidthStr = strtok(res, "x");
    int resWidth = atoi(resWidthStr);
    int resHeight = atoi(res);

    float xArray[resWidth];
    float yArray[resHeight];

    float coeficientWidth = width / resWidth;
    float coeficientHeight = height / resHeight;

    float x = xCenter - width / 2;
    float y = yCenter - height / 2;

    int i = 0;
    while(x < xCenter + width / 2) {
        xArray[i] = x + coeficientWidth / 2;
        x += coeficientWidth;
    }
    i = 0;
    while(y < yCenter + height / 2) {
        yArray[i] = y + coeficientHeight / 2;
        y += coeficientHeight;
    }

    int output[resWidth][resHeight];

    for (i = 0; i< resWidth; i++) {
        for (int j = 0; j< resHeight; j++) {
            float zReal = xArray[i];
            float zIm = yArray[j];
            int k = 0;
            for (k; j < iterations; k++) {
                if (module(zReal, zIm) > 2) {
                    break;
                }
                float aux = calculateX(zReal, zIm, cX);
                zIm = calculateY(zReal, zIm, cY);
                zReal = aux;
                zReal = aux;
            }
            output[i][j] = k;
        }
    }
    return output;
}
