#ifndef COMMON_H_
#define COMMON_H_

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/ioctl.h>
#include <unistd.h>

#define PI 3.141592653589793238462643383279502884197169399375105
#define E 2.71828182845904523536028747135266249775724709369995

#define RAND_DOUBLE (((rand() / (double)RAND_MAX) * 2) - 1)
#define DEG_TO_RAD(x) ((x) * (PI / 180.0))
#define RAD_TO_DED(x) ((x) * 180.0 / PI)

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

#endif
