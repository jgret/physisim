#include "pch.h"
#include <math.h>

float isqrt(float f)
{
	long i;
	float x2, y;
	const float threehalfs = 1.5F;

	x2 = f * 0.5F;
	y = f;
	i = *(long*)&y;                       // evil floating point bit level hacking
	i = 0x5f3759df - (i >> 1);               // what the fuck?
	y = *(float*)&i;
	y = y * (threehalfs - (x2 * y * y));   // 1st iteration
	// y  = y * ( threehalfs - ( x2 * y * y ) );   // 2nd iteration, this can be removed

	return y;
}

TEST(TestSqrtPrecision, TestSqrt) {

	float x1 = std::sqrtf(2);
	float x2 = 1 / isqrt(2);

	EXPECT_FLOAT_EQ(x1, x2);

}
