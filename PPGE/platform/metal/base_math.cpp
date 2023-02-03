#include "base_math.h"



namespace Metal
{
    using float4 = simd_float4;

SimpleMath::Matrix Identity(){
    return (simd_float4x4){(float4){1.f, 0.f, 0.f, 0.f},
                           (float4){0.f, 1.f, 0.f, 0.f},
                           (float4){0.f, 0.f, 1.f, 0.f},
                           (float4){0.f, 0.f, 0.f, 1.f}};
}
    SimpleMath::Matrix CreatePerspectiveFieldOfView(float fov, float aspectRatio, float znear, float zfar)
    {
        float ys = 1.0f / tanf(fov * 0.5f);
        float xs = ys / aspectRatio;
        float zs = zfar / (znear - zfar);
        
        return simd_matrix_from_rows((float4){xs, 0.0f, 0.0f, 0.0f},
                                     (float4){0.0f, ys, 0.0f, 0.0f},
                                     (float4){0.0f, 0.0f, zs, znear * zs},
                                     (float4){0.0f, 0.0f, -1.0f, 0.0f});
    }

    SimpleMath::Matrix CreateOrthographicOffCenter(float left, float right, float bottom, float top, float far,
                                                          float near)
    {
        float sLength = 1.0f / (right - left);
        float sHeight = 1.0f / (top - bottom);
        float sDepth = 1.0f / (far - near);

        simd::float4 P;
        simd::float4 Q;
        simd::float4 R;
        simd::float4 S;

        P.x = 2.0f * sLength;
        P.y = 0.0f;
        P.z = 0.0f;
        P.w = 0.0f;

        Q.x = 0.0f;
        Q.y = 2.0f * sHeight;
        Q.z = 0.0f;
        Q.w = 0.0f;

        R.x = 0.0f;
        R.y = 0.0f;
        R.z = sDepth;
        R.w = 0.0f;

        S.x = 0.0f;
        S.y = 0.0f;
        S.z = -near * sDepth;
        S.w = 1.0f;

        return Metal::SimpleMath::Matrix{P, Q, R, S};
    }

    SimpleMath::Matrix CreateOrthographic(float width, float height, float zNearPlane, float zFarPlane)
    {
        return CreateOrthographicOffCenter(-width / 2., width / 2., -height / 2., height / 2., zFarPlane, zNearPlane);
    }

    SimpleMath::Matrix CreateFromQuaternion(SimpleMath::Quaternion rot)
    {
        return simd::float4x4(rot);
    }

    SimpleMath::Matrix CreateScale(SimpleMath::Vector3 v)
    {
        
        return simd_matrix(
            (float4){v.x, 0, 0, 0},
            (float4){0, v.y, 0, 0},
            (float4){0, 0, v.z, 0},
            (float4){0, 0, 0, 1});
    }

    SimpleMath::Matrix rotateX(float angleRadians)
   {
       const float a = angleRadians;
       return simd_matrix_from_rows((float4){1.0f, 0.0f, 0.0f, 0.0f},
                                    (float4){0.0f, cosf(a), sinf(a), 0.0f},
                                    (float4){0.0f, -sinf(a), cosf(a), 0.0f},
                                    (float4){0.0f, 0.0f, 0.0f, 1.0f});
   }

    SimpleMath::Matrix rotateY(float angleRadians)
   {
       const float a = angleRadians;
       return simd_matrix_from_rows((float4){cosf(a), 0.0f, sinf(a), 0.0f},
                                    (float4){0.0f, 1.0f, 0.0f, 0.0f},
                                    (float4){-sinf(a), 0.0f, cosf(a), 0.0f},
                                    (float4){0.0f, 0.0f, 0.0f, 1.0f});
   }

    SimpleMath::Matrix rotateZ(float angleRadians)
   {

       const float a = angleRadians;
       return simd_matrix_from_rows((float4){cosf(a), sinf(a), 0.0f, 0.0f},
                                    (float4){-sinf(a), cosf(a), 0.0f, 0.0f},
                                    (float4){0.0f, 0.0f, 1.0f, 0.0f},
                                    (float4){0.0f, 0.0f, 0.0f, 1.0f});
   }

