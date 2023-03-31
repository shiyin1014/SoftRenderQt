////
//// Created by dushi on 2023/3/24.
////
//
//#include "shader.h"
//float PBRShader::ndf_ggx_tr(Vector3f&normal,Vector3f& half, const float& roughness){
//    float a = roughness*roughness;
//    float a_2 = a*a;
//    float n_dot_h = std::max(0.0f,normal*half);
//    float n_dot_h_2 = n_dot_h*n_dot_h;
//    float nom = a_2;
//    float denom = n_dot_h_2*(a_2-1.0f)+1.0f;
//    denom = denom*denom*M_PI;
//    return nom/denom;
//}
//
//void PBRShader::vertex_shader(int index) {
//    payload.out_vertex_clipPos[index] = mat4_mul_vec4(payload.mvp_matrix,payload.vertex_pos[index]);
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
//    Vector3f tangent_vertex_pos = mat3_mul_vec3(TBN,Vector3f(payload.vertex_pos[index].x,
//                                                             payload.vertex_pos[index].y,
//                                                             payload.vertex_pos[index].z));
//    payload.tangent_vertex_pos[index] = tangent_vertex_pos;
//    payload.tangent_eye_pos[index] = tangent_eye_pos;
//    //将预先定义的四个点光源从世界坐标转换到切线空间中去
//    payload.tangent_pbr_point_light_1[index] = mat3_mul_vec3(TBN,lights[0].position);
//    payload.tangent_pbr_point_light_2[index] = mat3_mul_vec3(TBN,lights[1].position);
//    payload.tangent_pbr_point_light_3[index] = mat3_mul_vec3(TBN,lights[2].position);
//    payload.tangent_pbr_point_light_4[index] = mat3_mul_vec3(TBN,lights[3].position);
//}
//
//Vector3f mix(Vector3f& x,Vector3f& y,float a){
//    Vector3f v1 = x*(1-a);
//    Vector3f v2 = y*a;
//    return v1+v2;
//}
//
//Vector3f pow_function(Vector3f vector3, Vector3f vector31) {
//    return {pow(vector3.x,vector31.x),pow(vector3.y,vector31.y),pow(vector3.z,vector31.z)};
//}
//
//Vector3f PBRShader::fragment_shader(const Vector3f& barycentric) {
//    float denom = 1.0f/(barycentric.x/payload.out_vertex_clipPos[0].w+barycentric.y/payload.out_vertex_clipPos[1].w+
//                        barycentric.z/payload.out_vertex_clipPos[2].w);
//    Vector2f uv_pos = bary_inter(barycentric,payload.uv_pos,denom);
//    // albedo
//    Vector3f albedo = get_color_from_texture(uv_pos,payload.model->diffuse_texture)*(1.0f/255.0f);
//    // metallic
//    float metallic = get_color_from_texture(uv_pos,payload.model->metalness_texture).x*(1.0f/255.0f);
//    // roughness
//    float roughness = get_color_from_texture(uv_pos,payload.model->roughness_texture).x*(1.0f/255.0f);
//    // normal
//    Vector3f normal_dir = get_color_from_texture(uv_pos,payload.model->normal_tangent_texture);//0-255
//    normal_dir = normal_dir * (1.0f/255.0f);//0-1
//    Vector3f normal = normal_dir*2.0f - 1.0f;//from [0-1] to [-1,1]
//    normal.normalized();//tangent space normal dir
//    //插值计算属性
//    Vector3f tangent_frag_pos = bary_inter(barycentric,payload.tangent_vertex_pos,denom);
//    Vector3f tangent_view_pos = bary_inter(barycentric,payload.tangent_eye_pos,denom);
//    Vector3f view_dir = (tangent_view_pos-tangent_frag_pos).normalized();
//    std::vector<Vector3f> tangent_lights_pos;
//    tangent_lights_pos.push_back(bary_inter(barycentric,payload.tangent_pbr_point_light_1,denom));
//    tangent_lights_pos.push_back(bary_inter(barycentric,payload.tangent_pbr_point_light_2,denom));
//    tangent_lights_pos.push_back(bary_inter(barycentric,payload.tangent_pbr_point_light_3,denom));
//    tangent_lights_pos.push_back(bary_inter(barycentric,payload.tangent_pbr_point_light_4,denom));
//    Vector3f F0(0.04f);
//    F0 = mix(F0,albedo,metallic);
//    Vector3f Lo(0.0f);
//    for (int i = 0; i < 4; ++i) {
//        // calculate per-light radiance
//        Vector3f light_pos = tangent_lights_pos[i];
//        Vector3f light_dir = (light_pos - tangent_frag_pos).normalized();
//        Vector3f half_dir = (light_dir+view_dir).normalized();
//        float distance = (light_pos - tangent_frag_pos).norm();
//        float attenuation = 1.0f/(distance*distance);
//        Vector3f radiance = lights[i].intensity * attenuation;//点光源经过距离衰减后到达当前着色点的能量
//
//        //cook-torrance brdf (specular)
//        float NDF = ndf_ggx_tr(normal,half_dir,roughness);
//        float G = geometry_smith(normal,view_dir,light_dir,roughness);
//        Vector3f F = fresnel_schlick(std::max(0.0f,half_dir*view_dir),F0);
//        Vector3f nom = F * NDF * G;
//        float denom = 4.0f * std::max(0.0f,normal*light_dir)*std::max(0.0f,normal*view_dir)+0.0001f;
//        Vector3f specular = nom/denom;
//
//        //kS is equal to fresnel
//        Vector3f kS = F;
//        Vector3f kD = Vector3f (1.0f) - kS;
//        kD = kD * (1.0f-metallic);//kD 表示漫反射系数，只有非金属才有，因此这里取反
//
//        float normal_dot_light = std::max(0.0f,normal*light_dir);
//        Vector3f diffuse = kD*albedo/M_PI;
//        Vector3f current_Lo = (diffuse+specular)*radiance*normal_dot_light;
//        Lo = Lo + current_Lo;
//    }
//
//    //ambient lighting
//    Vector3f ambient = albedo * 0.05;
//    Vector3f color = ambient+Lo;
//    // HDR
//    Vector3f temp_color = color+1.0f;
//    color = color / temp_color;
//    //gamma correct
//    color = pow_function(color,Vector3f(1.0/2.2));
//    return color * 255.0f;
//}
//
//float PBRShader::geometry_schlick_ggx(Vector3f &normal, Vector3f &view_or_light, const float &roughness) {
//    float r = roughness+1.0f;
//    float k = r*r/8.0f;
//    float nom = std::max(0.0f,normal*view_or_light);
//    float denom = nom*(1-k)+k;
//    return nom/denom;
//}
//
//float PBRShader::geometry_smith(Vector3f &normal, Vector3f &view_dir, Vector3f &light_dir, const float &roughness) {
//    float ggx1 = geometry_schlick_ggx(normal,view_dir,roughness);
//    float ggx2 = geometry_schlick_ggx(normal,light_dir,roughness);
//    return ggx1*ggx2;
//}
//
//Vector3f PBRShader::fresnel_schlick(float cos_theta, Vector3f &f0) {
//    Vector3f res = (Vector3f(1.0f) - f0)* (float )pow(1.0-cos_theta,5.0f);
//    return f0+res;
//}
//
//PBRShader::PBRShader() {
//    light light1{Vector3f(3,3,3),Vector3f(3.0f) };
//    light light2{Vector3f (-3,0,3),Vector3f(3.0f) };
//    light light3{Vector3f(3,3,0),Vector3f(3.0f) };
//    light light4{Vector3f(-3,3,0),Vector3f(3.0f) };
//    lights.push_back(light1);
//    lights.push_back(light2);
//    lights.push_back(light3);
//    lights.push_back(light4);
//}
//
//
