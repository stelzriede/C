/*
 * File:   Lab4_Main.c
 * Author: Jacob Stelzriede
 *
 * Created on March 12, 2018, 11:34 PM
 */
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Libraries
#include "xc.h"
#include <stdio.h>
#include <math.h>
#include <BOARD.h>
#include <OledDriver.h>
#include <Oled.h>
#include <AD.h>
#include <MPU9250.h>
#include <I2C.h>
#include <timers.h>
#include "MatrixMath.h"

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Globals and Defines
#define BIASX 208
#define BIASY -101
#define BIASZ 58

#define AVERAGE 10

float p, q, r;

float psi = 0; //0.52359; //30 deg
float theta = 0; //1.04719; //60 deg
float phi = 0; //-.52359; //-30 deg

float psi1, psi2;
float theta1, theta2;
float phi1, phi2;

// Vectors
float nvector[3] = {1, 0, 0};
float evector[3] = {0, -1, 0};
float dvector[3] = {0, 0, -1};

float resultMat[3][3] = {};
float rExp[3][3] = {};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ User Functions
//float findAngles(float dcm);
float degToRad(float degree);
float radToDeg(float radian);
float norm(float vector[3]);
float sinc(float value);
void matExp(float input[3], float rExp[3][3]);

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ main()

int main(void) {
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Initializations
    BOARD_Init();
    OledInit();
    MPU9250_Init();
    TIMERS_Init();

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Variables
    // Gains
    float kpA = 0.2;
    float kiA = kpA * 0.1;
    float kpM = 0.2;
    float kiM = kpM * 0.1;

    // MPU and timer variables
    int aveGX, aveGY, aveGZ, aveCount;
    int initTimer, currentTime;


    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Matrices

    // Identity Matrix
    float eye[3][3] = {
        {1, 0, 0},
        {0, 1, 0},
        {0, 0, 1}
    };
    // Given DCM from Euler Angles in lab manual
    float dcm[3][3] = {
        {cos(theta) * cos(psi), cos(theta) * sin(psi), -sin(theta)},
        {sin(phi) * sin(theta) * cos(psi) - cos(phi) * sin(psi),
            sin(phi) * sin(theta) * sin(psi) + cos(phi) * cos(psi), sin(phi) * cos(theta)},
        {cos(phi) * sin(theta) * cos(phi) + sin(phi) * sin(psi),
            cos(phi) * sin(theta) * sin(psi) - sin(phi) * cos(psi), cos(phi) * cos(theta)}
    };

    // Gyro Bias and input
    float rMinus[3][3] = {
        {1, 0, 0},
        {0, 1, 0},
        {0, 0, 1}
    };
    float rPlus[3][3] = {};
    float rPlusT[3][3] = {};

    float bias[3] = {BIASX, BIASY, BIASZ};
    float gyroInput[3] = {}; //{.17453,.08726,.17453}; //10,5,10
    float accelInput[3] = {};
    float magInput[3] = {};

    //MatrixScalarMultiply(1, eye, rMinus);
    //MatrixSetEqual(eye, rMinus);

    // mag data from UCSC
    float magInert[3] = {22825 / 47835, 5393 / 47835, 41690 / 47835};
    float accelInert[3] = {0, 0, -1};

    float testVec[3] = {1, 2, 3};

    float initNVector[3] = {};
    float initEVector[3] = {};
    float initDVector[3] = {};

    //initNVector=Ro*nvector;
    MatrixVector(dcm, nvector, initNVector);
    MatrixVector(dcm, evector, initEVector);
    MatrixVector(dcm, dvector, initDVector);
    //R=Ro;  //not sure if i need this

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~test area inside main



    //closed loop
    /*
  % set mags and accels to unit vectors
  //accels = accels/norm(accels);
    float anorm = norm(accelInput);
    accelInput[0] = accelInput[0]/anorm;
    accelInput[1] = accelInput[1]/anorm;
    accelInput[2] = accelInput[2]/anorm;

  //mags = mags/norm(mags);
     float mnorm = norm(magInput);
     magInput[0] = magInput[0]/mnorm;
     magInput[1] = magInput[1]/mnorm;
     magInput[2] = magInput[2]/mnorm;

  % set inertial reference vectors to unit vectors
  //magInertial = magInertial/norm(magInertial);
    float mInorm = norm(magInertial);
    magInertial[0] = magInertial[0]/mInorm;
    magInertial[1] = magInertial[1]/mInorm;
    magInterial[2] = magInterial[2]/mInorm;

  //accelInertial = accelInertial/norm(accelInertial);
    float aInorm = norm(accelInertial);
    accelInertial[0] = accelInertial[0]/aInorm;
    accelInertial[1] = accelInertial[1]/aInorm;
    accelInertial[2] = accelInertial[2]/aInorm;

  //gyroInputWithBias = gyros - Bminus;
    gyroInput[0] = MPU9250_ReadGyroX();
    gyroInput[1] = MPU9250_ReadGyroY();
    gyroInput[2] = MPU9250_ReadGyroZ();
    // (gx+-bias/2^15+1)*250
    gyroInput[0] = ((gyroInput[0]+BIASX)*250)/32769;
    gyroInput[1] = ((gyroInput[1]+BIASY)*250)/32769;
    gyroInput[2] = ((gyroInput[2]+BIASZ)*250)/32769;

  % accelerometer correction in the body frame
  //wmeas_a = rcross(accels)*(Rminus' * accelInertial);
    float aCross[3][3] = {
        {0, -accelInput[2], accelInput[1]},
        {accelInput[2], 0, -accelInput[0]},
        {-accelInput[1],accelInput[0],0}
    };
	/*  // Wahba's Method
		W = [w';w';w'];
		B = (B_v.*W)*E_v';
		[U,S,V]=svd(B);
		R_est = (U*diag([1 1 det(U)*det(V)])*V')';

		if (nargout == 2),
		    d = det(U)*det(V);
		    s1 = S(1,1);
		    s2 = S(2,2);
		    s3 = S(3,3);
		    Pz = diag([(1-s1)/(s2-d*s3)^2 (1-s2)/(s1+d*s3)^2 (1-s3)/(s1+s2)^2]);
		    Pbody = V*Pz*V';
		end
	*/

  % magnetometer correction in the body frame
  //wmeas_m = rcross(mags) * (Rminus' * magInertial);

  //gyroInputWithFeedback = gyroInputWithBias + Kp_a*wmeas_a + Kp_m*wmeas_m;
  //bdot=-Ki_a*wmeas_a - Ki_m*wmeas_m;

  //Rplus = Rminus * Rexp(gyroInputWithFeedback)
  //Bplus = Bminus + bdot;
     */
    float avgX = 0, avgY = 0, avgZ = 0;
    float count = 0;
    int start = TIMERS_GetMilliSeconds();
    printf("Please wait 10 Seconds\r\n");
    while (1) {
        avgX += MPU9250_ReadGyroX();
        avgY += MPU9250_ReadGyroY();
        avgZ += MPU9250_ReadGyroZ();
        count ++;
        int check = TIMERS_GetMilliSeconds() - start;
        if(check >= 10000){
            break;
        }
    }
    avgX = avgX / count;
    avgY = avgY / count;
    avgZ = avgZ / count;
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ For eva eva loop
    initTimer = TIMERS_GetMilliSeconds();
    while (1) {
        currentTime = TIMERS_GetMilliSeconds();
        if ((currentTime - initTimer) > 20) {

            // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ open loop version
            gyroInput[0] = MPU9250_ReadGyroX();
            gyroInput[1] = MPU9250_ReadGyroY();
            gyroInput[2] = MPU9250_ReadGyroZ();
            // (gx+-bias/2^15+1)*250
            gyroInput[0] = (((gyroInput[0] + avgX)*250) / 32769)*0.02;
            gyroInput[1] = (((gyroInput[1] + avgY)*250) / 32769)*0.02;
            gyroInput[2] = (((gyroInput[2] + avgZ)*250) / 32769)*0.02;

            // MatrixPrint(eye);
            matExp(gyroInput, rExp);
            //MatrixPrint(rExp);
            MatrixPrint(rMinus);
            MatrixMultiply(rExp, rMinus, rPlus);
            MatrixPrint(rPlus);
            MatrixScalarMultiply(1, rPlus, rMinus); //set equal
            //MatrixScalarMultiply(1, rPlus, rMinus);
            //MatrixPrint(rPlus);

            theta1 = -asin(rExp[0][2]);
            psi1 = atan2(rExp[0][1], rExp[0][0]);
            phi1 = atan2(rExp[1][2], rExp[2][2]);

            printf("\n");
            printf("%f, ", radToDeg(theta1));
            printf("%f, ", radToDeg(phi1));
            printf("%f, ", radToDeg(psi1));
            theta1 = radToDeg(theta1);
            phi1 = radToDeg(phi1);
            psi1 = radToDeg(psi1);

            initTimer = TIMERS_GetMilliSeconds();
        }//timer loop


        // Oled Update
        char SecondLine[40];
        char ThirdLine[40];
        char ForthLine[40];
        OledDrawString(__TIME__", "__DATE__);
        sprintf(SecondLine, "\n   Roll: %.2f", (double) theta1);
        OledDrawString(SecondLine);
        sprintf(ThirdLine, "\n\n  Pitch: %.2f", (double) phi1);
        OledDrawString(ThirdLine);
        sprintf(ForthLine, "\n\n\n    Yaw: %.2f", (double) psi1);
        OledDrawString(ForthLine);
        OledUpdate();
    } // while loop
    return 0;
} // end of main
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ User Functions

