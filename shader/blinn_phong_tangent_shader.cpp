//
// Created by dushi on 2023/3/23.
//

#include "shader.h"

void BlinnPhongTangentShader::vertex_shader(int index) {
    payload.out_vertex_clipPos[index] = mat4_mul_vec4(payload.mvp_matrix,payload.vertex_pos[index]);
    payload.in_clip[index] = payload.out_vertex_clipPos[index];

//    //计算TBN矩阵
//    mat3 normal_matrix = mat3_from_mat4(payload.model_matrix);
//    Vector3f t = payload.tangent_dir;
//    Vector3f vertex_normal = payload.normal_dir[index];
//    Vector3f temp(vertex_normal * (t * vertex_normal));
//    Vector3f ortho_T = (t - temp).normalized();
//    Vector3f T = mat3_mul_vec3(normal_matrix,ortho_T).normalized();
//    Vector3f N = mat3_mul_vec3(normal_matrix,vertex_normal).normalized();
//    Vector3f B = (N^T).normalized();
//    B = mat3_mul_vec3(normal_matrix,B).normalized();
//    mat3 TBN = mat3_from_3_vector3f(T,B,N);
//    //将计算光照的相关信息从世界空间转换到切线空间中去
//    Vector3f tangent_eye_pos = mat3_mul_vec3(TBN,payload.camera->getEye());
//    Vector3f tangent_light_pos = mat3_mul_vec3(TBN,payload.point_light.position);
//    Vector3f tangent_vertex_pos = mat3_mul_vec3(TBN,Vector3f(payload.vertex_pos[index].x,
//                                                             payload.vertex_pos[index].y,
//                                                             payload.vertex_pos[index].z));
//    payload.tangent_vertex_pos[index] = tangent_vertex_pos;
//    payload.tangent_eye_pos[index] = tangent_eye_pos;//每个顶点的TBN矩阵不一样，因此对应的属性也不同
//    payload.tangent_point_light_pos[index] = tangent_light_pos;

//    //将顶点在点光源下的投影的裁剪空间的坐标保存下来为了计算阴影
//    Vector4f light_space_vertex_pos = mat4_mul_vec4(payload.light_mvp_matrix,payload.vertex_pos[index]);
//    payload.light_space_vertex_pos[index] = light_space_vertex_pos;
}

Vector3f BlinnPhongTangentShader::fragment_shader(const Vector3f& barycentric) {
    float denom = 1.0f/(barycentric.x/payload.out_vertex_clipPos[0].w+barycentric.y/payload.out_vertex_clipPos[1].w+
                        barycentric.z/payload.out_vertex_clipPos[2].w);
    Vector3f total_color(0,0,0);
    Vector3f color,ks;
    Vector2f uv_pos = bary_inter(barycentric,payload.uv_pos,denom);
    //diffuse and specular from textures
    Vector3f diffuse_rgb = get_color_from_texture(uv_pos,payload.model->diffuse_texture);
    float pow_times = 0.0f;
    if (payload.model->specular_texture){
        Vector3f specular_rgb = get_color_from_texture(uv_pos,payload.model->specular_texture);
        pow_times = specular_rgb.x;
    }
    //tangent space vertex normal
    Vector3f normal_dir = get_color_from_texture(uv_pos,payload.model->normal_tangent_texture);//0-255
    normal_dir = normal_dir * (1.0f / 255.0f);//0-1
    Vector3f normal = normal_dir * 2.0f - 1.0f;//转换到[-1,1]范围内
    normal.normalized();

    color = diffuse_rgb * (1.0f / 255.0f);

    ks = {0.3};
    float ka =0.6;

    //插值计算属性
    Vector3f tangent_frag_pos = bary_inter(barycentric,payload.tangent_vertex_pos,denom);
    Vector3f tangent_view_pos = bary_inter(barycentric,payload.tangent_eye_pos,denom);
    Vector3f tangent_light_pos = bary_inter(barycentric,payload.tangent_point_light_pos,denom);
    Vector3f light_dir = (tangent_light_pos-tangent_frag_pos).normalized();
    Vector3f view_dir = (tangent_view_pos-tangent_frag_pos).normalized();

    //blinn-phong shading model
    //ambient
    Vector3f ambient = color*ka;
    //diffuse
    float diff = std::max(0.0f,light_dir*normal);
    Vector3f diffuse = color * diff;
    //specular
    Vector3f half_vector = (light_dir+view_dir).normalized();
    float spec = std::pow(std::max(0.0f,half_vector*normal),256.0f+pow_times);
    Vector3f specular = ks * spec;

    total_color = ambient + diffuse + specular;
    return total_color * 255.0f;
}