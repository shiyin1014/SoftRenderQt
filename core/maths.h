//
// Created by dushi on 2023/1/25.
//

#ifndef SOFTRENDERERQT_MATHS_H
#define SOFTRENDERERQT_MATHS_H

#include <ostream>
#include <vector>
#include "cmath"
#include "macro.h"
//-------------------------------------------------------vector2-------------------------------------------------------
template <class t>
struct Vector2{
    t x,y;//成员变量
    Vector2(){
        x = 0;
        y =0;
    }
    Vector2(t _x, t _y){
        x = _x;
        y = _y;
    }
    //操作符重载
    inline Vector2<t> operator +(Vector2<t> &v) const{
        return Vector2(x+v.x,y+v.y);
    }
    inline Vector2<t> operator -(Vector2<t> &v) const{
        return Vector2(x-v.x,y-v.y);
    }
    inline Vector2<t> operator *(float f) const{
        return Vector2(x*f,y*f);
    }
    inline Vector2<t> operator /(float f) const{
        return Vector2(x/f,y/f);
    }
    //左移运算符重载
    template<class >
    friend std::ostream& operator<<(std::ostream& out,Vector2<t> &v);
};

template<class t>
std::ostream &operator<<(std::ostream &out, Vector2<t> &v) {
    out<<"("<<v.x<<","<<v.y<<")"<<std::endl;
    return out;
}

//-------------------------------------------------------vector3-------------------------------------------------------
template <class t>
struct Vector3{
    t x,y,z;
    Vector3(){
        x=0;
        y=0;
        z=0;
    }
    Vector3(t _xyz){
        x = _xyz;
        y = _xyz;
        z = _xyz;
    }
    Vector3(t _x, t _y,t _z){
        x = _x;
        y = _y;
        z = _z;
    }
    inline Vector3<t> operator +(Vector3<t> &v) const{
        return Vector3(x+v.x,y+v.y,z+v.z);
    }
    inline Vector3<t> operator -(Vector3<t> &v) const{
        return Vector3(x-v.x,y-v.y,z-v.z);
    }
    inline Vector3<t> operator /(Vector3<t> &v) const{
        return Vector3(x/v.x,y/v.y,z/v.z);
    }
    inline Vector3<t> operator-(float f) const{
        return Vector3(x-f,y-f,z-f);
    }
    inline Vector3<t> operator *(float f) const{
        return Vector3(x*f,y*f,z*f);
    }
    inline Vector3<t> operator /(float f) const{
        return Vector3(x/f,y/f,z/f);
    }
    inline Vector3<t> operator +(float f) const{
        return Vector3(x+f,y+f,z+f);
    }
    Vector3<t>& operator +=(const Vector3<t>& v){
        x+=v.x;
        y+=v.y;
        z+=v.z;
        return *this;
    }
    //两个向量的点乘
    inline t operator *(Vector3<t>& v) const{
        return x*v.x+y*v.y+z*v.z;
    }
    //两个三维向量的叉乘
    inline Vector3<t> operator ^(Vector3<t>& v) const{
        return Vector3(y*v.z-z*v.y,z*v.x-x*v.z,x*v.y-y*v.x);
    }
    //取模
    [[nodiscard]] float norm() const{
        return std::sqrt(x*x+y*y+z*z);
    }
    //向量单位化
    Vector3<t>& normalized(t l =1){
        *this = (*this)*(l/norm());//注意这里由于没有实现除法/操作符重载的函数，因此只能写成乘法的形式
        return *this;
    }
    template<class>
    friend std::ostream& operator<<(std::ostream& out,Vector3<t> &v);

};

template<class t>
std::ostream &operator<<(std::ostream &out, Vector3<t> &v) {
    out<<"("<<v.x<<","<<v.y<<","<<v.z<<")"<<std::endl;
    return out;
}

template <class t>
struct Vector4{
    t x,y,z,w;
    Vector4(){
        x=0;
        y=0;
        z=0;
        w=0;
    }
    Vector4(t _x,t _y,t _z,t _w){
        x = _x;
        y = _y;
        z = _z;
        w = _w;
    }
    Vector4(Vector3<t> v){
        x = v.x;
        y = v.y;
        z = v.z;
        w = 1.0;
    }

    inline Vector4<t> operator -(Vector4<t>& v) const{
        return Vector4<t>(x-v.x,y-v.y,z-v.z,w-v.w);
    }

    inline Vector4<t> operator *(float f) const{
        return Vector4<t>(x*f,y*f,z*f,w*f);
    }

    inline Vector4<t> operator +(Vector4<t> &v) const{
        return Vector4(x+v.x,y+v.y,z+v.z,w+v.w);
    }

    inline Vector4<t> operator /(float f) const{
        return Vector4(x/f,y/f,z/f,w/f);
    }

    Vector4<t>& operator +=(float f){
        x+=f;
        y+=f;
        z+=f;
        w+=f;
        return *this;
    }

    template<class>
    friend std::ostream& operator<<(std::ostream& out, Vector4<t>& v);
};

template<class t>
std::ostream &operator<<(std::ostream &out, Vector4<t> &v) {
    out<<"("<<v.x<<","<<v.y<<","<<v.z<<","<<v.w<<")"<<std::endl;
    return out;
}

typedef Vector2<float> Vector2f;
typedef Vector2<int> Vector2i;
typedef Vector3<float> Vector3f;
typedef Vector3<int> Vector3i;
typedef Vector4<float> Vector4f;

//-------------------------------------------------------mat-------------------------------------------------------
typedef struct {
    float raw[3][3];
}mat3;

typedef struct{
    float raw[4][4];
}mat4;

//mat3矩阵相关函数
mat3 mat3_identity();
mat3 mat3_from_mat4(mat4 m4);
Vector3f mat3_mul_vec3(mat3 m3,Vector3f v);
mat3 mat3_mul_mat3(mat3 m1,mat3 m2);
mat3 mat3_inverse(mat3 m3);
mat3 mat3_transpose(mat3 m3);
mat3 mat3_inverse_transpose(mat3 m3);
mat3 mat3_from_3_vector3f(Vector3f v1,Vector3f v2,Vector3f v3);
void mat3_print(const char* name,mat3 m);

//mat4矩阵相关函数
mat4 mat4_identity();
Vector4f mat4_mul_vec4(mat4 m,Vector4f v);
mat4 mat4_mul_mat4(mat4 m1,mat4 m2);
mat4 mat4_inverse(mat4 m);
mat4 mat4_transpose(mat4 m);
mat4 mat4_inverse_transpose(mat4 m);
mat4 mat4_mul_float(mat4 m,float f);
void mat4_print(const char* name,mat4 m);

//变换相关函数
mat4 mat4_translation(float x,float y,float z);
mat4 mat4_scale(float x,float y,float z);
mat4 mat4_rotate_x(float angle);
mat4 mat4_rotate_y(float angle);
mat4 mat4_rotate_z(float angle);
mat4 mat4_lookAt(Vector3f eyePos,Vector3f targetPos,Vector3f upDir);
mat4 mat4_perspective(float fov,float aspect_ratio,float near,float far);


Vector2f vector2f_lerp(Vector2f start,Vector2f end,float alpha);
Vector3f vector3f_lerp(Vector3f start,Vector3f end,float alpha);
Vector4f vector4f_lerp(Vector4f start,Vector4f end,float alpha);


#endif //SOFTRENDERERQT_MATHS_H
