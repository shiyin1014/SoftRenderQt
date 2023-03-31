//
// Created by dushi on 2023/1/27.
//
#include <cmath>
#include <iostream>
#include <cassert>
#include "maths.h"

//-------------------------------------------------------mat3-------------------------------------------------------
//mat3的单位矩阵
mat3 mat3_identity() {
    mat3 m;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (i == j) {
                m.raw[i][j] = 1;
            } else {
                m.raw[i][j] = 0;
            }
        }
    }
    return m;
}

//从mat4中提取出左上角的mat3
mat3 mat3_from_mat4(mat4 m4) {
    mat3 m;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            m.raw[i][j] = m4.raw[i][j];
        }
    }
    return m;
}

//mat3与vec3相乘
Vector3f mat3_mul_vec3(mat3 m3, Vector3f v) {
    Vector3f res;
    res.x = m3.raw[0][0] * v.x + m3.raw[0][1] * v.y + m3.raw[0][2] * v.z;
    res.y = m3.raw[1][0] * v.x + m3.raw[1][1] * v.y + m3.raw[1][2] * v.z;
    res.z = m3.raw[2][0] * v.x + m3.raw[2][1] * v.y + m3.raw[2][2] * v.z;
    return res;
}

//两个mat3矩阵相乘
mat3 mat3_mul_mat3(mat3 m1, mat3 m2) {
    mat3 m;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            for (int k = 0; k < 3; ++k) {
                m.raw[i][j] += m1.raw[i][k] * m2.raw[k][j];
            }
        }
    }
    return m;
}

//求3*3行列式的值
static float mat3_determinant(mat3 m) {
    float a = +m.raw[0][0] * (m.raw[1][1] * m.raw[2][2] - m.raw[2][1] * m.raw[1][2]);
    float b = -m.raw[0][1] * (m.raw[1][0] * m.raw[2][2] - m.raw[2][0] * m.raw[1][2]);
    float c = +m.raw[0][2] * (m.raw[1][0] * m.raw[2][1] - m.raw[1][1] * m.raw[2][0]);
    return a + b + c;
}

//mat3的伴随矩阵
static mat3 mat3_adjoint(mat3 m) {
    mat3 adjoint;
    adjoint.raw[0][0] = +(m.raw[1][1] * m.raw[2][2] - m.raw[2][1] * m.raw[1][2]);
    adjoint.raw[0][1] = -(m.raw[1][0] * m.raw[2][2] - m.raw[2][0] * m.raw[1][2]);
    adjoint.raw[0][2] = +(m.raw[1][0] * m.raw[2][1] - m.raw[2][0] * m.raw[1][1]);
    adjoint.raw[1][0] = -(m.raw[0][1] * m.raw[2][2] - m.raw[2][1] * m.raw[0][2]);
    adjoint.raw[1][1] = +(m.raw[0][0] * m.raw[2][2] - m.raw[2][0] * m.raw[0][2]);
    adjoint.raw[1][2] = -(m.raw[0][0] * m.raw[2][1] - m.raw[2][0] * m.raw[0][1]);
    adjoint.raw[2][0] = +(m.raw[0][1] * m.raw[1][2] - m.raw[1][1] * m.raw[0][2]);
    adjoint.raw[2][1] = -(m.raw[0][0] * m.raw[1][2] - m.raw[1][0] * m.raw[0][2]);
    adjoint.raw[2][2] = +(m.raw[0][0] * m.raw[1][1] - m.raw[1][0] * m.raw[0][1]);
    return mat3_transpose(adjoint);//转置
}

//求mat3的逆矩阵
mat3 mat3_inverse(mat3 m3) {
    mat3 inverse;
    float determinant = mat3_determinant(m3);
    float inv_determinant = 1 / determinant;
    mat3 adjoint = mat3_adjoint(m3);
//    mat3_print("adjoint",adjoint);
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            inverse.raw[i][j] = adjoint.raw[i][j] * inv_determinant;
        }
    }
    return inverse;
}

//转置
mat3 mat3_transpose(mat3 m3) {
    mat3 m;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            m.raw[i][j] = m3.raw[j][i];
        }
    }
    return m;
}

