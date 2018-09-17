/**
 * @headerfile MatrixPrinting.h ""
 * Defines utility functions which Google Test can use to print Eigen matrices
 * in failure output instead of byte data.
 * @author Matthew McLaurin
 */

#pragma once

#include "MatrixTypes.h"

namespace Eigen
{
void PrintTo(const Vec2i vec, std::ostream* os)
{
    *os << "(" << vec.x() << ", " << vec.y() << ")\n";
}

void PrintTo(const Vec3i vec, std::ostream* os)
{
    *os << "(" << vec.x() << ", " << vec.y() << ", " << vec.z() << ")\n";
}

void PrintTo(const Vec4i vec, std::ostream* os)
{
    *os << "(" << vec.x() << ", " << vec.y() << ", " << vec.z() << ", "
        << vec.w() << ")\n";
}

void PrintTo(const Vec2f vec, std::ostream* os)
{
    *os << "(" << vec.x() << ", " << vec.y() << ")\n";
}

void PrintTo(const Vec3f vec, std::ostream* os)
{
    *os << "(" << vec.x() << ", " << vec.y() << ", " << vec.z() << ")\n";
}

void PrintTo(const Vec4f vec, std::ostream* os)
{
    *os << "(" << vec.x() << ", " << vec.y() << ", " << vec.z() << ", "
        << vec.w() << ")\n";
}

void PrintTo(const Mat3f& mat, std::ostream* os)
{
    *os << std::endl << mat << std::endl;
}

void PrintTo(const Mat4f& mat, std::ostream* os)
{
    *os << std::endl << mat << std::endl;
}

void PrintTo(const MatXf& mat, std::ostream* os)
{
    *os << std::endl << mat << std::endl;
}

void PrintTo(const MatXi& mat, std::ostream* os)
{
    *os << std::endl << mat << std::endl;
}

void PrintTo(const Quatf& quat, std::ostream* os)
{
    *os << std::endl << quat.toRotationMatrix() << std::endl;
}
}