float radToDeg(float radian) {
    return (180 * radian) / 3.14159;
}

float degToRad(float degree) {
    return (degree * 3.14159) / 180;
}

float norm(float vector[3]) {
    return sqrt(pow(vector[0], 2) + pow(vector[1], 2) + pow(vector[2], 2));
}

float sinc(float value) {
    float expansion = 0;
    if (value <= 0.5) {
        expansion = 1 - (pow(value, 2) / (3 * 2))+(pow(value, 4) / (5 * 4 * 3 * 2))-(pow(value, 6) / (7 * 6 * 5 * 4 * 3 * 2));
    } else {
        expansion = sin(value) / value;
    }
    return expansion;
} // end of sinc()

void matExp(float input[3], float rExp[3][3]) {
    float wnorm = norm(input);
    //printf("wnorm: %f\n", wnorm);
    // rx = rcross(gyros)
    float rx[3][3] = {
        {0, -input[2], input[1]},
        {input[2], 0, -input[0]},
        {-input[1], input[0], 0}
    };
    //MatrixPrint(rx);
    float c = cos(wnorm / 2);
    float s = sinc(wnorm / 2);

    //printf("s: %f \nc: %f\n", s, c);
    float mat1[3][3] = {
        {1, 0, 0},
        {0, 1, 0},
        {0, 0, 1}
    };
    // r_exp = [ ] + s*c*rx + s*s/2*rx*rx
    float result1[3][3] = {};
    MatrixScalarMultiply(s*c, rx, result1);
    float result2[3][3] = {};
    MatrixAdd(result1, mat1, result2);
    float result3[3][3] = {};
    MatrixMultiply(rx, rx, result3);
    float result4[3][3] = {};
    MatrixScalarMultiply(s * s / 2, result3, result4);
    MatrixAdd(result2, result4, rExp);
    // MatrixPrint(rExp);

} // end of matExp()