//mat3逆的转置
mat3 mat3_inverse_transpose(mat3 m3) {
    mat3 m;
    mat3 inverse = mat3_inverse(m3);
    m = mat3_transpose(inverse);
    return m;
}

//从三个向量中初始化矩阵
mat3 mat3_from_3_vector3f(Vector3f v1, Vector3f v2, Vector3f v3) {
    mat3 m;
    m.raw[0][0] = v1.x;
    m.raw[0][1] = v1.y;
    m.raw[0][2] = v1.z;
    m.raw[1][0] = v2.x;
    m.raw[1][1] = v2.y;
    m.raw[1][2] = v2.z;
    m.raw[2][0] = v3.x;
    m.raw[2][1] = v3.y;
    m.raw[2][2] = v3.z;
    return m;
}

//打印
void mat3_print(const char *name, mat3 m) {
    int i, j;
    printf("%s", name);
    printf("\n");
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            printf("    %12f", m.raw[i][j]);
        }
        printf("\n");
    }
}

//-------------------------------------------------------mat4-------------------------------------------------------
//mat4的单位矩阵
mat4 mat4_identity() {
    mat4 m = {{{0}}};
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (i == j) {
                m.raw[i][j] = 1;
            } else {
                m.raw[i][j] = 0;
            }
        }
    }
    return m;
}

// mat4 与 vec4 相乘
Vector4f mat4_mul_vec4(mat4 m, Vector4f v) {
    Vector4f res;
    res.x = m.raw[0][0] * v.x + m.raw[0][1] * v.y + m.raw[0][2] * v.z + m.raw[0][3] * v.w;
    res.y = m.raw[1][0] * v.x + m.raw[1][1] * v.y + m.raw[1][2] * v.z + m.raw[1][3] * v.w;
    res.z = m.raw[2][0] * v.x + m.raw[2][1] * v.y + m.raw[2][2] * v.z + m.raw[2][3] * v.w;
    res.w = m.raw[3][0] * v.x + m.raw[3][1] * v.y + m.raw[3][2] * v.z + m.raw[3][3] * v.w;
    return res;
}

//mat4 与 mat4 相乘
mat4 mat4_mul_mat4(mat4 m1, mat4 m2) {
    mat4 m = {{{0}}};
    int i, j, k;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            for (k = 0; k < 4; k++) {
                m.raw[i][j] += m1.raw[i][k] * m2.raw[k][j];
            }
        }
    }
    return m;
}

// 去除mat4的第r行第c列的矩阵的行列式的值
static float mat4_minor(mat4 m, int r, int c) {
    mat3 cut_down;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            int row = i < r ? i : i + 1;
            int col = j < c ? j : j + 1;
            cut_down.raw[i][j] = m.raw[row][col];
        }
    }
    return mat3_determinant(cut_down);
}

static float mat4_cofactor(mat4 m, int r, int c) {
    float sign = ((r + c) % 2) == 0 ? 1.0 : -1.0f;
    float minor = mat4_minor(m, r, c);
    return sign * minor;
}

//mat4的伴随矩阵
static mat4 mat4_adjoint(mat4 m) {
    mat4 adjoint;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            adjoint.raw[i][j] = mat4_cofactor(m, i, j);
        }
    }
    return mat4_transpose(adjoint);
}

//mat4 的逆
mat4 mat4_inverse(mat4 m) {
    mat4 inverse;
    //先计算四阶行列式的值
    float determinant = 0, inv_determinant;
    mat4 adjoint = mat4_adjoint(m);
    for (int i = 0; i < 4; ++i) {
        determinant += m.raw[0][i] * adjoint.raw[i][0];
    }
    inv_determinant = 1 / determinant;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            inverse.raw[i][j] = adjoint.raw[i][j] * inv_determinant;
        }
    }
    return inverse;
}

//mat4 的转置
mat4 mat4_transpose(mat4 m) {
    mat4 transpose;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            transpose.raw[i][j] = m.raw[j][i];
        }
    }
    return transpose;
}

//mat4的逆的转置
mat4 mat4_inverse_transpose(mat4 m) {
    return mat4_transpose(mat4_inverse(m));
}

//mat4 乘上一个float变量
mat4 mat4_mul_float(mat4 m, float f) {
    mat4 res;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            res.raw[i][j] = m.raw[i][j] * f;
        }
    }
    return res;
}

