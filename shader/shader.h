//
// Created by dushi on 2023/3/18.
//

#ifndef SOFTRENDERQT_SHADER_H
#define SOFTRENDERQT_SHADER_H

#include "../core/maths.h"
#include "../core/camera.h"
#include "../core/model.hpp"
#include "../core/triangle.h"

struct light{
    Vector3f position;
    Vector3f intensity;
};

struct payloadInfo{
    //triangle
    Vector4f vertex_pos[3];
    Vector2f uv_pos[3];
    Vector3f normal_dir[3];
    Vector3f tangent_dir;//tangent of vertex

    //matrix
    mat4 model_matrix;
    mat4 view_matrix;
    mat4 perspective_matrix;
    mat4 mvp_matrix;

    //camera and model
    Camera *camera;
    Model *model;
    std::vector<Triangle*> triangleList;

    //for homogeneous clipping
    Vector2f in_uv[MAX_VERTEX];
    Vector3f in_normal[MAX_VERTEX];
    Vector4f in_world[MAX_VERTEX];
    Vector4f in_clip[MAX_VERTEX];
    Vector2f out_uv[MAX_VERTEX];
    Vector3f out_normal[MAX_VERTEX];
    Vector4f out_world[MAX_VERTEX];
    Vector4f out_clip[MAX_VERTEX];

    //in and out information
    light point_light;
    Vector4f out_vertex_clipPos[3];

    //tangent_space_information_for_lighting_calculate
    Vector3f tangent_point_light_pos[3];
    Vector3f tangent_eye_pos[3];
    Vector3f tangent_vertex_pos[3];
};

class Shader{
public:
    payloadInfo payload;
    virtual void vertex_shader(int index)=0;
    virtual Vector3f fragment_shader(const Vector3f& barycentric)=0;

    static Vector3f get_color_from_texture(Vector2f uv,cv::Mat* image);
    Vector2f bary_inter(const Vector3f& bary, const Vector2f pos[], const float& denom);
    Vector3f bary_inter(const Vector3f& bary, const Vector3f pos[], const float& denom);
    Vector3f bary_inter(const Vector3f& bary, const Vector4f pos[], const float& denom);
};

class BlinnPhongShader : public Shader{
public:
    void vertex_shader(int index) override;
    Vector3f fragment_shader(const Vector3f& barycentric) override;
};

class BlinnPhongTangentShader : public Shader{
public:
    void vertex_shader(int index) override;
    Vector3f fragment_shader(const Vector3f& barycentric) override;
};


//class SkyBoxShader : public Shader{
//public:
//    void vertex_shader(int index) override;
//    Vector3f fragment_shader(const Vector3f& barycentric) override;
//};
//
//class PBRShader : public Shader{
//public:
//    void vertex_shader(int index) override;
//    Vector3f fragment_shader(const Vector3f& barycentric) override;
//
//    //https://learnopengl-cn.github.io/07%20PBR/01%20Theory/
//    //normal distribution function
//    float ndf_ggx_tr(Vector3f&normal,Vector3f& half, const float& roughness);
//    //geometry function
//    static float geometry_schlick_ggx(Vector3f& normal,Vector3f& view_or_light, const float& roughness);
//    //geometry_smith
//    float geometry_smith(Vector3f& normal,Vector3f& view_dir,Vector3f& light_dir,const float& roughness);
//    //fresnel function
//    Vector3f fresnel_schlick(float cos_theta,Vector3f& f0);
//    PBRShader();
//    ~PBRShader()= default;
//private:
//    //virtual point lights in world space
//    std::vector<light> lights;
//};

#endif //SOFTRENDERQT_SHADER_H