/* Algorithm from Handout
float findAngles(float dcm){
    if(dcm[1][3] != 1 || dcm [1][3] != -1){
        theta1 = -asin(dcm[1][3]);
        theta2 = 3.14159 - theta1;

        psi1 = atan2(dcm[1][2]/cos(theta1), dcm[1][1]/cos(theta1));
        psi2 = atan2(dcm[1][2]/cos(theta2), dcm[1][1]/cos(theta2));

        phi1 = atan2(dcm[2][3]/cos(theta1), dcm[3][3]/cos(theta1));
        phi2 = atan2(dcm[2][3]/cos(theta2), dcm[3][3]/cos(theta2));
    }else{
        phi = 0;
        if(dcm[1][3] == -1){
            theta = 3.14158/2;
            psi = phi + atan2(dcm[3][2],dcm[3][1]);
        }else{
            theta = -3.14159/2;
            psi = -phi + atan2(-dcm[3][2],-dcm[3][1]);
        }
    }
}
 */

//~~~~~~~~~~~ Notes only beyond here ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

/*
//        currentTime = TIMERS_GetMilliSeconds();
//            if((currentTime - initTimer) > 20){
//                aX = MPU9250_ReadAccelX();
//                aY = MPU9250_ReadAccelY();
//                aZ = MPU9250_ReadAccelZ();
//                gX = MPU9250_ReadGyroX();
//                gY = MPU9250_ReadGyroY();
//                gZ = MPU9250_ReadGyroZ();
//                mX = MPU9250_ReadMagX();
//                mY = MPU9250_ReadMagY();
//                mZ = MPU9250_ReadMagZ();
//                mX = 0;
//
//                if(aveCount == AVERAGE){
//                    printf("\n");
//                    printf("%d, ", (aveGX/AVERAGE)+BIASX);
//                    printf("%d, ", (aveGY/AVERAGE)+BIASY);
//                    printf("%d, ", (aveGZ/AVERAGE)+BIASZ);
//                    aveCount = 0;
//                    aveGX = 0;
//                    aveGY = 0;
//                    aveGZ = 0;
//                }
//            }//timer loop
 */

