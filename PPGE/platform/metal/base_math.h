#pragma once

#include <simd/simd.h>

namespace Metal
{
namespace SimpleMath
{

using float4 = simd_float4;
using Color = simd_float4;
using Vector2 = simd_float2;
using Vector3 = simd_float3;
using Vector4 = simd_float4;
using Quaternion = simd_quatf;
using Matrix = simd::float4x4;

const Vector3 Vector3_Left = simd::float3{-1.f, 0.f, 0.f};
const Vector3 Vector3_Right = simd::float3{1.f, 0.f, 0.f};
const Vector3 Vector3_Up = simd::float3{0.f, 1.f, 0.f};
const Vector3 Vector3_Down = simd::float3{0.f, -1.f, 0.f};
const Vector3 Vector3_Backward = simd::float3{0.f, 0.f, 1.f};
const Vector3 Vector3_Forward = simd::float3{0.f, 0.f, -1.f};
const Vector3 Vector3_Zero = simd::float3{0.f,0.f,0.f};

Matrix Identity();
Matrix CreateLookAt(Vector3 eye, Vector3 target, Vector3 up);
Matrix CreatePerspectiveFieldOfView(float fov, float aspectRatio, float znear,float zfar);
Matrix CreateOrthographic(float width, float height, float zNearPlane,float zFarPlane);
Matrix CreateOrthographicOffCenter(float left, float right, float bottom,float top, float far, float near);
Matrix CreateFromQuaternion(Quaternion rot);
Quaternion CreateFromRotationMatrix(Matrix m);
Matrix CreateScale(Vector3 scale);
Matrix translate(const Vector3 &v);
Matrix rotateZ(float angleRadians);
Matrix rotateY(float angleRadians);
Matrix rotateX(float angleRadians);
Vector3 add(const Vector3 &a, const Vector3 &b);

Vector3 Up(const Matrix &m);
Vector3 Down(const Matrix &m);
Vector3 Right(const Matrix &m);
Vector3 Left(const Matrix &m);
Vector3 Forward(const Matrix &m);
Vector3 Backward(const Matrix &m);

Vector3 Up(Matrix &m, Vector3 v);
Vector3 Down(Matrix &m, Vector3 v);
Vector3 Right(Matrix &m, Vector3 v);
Vector3 Left(Matrix &m, Vector3 v);
Vector3 Forward(Matrix &m, Vector3 v);
Vector3 Backward(Matrix &m, Vector3 v);

} // namespace SimpleMath

} // namespace Metal
