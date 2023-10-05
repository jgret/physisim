#pragma once

#define REAL_DOUBLE
#define REAL_FLOAT

#ifdef REAL_FLOAT
typedef float real;
#elif USE_DOUBLE
typedef double real;
#else
#error "Either REAL_DOUBLE or REAL_FLOAT must be defined"
#endif