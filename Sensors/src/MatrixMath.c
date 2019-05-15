// **** Include libraries here ****
// Standard libraries
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//CMPE13 Support Library
#include "BOARD.h"

// Microchip libraries
#include <xc.h>
#include <plib.h>

// User libraries
#include "MatrixMath.h"

float result[3][3] = {};
float vector[3] = {};
float x;
float determinant;

/******************************************************************************
 * Your code goes in between this comment and the following one with asterisks.
 *****************************************************************************/
void MatrixSetEqual(float mat1[3][3], float result[3][3])
{
    int i, j;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            result[i][j] = mat1[i][j];
        }
    }
}

void MatrixVector(float mat[3][3], float vec[3], float vector[3])
{
    int i, j, product = 0;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            product += mat[i][j] * vec[j];
        };
        vector[i] = product;
        product = 0;
    }
}

// basic loop from lab manual, loop allows proper matrix members
// to be multiplied then added to the product.

void MatrixMultiply(float mat1[3][3], float mat2[3][3], float result[3][3])
{
    int i, j, k, product = 0;
    for (i = 0; i < 3; i++) {
        for (k = 0; k < 3; k++) {
            for (j = 0; j < 3; j++) {
                product += mat1[i][j] * mat2[j][k];
            };
            result[i][k] = product;
            product = 0;
        }
    }
}
// **************************************************************************************
// basic loop from lab manual, loop allows proper matrix members to be added in result.

void MatrixAdd(float mat1[3][3], float mat2[3][3], float result[3][3])
{
    int i, j;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            result[i][j] = mat1[i][j] + mat2[i][j];
        }
    }
}
//****************************************************************************************
// important for testing, this checks that each matrix is the same size

int MatrixEquals(float mat1[3][3], float mat2[3][3])
{
    int i, j;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            if (mat1[i][j] == mat2[i][j])
                continue;
            else
                return 0;
        }
    }
    return 1;
}
//****************************************************************************************
// basic loop from lab manual, this multiplies the matrix members by a scalar quantity.

void MatrixScalarMultiply(float x, float mat1[3][3], float result[3][3])
{
    int i, j;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            result[i][j] = mat1[i][j] * x;
        }
    }
}
//****************************************************************************************
// basic loop from lab manual, this adds the matrix members by a scalar quantity.

void MatrixScalarAdd(float x, float mat1[3][3], float result[3][3])
{
    int i, j;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            result[i][j] = mat1[i][j] + x;
        }
    }
}
//******************************************************************************************
// because of the combinations, it was easier to hard code a 3x3 determinant combinations.
// the actually formula uses the the determinant as the return variable.

float MatrixDeterminant(float mat1[3][3])
{
    float a = mat1[0][0];
    float b = mat1[0][1];
    float c = mat1[0][2];
    float d = mat1[1][0];
    float e = mat1[1][1];
    float f = mat1[1][2];
    float g = mat1[2][0];
    float h = mat1[2][1];
    float i = mat1[2][2];

    determinant = (a * e * i)+(b * f * g)+(c * d * h)-(c * e * g)-(b * d * i)-(a * f * h);

    return determinant;
}
//******************************************************************************************
// loop allows a diagonal sum to be stored into sum, the returns sum.

float MatrixTrace(float mat1[3][3])
{
    int i, j;
    float sum = 0;
    for (i = 0; i < 3; i++) {
        j = i;
        sum += mat1[i][j];
    }
    return sum;
}
//******************************************************************************************
// loop allows mirror of diagonal elements, returns result matrix

void MatrixTranspose(float mat1[3][3], float result[3][3])
{
    int i, j;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            result[j][i] = mat1[i][j];
        }
    }
}
//*****************************************************************************************
// same as determinant, hard coded the element combinations to make the formula easier to
// test and trouble shoot.

void MatrixInverse(float mat1[3][3], float result[3][3])
{
    float a = mat1[0][0];
    float b = mat1[0][1];
    float c = mat1[0][2];
    float d = mat1[1][0];
    float e = mat1[1][1];
    float f = mat1[1][2];
    float g = mat1[2][0];
    float h = mat1[2][1];
    float k = mat1[2][2];

    determinant = (a * e * k)+(b * f * g)+(c * d * h)-(c * e * g)-(b * d * k)-(a * f * h);

    float inverseMath[3][3] = {
        {(e * k)-(f * h), (c * h)-(b * k), (b * f)-(c * e)},
        {(f * g)-(d * k), (a * k)-(c * g), (c * d)-(a * f)},
        {(d * h)-(e * g), (b * g)-(a * h), (a * e)-(b * d)}
    };

    MatrixScalarMultiply((1/determinant), inverseMath, result);

}
//*****************************************************************************************
// loop allows printing of each element of the matrix.

void MatrixPrint(float mat1[3][3])
{
    int i, j;
    printf("---------------\n");
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            printf("| %10.5f |", (double) mat1[i][j]);
        }
        printf("\n");
    }
}
