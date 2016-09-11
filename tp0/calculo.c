#include <string.h>
#include <stdlib.h>

void doCalculo(char *center, int height, int width, char *res) {
    char *imaginary;
    imaginary = strtok(center, "+");
    strncpy(imaginary, imaginary, strlen(imaginary) -2);
    //imaginary[strlen(imaginary)] = "/0";
    float xCenter = atof(center);
    float yCenter = atof(imaginary);

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
    while(x < xCenter + width / 2) {

    }

}
