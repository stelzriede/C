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

// **** Define global, module-level, or external variables here ****
float random[3][3] = {
    {8, 6, 7},
    {5, 3, 0},
    {9, 9, 9}
};

float mat1[3][3] = {
    {0, 1, 2},
    {3, 4, 5},
    {6, 7, 8}
};
float mat2[3][3] = {
    {0, 1, 2},
    {3, 4, 5},
    {6, 7, 8}
};
float retResult[3][3] = {
    {0}
};

//                  ==* Start Main *==
int main()
{
    BOARD_Init();

    /******************************************************************************
     * Your code goes in between this comment and the following one with asterisks.
     *****************************************************************************/

    // MatrixEquals Test **************************************************************

    if (MatrixEquals(mat1, mat2) == 1)
        printf("MatrixEquals Test1: Passed\n");
    else
        printf("MatrixEquals Test1: Failed\n");

    if (MatrixEquals(mat1, random) == 0)
        printf("MatrixEquals Test2: Passed\n");
    else
        printf("MatrixEquals Test2: Failed\n");

    // MatrixAdd Test *****************************************************************

    float expected1[3][3] = {
        {0, 2, 4},
        {6, 8, 10},
        {12, 14, 16}
    };
    MatrixAdd(mat1, mat2, retResult);

    if (MatrixEquals(retResult, expected1) == 1)
        printf("MatrixAdd Test1: Passed\n");
    else
        printf("MatrixAdd Test1: Failed\n");

    if (MatrixEquals(retResult, random) == 0)
        printf("MatrixAdd Test2: Passed\n");
    else
        printf("MatrixAdd Test2: Failed\n");

    // MatrixScalarAdd Test ***********************************************************

    float expected2[3][3] = {
        {5, 6, 7},
        {8, 9, 10},
        {11, 12, 13}
    };
    MatrixScalarAdd(5, mat1, retResult);

    if (MatrixEquals(retResult, expected2) == 1)
        printf("MatrixScalarAdd Test1: Passed\n");
    else
        printf("MatrixScalarAdd Test1: Failed\n");

    if (MatrixEquals(retResult, random) == 0)
        printf("MatrixScalarAdd Test2: Passed\n");
    else
        printf("MatrixScalarAdd Test2: Failed\n");

    // MatrixMultiply Test *************************************************************

    float expected3[3][3] = {
        {15, 18, 21},
        {42, 54, 66},
        {69, 90, 111}
    };
    MatrixMultiply(mat1, mat2, retResult);

    if (MatrixEquals(retResult, expected3) == 1)
        printf("MatrixMultiply Test1: Passed\n");
    else
        printf("MatrixMultiply Test1: Failed\n");

    if (MatrixEquals(retResult, random) == 0)
        printf("MatrixMultiply Test2: Passed\n");
    else
        printf("MatrixMultiply Test2: Failed\n");

    // MatrixScalarMultiply Test ******************************************************

    //float expected1[3][3] = {{0,2,4},{6,8,10},{12,14,16}};
    MatrixScalarMultiply(2, mat2, retResult);

    if (MatrixEquals(retResult, expected1) == 1)
        printf("MatrixScalarMultiply Test1: Passed\n");
    else
        printf("MatrixScalarMultiply Test1: Failed\n");

    if (MatrixEquals(retResult, random) == 0)
        printf("MatrixScalarMultiply Test2: Passed\n");
    else
        printf("MatrixScalarMultiply Test2: Failed\n");


    // MatrixTrace Test ***************************************************************

    if (MatrixTrace(mat1) == 12)
        printf("MatrixTrace Test1: Passed\n");
    else
        printf("MatrixTrace Test1: Failed\n");
    if (MatrixTrace(mat1) == 99)
        printf("MatrixTrace Test1: Failed\n");
    else
        printf("MatrixTrace Test1: Passed\n");

    // MatrixTranspose Test ***********************************************************

    float expected4[3][3] = {
        {0, 3, 6},
        {1, 4, 7},
        {2, 5, 8}
    };
    MatrixTranspose(mat1, retResult);

    if (MatrixEquals(retResult, expected4) == 1)
        printf("MatrixTranspose Test1: Passed\n");
    else
        printf("MatrixTranspose Test1: Failed\n");

    if (MatrixEquals(retResult, random) == 0)
        printf("MatrixTranspose Test2: Passed\n");
    else
        printf("MatrixTranspose Test2: Failed\n");

    // MatrixDeterminant Test **********************************************************

    if (MatrixDeterminant(random) == 72)
        printf("MatrixDeterminant Test1: Passed\n");
    else
        printf("MatrixDeterminant Test1: Failed\n");

    if (MatrixDeterminant(random) == 55555)
        printf("MatrixDeterminant Test1: Failed\n");
    else
        printf("MatrixDeterminant Test1: Passed\n");

    // MatrixInverse Text **************************************************************

    float expected5[3][3] = {
        {.375, .125, -.2916666667},
        {-.625, .125, .4861111111},
        {.25, -.25, -.083333333}
    };
    MatrixInverse(random, retResult);
    if (MatrixEquals(retResult, expected5) == 1)
        printf("MatrixInverse Test1: Passed\n");
    else
        printf("MatrixInverse Test1: Failed\n");

    if (MatrixEquals(retResult, expected5) == 0)
        printf("MatrixInverse Test2: Failed\n");
    else
        printf("MatrixInverse Test2: Passed\n");

    //MatrixPrint(retResult);

     // MatrixPrint Example *************************************************************

    printf(" Sample Matrix Hardcored:\n");
    printf("| 0.0 || 1.0 || 2.0 |\n");
    printf("| 3.0 || 4.0 || 5.0 |\n");
    printf("| 6.0 || 7.0 || 8.0 |\n");

    printf(" Sample Matrix From MatrixPrint():\n");
    MatrixPrint(mat2);

    /******************************************************************************
     * Your code goes in between this comment and the preceding one with asterisks
     *****************************************************************************/

    // Returning from main() is bad form in embedded environments. So we sit and spin.
    while (1);
}