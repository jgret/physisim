#pragma once

#include <valarray>
#include "vector3f.h"

namespace psim
{

    class Matrix3x3
    {

    public:

        // Matrix layout
        // m00 m01 m02
        // m10 m11 m12
        // m20 m21 m22

        //  0   1   2
        //  3   4   5
        //  6   7   8

        std::valarray<float> a;

        Matrix3x3(float m00 = 0, float m01 = 0, float m02 = 0,
            float m10 = 0, float m11 = 0, float m12 = 0,
            float m20 = 0, float m21 = 0, float m22 = 0);

        Matrix3x3(const Matrix3x3& m);

        Matrix3x3 operator*(const float f) const;
        void operator*=(const float f);
        Vector3f operator*(const Vector3f& v) const;
        Matrix3x3 operator=(const Matrix3x3& m);

    };

    Matrix3x3 operator*(const float f, const Matrix3x3& m);

}