    SimpleMath::Matrix translate(const SimpleMath::Vector3 &v)
   {
       const float4 col0 = {1.0f, 0.0f, 0.0f, 0.0f};
       const float4 col1 = {0.0f, 1.0f, 0.0f, 0.0f};
       const float4 col2 = {0.0f, 0.0f, 1.0f, 0.0f};
       const float4 col3 = {v.x, v.y, v.z, 1.0f};
       return simd_matrix(col0, col1, col2, col3);
   }

    SimpleMath::Vector3 add(const SimpleMath::Vector3 &a, const SimpleMath::Vector3 &b)
    {
        return (SimpleMath::Vector3){a.x + b.x, a.y + b.y, a.z + b.z};
    }

    SimpleMath::Quaternion CreateFromRotationMatrix(const SimpleMath::Matrix &m)
    {
        
        return simd::quatf(m);
    }

    SimpleMath::Vector3 Up(const SimpleMath::Matrix &m){return simd::float3{m.columns[0].y, m.columns[1].y, m.columns[2].y};}
    SimpleMath::Vector3 Down(const SimpleMath::Matrix &m){return simd::float3{-m.columns[0].y, -m.columns[1].y, -m.columns[2].y};}
    SimpleMath::Vector3 Right(const SimpleMath::Matrix &m){return simd::float3{m.columns[0].x, m.columns[1].x, m.columns[2].x};}
    SimpleMath::Vector3 Left(const SimpleMath::Matrix &m){return simd::float3{-m.columns[0].x, -m.columns[1].x, -m.columns[2].x};}
    SimpleMath::Vector3 Backward(const SimpleMath::Matrix &m){return simd::float3{m.columns[0].z, m.columns[1].z, m.columns[2].z};}
    SimpleMath::Vector3 Forward(const SimpleMath::Matrix &m){return simd::float3{-m.columns[0].z, -m.columns[1].z, -m.columns[2].z};}

    SimpleMath::Vector3 Up(SimpleMath::Matrix &m, SimpleMath::Vector3 v){
        m.columns[0].y = v.x;
        m.columns[1].y = v.y;
        m.columns[2].y = v.z;
    }
    SimpleMath::Vector3 Down(SimpleMath::Matrix &m, SimpleMath::Vector3 v){
        m.columns[0].y = -v.x;
        m.columns[1].y = -v.y;
        m.columns[2].y = -v.z;
    }
    SimpleMath::Vector3 Right(SimpleMath::Matrix &m, SimpleMath::Vector3 v){
        m.columns[0].x = v.x;
        m.columns[1].x = v.y;
        m.columns[2].x = v.z;
    }
    SimpleMath::Vector3 Left(SimpleMath::Matrix &m, SimpleMath::Vector3 v){
        m.columns[0].x = -v.x;
        m.columns[1].x = -v.y;
        m.columns[2].x = -v.z;
    }
    SimpleMath::Vector3 Forward(SimpleMath::Matrix &m, SimpleMath::Vector3 v){

        m.columns[0].z = -v.x;
        m.columns[1].z = -v.y;
        m.columns[2].z = -v.z;
    }
    SimpleMath::Vector3 Backward(SimpleMath::Matrix &m, SimpleMath::Vector3 v){
        m.columns[0].z = v.x;
        m.columns[1].z = v.y;
        m.columns[2].z = v.z;
    }

    SimpleMath::Matrix CreateLookAt(SimpleMath::Vector3 eye, SimpleMath::Vector3 target, SimpleMath::Vector3 up)
    {
        
        auto zaxis = simd_normalize((target - eye));
        auto xaxis = simd_normalize(simd_cross(up, zaxis));
        auto yaxis = simd_cross(zaxis, xaxis);
        
        return simd_matrix_from_rows((float4){xaxis.x, yaxis.x, zaxis.x, 0.0f},
                              (float4){xaxis.y, yaxis.y, zaxis.y, 0.0f},
                              (float4){xaxis.z, yaxis.z, zaxis.z, 0.0f},
                              (float4){simd_dot(xaxis, -eye), simd_dot(yaxis, -eye), simd_dot(zaxis, -eye), 1.0f});
    }
}