//                //float accelInput[3]={};
//                accelInput[0] = MPU9250_ReadAccelX();
//                accelInput[1] = MPU9250_ReadAccelY();
//                accelInput[2] = MPU9250_ReadAccelZ();
//
//                //float magInput[3]={};
//                magInput[0] = MPU9250_ReadMagX();
//                magInput[1] = MPU9250_ReadMagY();
//                magInput[2] = MPU9250_ReadMagZ();
/*
    // Rotation Matrices
    float rotX[3][3] = {
        {1, 0, 0},
        {0, cos(psi), -sin(psi)},
        {0, sin(psi), cos(psi)}
    };
    float rotY[3][3] = {
        {cos(theta), 0, sin(theta)},
        {0, 1, 0},
        {-sin(theta), 0, cos(theta)}
    };
    float rotZ[3][3] = {
        {cos(phi), -sin(phi), 0},
        {sin(phi), cos(phi), 0},
        {0, 0, 1}
    };
 */

/*
    float dcm[3][3] = {
        {cos(theta)*cos(psi), cos(theta)*sin(psi), -sin(theta)},
        {sin(phi)*sin(theta)*cos(psi)-cos(phi)*sin(psi),
         sin(phi)*sin(theta)*sin(psi)+cos(phi)*cos(psi), sin(phi)*cos(theta)},
        {cos(phi)*sin(theta)*cos(phi)+sin(phi)*sin(psi),
         cos(phi)*sin(theta)*sin(psi)-sin(phi)*cos(psi), cos(phi)*cos(theta)}
    };
 */
/*
                if(rPlus[1][3] != 1 || rPlus[1][3] != -1){
                        theta1 = -asin(rPlus[1][3]);
                        theta2 = 3.14159 - theta1;

                        psi1 = atan2(rPlus[1][2]/cos(theta1), rPlus[1][1]/cos(theta1));
                        psi2 = atan2(rPlus[1][2]/cos(theta2), rPlus[1][1]/cos(theta2));

                        phi1 = atan2(rPlus[2][3]/cos(theta1), rPlus[3][3]/cos(theta1));
                        phi2 = atan2(rPlus[2][3]/cos(theta2), rPlus[3][3]/cos(theta2));
                }else{
                    phi1 = 0;
                    if(rPlus[1][3] == -1){
                        theta1 = 3.14158/2;
                        psi1 = phi1 + atan2(rPlus[3][2],rPlus[3][1]);
                    }else{
                        theta1 = -3.14159/2;
                        psi1 = -phi1 + atan2(-rPlus[3][2],-rPlus[3][1]);
                    }
                }
 */