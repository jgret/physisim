#include "matrix3x3.h"

using namespace psim;

Matrix3x3::Matrix3x3(float m00, float m01, float m02, float m10, float m11, float m12, float m20, float m21, float m22)
{
    a = std::valarray{ m00, m01, m02, m10, m11, m12, m20, m21, m22 };
}

psim::Matrix3x3::Matrix3x3(const Matrix3x3& other)
{
    this->a = other.a;
}

Matrix3x3 Matrix3x3::operator*(const float f) const
{
    Matrix3x3 ret;
    ret.a = this->a * f;
    return ret;
}

void psim::Matrix3x3::operator*=(const float f)
{
    this->a = this->a * f;
}

Vector3f psim::Matrix3x3::operator*(const Vector3f& v) const
{
    return Vector3f
    {
        a[0] * v.x + a[1] * v.y + a[2] * v.z,
        a[3] * v.x + a[4] * v.y + a[5] * v.z,
        a[6] * v.x + a[7] * v.y + a[8] * v.z
    };
}

Matrix3x3 psim::Matrix3x3::operator=(const Matrix3x3& m)
{
    return Matrix3x3(m);
}

Matrix3x3 psim::operator*(const float f, const Matrix3x3& m)
{
    Matrix3x3 ret;
    ret.a = m.a * f;
    return ret;
}