//打印
void mat4_print(const char *name, mat4 m) {
    int i, j;
    printf("mat4 %s =\n", name);
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            printf("    %12f", m.raw[i][j]);
        }
        printf("\n");
    }
}

//-------------------------------------------------------transform------------------------------------------------------
mat4 mat4_translation(float x, float y, float z) {
    mat4 m = mat4_identity();
    m.raw[0][3] = -x;
    m.raw[1][3] = -y;
    m.raw[2][3] = -z;
    return m;
}

mat4 mat4_scale(float x, float y, float z) {
    mat4 scale = mat4_identity();
    assert(x != 0 && y != 0 && z != 0);
    scale.raw[0][0] = x;
    scale.raw[1][1] = y;
    scale.raw[2][2] = z;
    return scale;
}

mat4 mat4_rotate_x(float angle) {
    auto c = (float )std::cos(angle*PI/180);
    auto s = (float )std::sin(angle*PI/180);
    mat4 m = mat4_identity();
    m.raw[1][1] = c;
    m.raw[1][2] = -s;
    m.raw[2][1] = s;
    m.raw[2][2] = c;
    return m;
}

mat4 mat4_rotate_y(float angle) {
    auto c = (float )std::cos(angle*PI/180);
    auto s = (float )std::sin(angle*PI/180);
    mat4 m = mat4_identity();
    m.raw[0][0] = c;
    m.raw[0][2] = s;
    m.raw[2][0] = -s;
    m.raw[2][2] = c;
    return m;
}

mat4 mat4_rotate_z(float angle) {
    auto c = (float )std::cos(angle);
    auto s = (float )std::sin(angle);
    mat4 m = mat4_identity();
    m.raw[0][0] = c;
    m.raw[0][1] = -s;
    m.raw[1][0] = s;
    m.raw[1][1] = c;
    return m;
}

//lookAt矩阵
mat4 mat4_lookAt(Vector3f eyePos, Vector3f targetPos, Vector3f upDir) {
    mat4 lookAt;
    mat4 translation = mat4_translation(eyePos.x, eyePos.y, eyePos.z);
    Vector3f forward_dir = eyePos - targetPos;
    forward_dir.normalized();
    upDir.normalized();
    Vector3f left = upDir ^ forward_dir;
    left.normalized();
    Vector3f up = forward_dir ^ left;
    up.normalized();
    mat4 rotate = mat4_identity();
    rotate.raw[0][0] = left.x;
    rotate.raw[0][1] = left.y;
    rotate.raw[0][2] = left.z;

    rotate.raw[1][0] = up.x;
    rotate.raw[1][1] = up.y;
    rotate.raw[1][2] = up.z;

    rotate.raw[2][0] = forward_dir.x;
    rotate.raw[2][1] = forward_dir.y;
    rotate.raw[2][2] = forward_dir.z;

    lookAt = mat4_mul_mat4(rotate, translation);
    return lookAt;
}

//透视投影矩阵
mat4 mat4_perspective(float fov, float aspect_ratio, float near, float far) {
    mat4 pers = mat4_identity();
    assert(fov > 0 && near > 0 && far > 0 && far > near && aspect_ratio > 0);
    auto radian = (float )(fov * PI / 180.0);
    pers.raw[1][1] = 1 / (float) std::tan(0.5 * radian);
    pers.raw[0][0] = pers.raw[1][1] / aspect_ratio;
    pers.raw[2][2] = (near + far) / (near - far);
    pers.raw[2][3] = (2 * near * far) / (near - far);
    pers.raw[3][2] = -1;
    pers.raw[3][3] = 0;
    return pers;
}

Vector2f vector2f_lerp(Vector2f start, Vector2f end, float alpha) {
    Vector2f temp = (end - start) * alpha;
    return start + temp;
}

Vector3f vector3f_lerp(Vector3f start, Vector3f end, float alpha) {
    Vector3f temp = (end - start) * alpha;
    return start + temp;
}

Vector4f vector4f_lerp(Vector4f start, Vector4f end, float alpha) {
    Vector4f temp = (end - start) * alpha;
    return start + temp;
}
