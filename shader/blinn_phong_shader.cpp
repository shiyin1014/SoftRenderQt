//
// Created by dushi on 2023/3/18.
//

#include "shader.h"
Vector3f Shader::bary_inter(const Vector3f& bary, const Vector4f pos[], const float& denom){
    Vector4f v1 = pos[0]*bary.x/payload.out_vertex_clipPos[0].w;
    Vector4f v2 = pos[1]*bary.y/payload.out_vertex_clipPos[1].w;
    Vector4f v3 = pos[2]*bary.z/payload.out_vertex_clipPos[2].w;
    Vector3f result(v1.x+v2.x+v3.x,v1.y+v2.y+v3.y,v1.z+v2.z+v3.z);
    return result*denom;
}

Vector3f Shader::bary_inter(const Vector3f& bary, const Vector3f pos[], const float& denom){
    Vector3f v1 = pos[0]*bary.x/payload.out_vertex_clipPos[0].w;
    Vector3f v2 = pos[1]*bary.y/payload.out_vertex_clipPos[1].w;
    Vector3f v3 = pos[2]*bary.z/payload.out_vertex_clipPos[2].w;
    return (v1+v2+v3)*denom;
}

Vector2f Shader::bary_inter(const Vector3f& bary, const Vector2f pos[], const float& denom){
    Vector2f v1 = pos[0]*bary.x/payload.out_vertex_clipPos[0].w;
    Vector2f v2 = pos[1]*bary.y/payload.out_vertex_clipPos[1].w;
    Vector2f v3 = pos[2]*bary.z/payload.out_vertex_clipPos[2].w;
    Vector2f result(v1.x+v2.x+v3.x,v1.y+v2.y+v3.y);
    return result*denom;
}

Vector3f Shader::get_color_from_texture(Vector2f uv,cv::Mat* image){
    int width = (*image).rows;
    int height = (*image).cols;
    float r = (*image).at<cv::Vec3b>((int)(uv.y*(float )width),(int )(uv.x*(float )height))[0];
    float g = (*image).at<cv::Vec3b>((int)(uv.y*(float )width),(int )(uv.x*(float )height))[1];
    float b = (*image).at<cv::Vec3b>((int)(uv.y*(float )width),(int )(uv.x*(float )height))[2];
    return {r,g,b};
}

void BlinnPhongShader::vertex_shader(int index) {
    payload.out_vertex_clipPos[index] = mat4_mul_vec4(payload.mvp_matrix,payload.vertex_pos[index]);
    payload.in_clip[index] = payload.out_vertex_clipPos[index];
}

Vector3f BlinnPhongShader::fragment_shader(const Vector3f& barycentric) {
    float denom = 1.0f/(barycentric.x/payload.out_vertex_clipPos[0].w+barycentric.y/payload.out_vertex_clipPos[1].w+
            barycentric.z/payload.out_vertex_clipPos[2].w);
    //插值处理
    Vector3f world_pos = bary_inter(barycentric,payload.vertex_pos,denom);
    Vector2f uv_pos = bary_inter(barycentric,payload.uv_pos,denom);
    Vector3f normal_dir = bary_inter(barycentric,payload.normal_dir,denom);
    normal_dir.normalized();

    Vector3f total_rgb;
    Vector3f color,ks;
    //diffuse map
    Vector3f diffuse_color = get_color_from_texture(uv_pos,payload.model->diffuse_texture);
    color = diffuse_color/255.0f;
    //specular map
    float pow_times = 0.0f;
    if (payload.model->specular_texture){
        Vector3f specular_pow = get_color_from_texture(uv_pos,payload.model->specular_texture);
        pow_times = specular_pow.x;
    }

    ks = {0.3};
    float ka = 0.6;

    //ambient
    Vector3f ambient = color * ka;
    //diffuse
    Vector3f light_pos = payload.point_light.position;
    Vector3f light_dir = (light_pos - world_pos).normalized();
    float normal_dot_light = light_dir*normal_dir;
    float diff = std::max(0.0f,normal_dot_light);
    Vector3f diffuse = color * diff ;

    //specular
    Vector3f view_pos = payload.camera->getEye();
    Vector3f view_dir = (view_pos - world_pos).normalized();
    Vector3f half_dir = (view_dir+light_dir).normalized();
    Vector3f specular = ks * std::pow(std::max(0.0f,half_dir*normal_dir),128.0f+pow_times);

    total_rgb = ambient + diffuse + specular;
    return total_rgb * 255.0f;
}


