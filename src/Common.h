#ifndef COMMON_H_
#define COMMON_H_

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/ioctl.h>
#include <unistd.h>

#define PI 3.14159265

#define RAND_DOUBLE (((rand() / (double)RAND_MAX) * 2) - 1)
#define DEG_TO_RAD(x) ((x) * (PI / (double)180))

#endif
