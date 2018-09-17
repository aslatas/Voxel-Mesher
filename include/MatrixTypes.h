/**
 * @file MatrixTypes.h
 * Defines convenience aliases for common Eigen datatypes.
 */

#pragma once

#include <Eigen/Geometry>
#include <Eigen/Core>

using Vec2f = Eigen::Vector2f;
using Vec3f = Eigen::Vector3f;
using Vec4f = Eigen::Vector4f;
using VecXf = Eigen::VectorXf;

using Vec2i = Eigen::Vector2i;
using Vec3i = Eigen::Vector3i;
using Vec4i = Eigen::Vector4i;
using VecXi = Eigen::VectorXi;

using Mat3f = Eigen::Matrix3f;
using Mat4f = Eigen::Matrix4f;
using MatXf = Eigen::MatrixXf;

using Quatf = Eigen::Quaternionf;