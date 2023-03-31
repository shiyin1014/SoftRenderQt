//
// Created by dushi on 2023/3/19.
//

#include "pipeline.h"

Pipeline::Pipeline(int w, int h) {
    this->width = w;
    this->height = h;
    clear_buffer();
}

void Pipeline::clear_buffer() {
    color_buffer = std::vector<Vector3f>(width*height,Vector3f(0));
    depth_buffer = std::vector<float>(width*height,std::numeric_limits<float>::infinity());
}

void Pipeline::set_pixel_color(const Vector2i &coordinate, const Vector3f &color) {
    this->color_buffer[get_index(coordinate.x,coordinate.y)] = color;
}

int Pipeline::get_index(int x,int y) const{
    int index = (height - y - 1) * width + x;
    if (index>=250000||index<0){
        std::cout<<"x = "<<x<<","<<"y = "<<y<<std::endl;
        std::cout<<index<<std::endl;
    }
    return index;
}

bool insideTriangle(float x,float y,const Triangle& triangle){
    std::vector<Vector3f> v;
    for (auto i : triangle.v) {
        Vector3f vector3F(i.x,i.y,1.0f);
        v.push_back(vector3F);
    }
    Vector3f p(x,y,1.0);

    Vector3f a_b = v[1]-v[0];
    Vector3f b_c = v[2]-v[1];
    Vector3f c_a = v[0]-v[2];
    Vector3f a_p = p-v[0];
    Vector3f b_p = p-v[1];
    Vector3f c_p = p-v[2];
    //叉乘
    Vector3f a_p_cross_a_b = a_p^a_b;
    Vector3f b_p_cross_b_c = b_p^b_c;
    Vector3f c_p_cross_c_a = c_p^c_a;
    if ((a_p_cross_a_b.z>=0&&b_p_cross_b_c.z>=0&&c_p_cross_c_a.z>=0)||a_p_cross_a_b.z<=0&&b_p_cross_b_c.z<=0&&c_p_cross_c_a.z<=0){
        return true;
    }
    return false;
}

//获取给定三角形的重心坐标
Vector3f barycentric(Triangle triangle, Vector2f P){
    float alpha = (-(P.x -triangle.v[1].x)*(triangle.v[2].y-triangle.v[1].y)+(P.y-triangle.v[1].y)*(triangle.v[2].x - triangle.v[1].x))*1.0
                  /(1.0*(-(triangle.v[0].x -triangle.v[1].x)*(triangle.v[2].y-triangle.v[1].y)+(triangle.v[0].y-triangle.v[1].y)*(triangle.v[2].x - triangle.v[1].x)));
    float beta = (-(P.x -triangle.v[2].x)*(triangle.v[0].y-triangle.v[2].y)+(P.y-triangle.v[2].y)*(triangle.v[0].x - triangle.v[2].x))*1.0
                 /(1.0*(-(triangle.v[1].x -triangle.v[2].x)*(triangle.v[0].y-triangle.v[2].y)+(triangle.v[1].y-triangle.v[2].y)*(triangle.v[0].x - triangle.v[2].x)));
    float gamma = 1-alpha - beta;
    return {alpha,beta,gamma};
}

void Pipeline::rasterizer_triangle(const Vector3f screen_pos[],Shader& shader){
    //bounding box calculate
    //利用平面轴对齐包围盒，加速三角形光栅化
    float min_x = 99999;
    float min_y = 99999;
    float max_x = -99999;
    float max_y = -99999;
    for (int i=0;i<3;i++) {
        if (screen_pos[i].x > max_x)max_x = screen_pos[i].x;
        if (screen_pos[i].x < min_x)min_x = screen_pos[i].x;
        if (screen_pos[i].y > max_y)max_y = screen_pos[i].y;
        if (screen_pos[i].y < min_y)min_y = screen_pos[i].y;
    }
    Triangle triangle;
    for (int i = 0; i < 3; ++i) {
        triangle.v[i].x = screen_pos[i].x;
        triangle.v[i].y = screen_pos[i].y;
    }
    for (int i = (int )min_x; i <= (int )max_x; ++i) {
        for (int j = (int )min_y; j <=(int ) max_y; ++j) {
            if (insideTriangle((float )(i+0.5),(float )(j+0.5),triangle)){
                Vector3f barycentric_coord = barycentric(triangle,Vector2f((float )(i+0.5),(float )(j+0.5)));
                float alpha = barycentric_coord.x;
                float beta = barycentric_coord.y;
                float gamma = barycentric_coord.z;
                //perspective correct interpolation
                //https://stackoverflow.com/questions/24441631/how-exactly-does-opengl-do-perspectively-correct-linear-interpolation
                float denom = 1.0f/(alpha/shader.payload.out_vertex_clipPos[0].w+beta/shader.payload.out_vertex_clipPos[1].w+
                        gamma/shader.payload.out_vertex_clipPos[2].w);
                float depth = (alpha * screen_pos[0].z/shader.payload.out_vertex_clipPos[0].w +
                        beta * screen_pos[1].z/shader.payload.out_vertex_clipPos[1].w +
                        gamma * screen_pos[2].z/shader.payload.out_vertex_clipPos[2].w)*denom;
                if (depth < depth_buffer[get_index(i,j)]){
                    set_depth_value(Vector2i(i,j),depth);
                    Vector3f color = shader.fragment_shader(barycentric_coord);
                    set_pixel_color(Vector2i(i,j),color);
                }
            }
        }
    }
}

void Pipeline::draw(Shader &shader) {
    //调用顶点着色器，片元着色器进行光栅化
    for (auto & i : shader.payload.model->triangleList) {
        Triangle triangle = *i;
        if (typeid(shader).name() == typeid(BlinnPhongTangentShader).name()){
            //求切线和副切线
            float delta_u1 = triangle.tex_coords[1].x - triangle.tex_coords[0].x;
            float delta_v1 = triangle.tex_coords[1].y - triangle.tex_coords[0].y;
            float delta_u2 = triangle.tex_coords[2].x - triangle.tex_coords[0].x;
            float delta_v2 = triangle.tex_coords[2].y - triangle.tex_coords[0].y;
            float ratio = 1.0f/(delta_u1*delta_v2 - delta_u2*delta_v1);
            Vector4f e1 = triangle.v[1] - triangle.v[0];
            Vector4f e2 = triangle.v[2] - triangle.v[0];
            float tangent_x = ratio * (delta_v2*e1.x-delta_v1*e2.x);
            float tangent_y = ratio * (delta_v2*e1.y-delta_v1*e2.y);
            float tangent_z = ratio * (delta_v2*e1.z-delta_v1*e2.z);
            Vector3f tangent(tangent_x,tangent_y,tangent_z);
            tangent.normalized();
            shader.payload.tangent_dir = tangent;
        }
        //顶点着色器
        Vector3f ndc_pos[3];
        Vector3f screen_pos[3];
        for (int j = 0; j < 3; ++j) {
            shader.payload.vertex_pos[j] = triangle.v[j];
            shader.payload.in_world[j] = triangle.v[j];
            shader.payload.normal_dir[j] = triangle.normal[j];
            shader.payload.in_normal[j] = triangle.normal[j];
            shader.payload.uv_pos[j] = triangle.tex_coords[j];
            shader.payload.in_uv[j] = triangle.tex_coords[j];
            shader.vertex_shader(j);//顶点着色器
        }
        int number_vertex = homogeneous_clipping(shader.payload);
        for (int j = 0; j < number_vertex-2; ++j) {
            int index0 = 0;
            int index1 = j+1;
            int index2 = j+2;
            transform_attribute(shader,index0,index1,index2);
            for (int k = 0; k < 3; ++k) {
                Vector4f temp = shader.payload.out_vertex_clipPos[k] / shader.payload.out_vertex_clipPos[k].w;//透视除法
                ndc_pos[k] = Vector3f (temp.x,temp.y,temp.z);
                //viewport transform
                screen_pos[k].x = (ndc_pos[k].x + 1.0f) * 0.5f * (float )(width);
                screen_pos[k].y = (ndc_pos[k].y + 1.0f) * 0.5f * (float )(height);
                screen_pos[k].z = ndc_pos[k].z * 0.5f + 0.5f;
            }
            if (back_face_culling&&is_back_culling(ndc_pos))continue;
            rasterizer_triangle(screen_pos,shader);
        };
    }
}

void Pipeline::set_depth_value(const Vector2i &coordinate, const float &depth) {
    this->depth_buffer[get_index(coordinate.x,coordinate.y)] = depth;
}

std::vector<float> Pipeline::get_depth_buffer() {
    return depth_buffer;
}

std::vector<Vector3f>& Pipeline::get_color_buffer() {
    return color_buffer;
}

void Pipeline::wire_frame(Shader &shader) {
    //调用顶点着色器，片元着色器进行光栅化
    for (auto & i : shader.payload.model->triangleList) {
        Triangle triangle = *i;
        //顶点着色器
        Vector3f ndc_pos[3];
        Vector3f screen_pos[3];
        for (int j = 0; j < 3; ++j) {
            shader.payload.vertex_pos[j] = triangle.v[j];
            shader.payload.in_world[j] = triangle.v[j];
            shader.payload.normal_dir[j] = triangle.normal[j];
            shader.payload.in_normal[j] = triangle.normal[j];
            shader.payload.uv_pos[j] = triangle.tex_coords[j];
            shader.payload.in_uv[j] = triangle.tex_coords[j];
            shader.vertex_shader(j);//顶点着色器
        }
        //在顶点着色器之后和透视除法之前有一个齐次剪裁的操作，目的是去除那些在视锥体之外的物体，我们没必要在这些三角形上浪费资源
        int number = homogeneous_clipping(shader.payload);
        //组装三角形
        for (int j = 0; j < number-2; ++j) {
            int index0 = 0;
            int index1 = j + 1;
            int index2 = j + 2;
            transform_attribute(shader,index0,index1,index2);
            //透视除法与视图转换
            for (int k = 0; k < 3; ++k) {
                Vector4f temp= shader.payload.out_vertex_clipPos[k] / shader.payload.out_vertex_clipPos[k].w;//透视除法
                ndc_pos[k] = Vector3f (temp.x,temp.y,temp.z);
                //viewport transform
                screen_pos[k].x = round((ndc_pos[k].x + 1.0f) * 0.5f * (float )(width-1));//round是为了避免精度问题
                screen_pos[k].y = round((ndc_pos[k].y + 1.0f) * 0.5f * (float )(height-1));
                screen_pos[k].z = ndc_pos[k].z * 0.5f + 0.5f;
            }
            if (back_face_culling&&is_back_culling(ndc_pos))continue;
            //draw wire_frame
            draw_line(screen_pos[0],screen_pos[1]);
            draw_line(screen_pos[1],screen_pos[2]);
            draw_line(screen_pos[2],screen_pos[0]);
        }
    }
}

//Bresenham’s Line Drawing Algorithm
//https://github.com/ssloy/tinyrenderer/wiki/Lesson-1:-Bresenham%E2%80%99s-Line-Drawing-Algorithm#wireframe-rendering
void Pipeline::draw_line(const Vector3f &begin, const Vector3f &end) {
    Vector3f color(255,255,255);
    int x0 = begin.x,y0 = begin.y;
    int x1 = end.x,y1 = end.y;
    bool steep = false;
    if (std::abs(x0-x1)<std::abs(y0-y1)) {
        std::swap(x0, y0);
        std::swap(x1, y1);
        steep = true;
    }
    if (x0>x1) {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }
    int dx = x1-x0;
    int dy = y1-y0;
    int derror2 = std::abs(dy)*2;
    int error2 = 0;
    int y = y0;
    for (int x=x0; x<=x1; x++) {
        if (steep) {
            set_pixel_color(Vector2i(y,x), color);
        } else {
            set_pixel_color(Vector2i(x,y), color);
        }
        error2 += derror2;
        if (error2>dx) {
            y += (y1>y0?1:-1);
            error2 -= dx*2;
        }
    }
}

int Pipeline::homogeneous_clipping(payloadInfo &payload) {
    int number_vertex = 3;
    number_vertex = clip_with_plane(W_PLANE,number_vertex,payload);
    number_vertex = clip_with_plane(X_LEFT,number_vertex,payload);
    number_vertex = clip_with_plane(X_RIGHT,number_vertex,payload);
    number_vertex = clip_with_plane(Y_TOP,number_vertex,payload);
    number_vertex = clip_with_plane(Y_BOTTOM,number_vertex,payload);
    number_vertex = clip_with_plane(Z_NEAR,number_vertex,payload);
    number_vertex = clip_with_plane(Z_FAR,number_vertex,payload);
    return number_vertex;
}

int Pipeline::clip_with_plane(clip_plane plane, int number_vertex, payloadInfo &payload) {
    int out_vertex_number = 0;
    int previous_index,current_index;
    int is_odd = (plane+1)%2;

    //set the right in and out data
    Vector4f* in_clip = is_odd ? payload.in_clip : payload.out_clip;
    Vector4f* in_world = is_odd ? payload.in_world : payload.out_world;
    Vector3f* in_normal = is_odd ? payload.in_normal : payload.out_normal;
    Vector2f* in_uv = is_odd ? payload.in_uv : payload.out_uv;
    Vector4f* out_clip = is_odd ? payload.out_clip : payload.in_clip;
    Vector4f* out_world = is_odd ? payload.out_world : payload.in_world;
    Vector3f* out_normal = is_odd ? payload.out_normal : payload.in_normal;
    Vector2f* out_uv = is_odd ? payload.out_uv : payload.in_uv;

    //遍历所有的边并判断是否在范围内
    for (int i = 0; i < number_vertex; ++i) {
        // couple((0,number_vertex-1),(1,0),(2,1),...,(number_vertex-1,number_vertex-2))
        current_index = i;
        previous_index = (i-1+number_vertex)%number_vertex;
        Vector4f current_vertex = in_clip[current_index];
        Vector4f previous_vertex = in_clip[previous_index];
        //当前顶点是否在给定平面内部
        int is_cur_inside = is_inside_plane(plane,current_vertex);
        //上一个顶点是否在给定平面内部
        int is_pre_inside = is_inside_plane(plane,previous_vertex);
        //表示当前点和上一个点中有一个落在了内部，而另一个落在了外部，因此需要求两顶点的连线与平面的交点
        if (is_cur_inside!=is_pre_inside){
            float ratio = get_intersect_ratio(previous_vertex,current_vertex,plane);//求插值
            //利用插值将新的顶点信息保存下来
            out_clip[out_vertex_number] = vector4f_lerp(previous_vertex,current_vertex,ratio);
            out_uv[out_vertex_number] = vector2f_lerp(in_uv[previous_index],in_uv[current_index],ratio);
            out_normal[out_vertex_number] = vector3f_lerp(in_normal[previous_index],in_normal[current_index],ratio);
            out_world[out_vertex_number] = vector4f_lerp(in_world[previous_index],in_world[current_index],ratio);
            out_vertex_number++;//新创建了一个顶点
        }
        //这里表示两个顶点全部落在了内测，即当前顶点不需要丢弃，保留下来即可
        if (is_cur_inside){
            //将当前顶点的信息保存下来
            out_clip[out_vertex_number] = current_vertex;
            out_world[out_vertex_number] = in_world[current_index];
            out_uv[out_vertex_number] = in_uv[current_index];
            out_normal[out_vertex_number] = in_normal[current_index];
            out_vertex_number++;//顶点数量+1
        }
    }
    return out_vertex_number;
}

int Pipeline::is_inside_plane(clip_plane plane, Vector4f vertex) {
    switch (plane) {
        case W_PLANE:
            return vertex.w >= EPSILON;
        case X_LEFT:
            return vertex.x >= -vertex.w;
        case X_RIGHT:
            return vertex.x <= +vertex.w;
        case Y_TOP:
            return vertex.y <= +vertex.w;
        case Y_BOTTOM:
            return vertex.y >= -vertex.w;
        case Z_NEAR:
            return vertex.z <= +vertex.w;
        case Z_FAR:
            return vertex.z >= -vertex.w;
        default:
            return 0;
    }
}

/**
 * calculate the intersect_ratio
 *  intersection : I = Q1 + t<Q2-Q1>
 *  https://fabiensanglard.net/polygon_codec/clippingdocument/Clipping.pdf
 * @param pre pre_vertex
 * @param cur cur_vertex
 * @param plane the given plane
 * @return ratio
 */
float Pipeline::get_intersect_ratio(Vector4f pre, Vector4f cur, clip_plane plane) {
    switch (plane) {
        case W_PLANE:
            return (pre.w - EPSILON) / (pre.w - cur.w);
        case X_LEFT:
            return (pre.w + pre.x) / ((pre.w + pre.x) - (cur.w + cur.x));//I.x = -I.w
        case X_RIGHT:
            return (pre.w - pre.x) / ((pre.w - pre.x) - (cur.w - cur.x));//I.x = I.w
        case Y_TOP:
            return (pre.w - pre.y) / ((pre.w - pre.y) - (cur.w - cur.y));//I.y = I.w
        case Y_BOTTOM:
            return (pre.w + pre.y) / ((pre.w + pre.y) - (cur.w + cur.y));//I.y = -I.w
        case Z_NEAR:
            return (pre.w - pre.z) / ((pre.w - pre.z) - (cur.w - cur.z));//I.z = I.w
        case Z_FAR:
            return (pre.w + pre.z) / ((pre.w + pre.z) - (cur.w + cur.z));//I.z = -I.w
        default:
            return 0;
    }
}

void Pipeline::transform_attribute(Shader& shader, int index0, int index1, int index2) {
    int index[3] = {index0,index1,index2};
    for (int i = 0; i < 3; ++i) {
        //vertex_clip_coord
        shader.payload.out_vertex_clipPos[i] =  shader.payload.out_clip[index[i]];
        //vertex_normal_dir
        shader.payload.normal_dir[i] =  shader.payload.out_normal[index[i]];
        //vertex_uv_coord
        shader.payload.uv_pos[i] =  shader.payload.out_uv[index[i]];
        //vertex_pos
        shader.payload.vertex_pos[i] =  shader.payload.out_world[index[i]];
        //tangent_space_information_for_calculating_lighting
        if (typeid(shader).name() == typeid(BlinnPhongTangentShader).name()){
            mat3 normal_matrix = mat3_from_mat4( shader.payload.model_matrix);
            Vector3f t =  shader.payload.tangent_dir;
            Vector3f vertex_normal =  shader.payload.normal_dir[i];
            Vector3f temp(vertex_normal * (t * vertex_normal));
            Vector3f ortho_T = (t - temp).normalized();
            Vector3f T = mat3_mul_vec3(normal_matrix,ortho_T).normalized();
            Vector3f N = mat3_mul_vec3(normal_matrix,vertex_normal).normalized();
            Vector3f B = (N^T).normalized();
            B = mat3_mul_vec3(normal_matrix,B).normalized();
            mat3 TBN = mat3_from_3_vector3f(T,B,N);
            //将计算光照的相关信息从世界空间转换到切线空间中去
            Vector3f tangent_eye_pos = mat3_mul_vec3(TBN, shader.payload.camera->getEye());
            Vector3f tangent_light_pos = mat3_mul_vec3(TBN, shader.payload.point_light.position);
            Vector3f tangent_vertex_pos = mat3_mul_vec3(TBN,Vector3f( shader.payload.vertex_pos[i].x,
                                                                      shader.payload.vertex_pos[i].y,
                                                                      shader.payload.vertex_pos[i].z));
            shader.payload.tangent_vertex_pos[i] = tangent_vertex_pos;
            shader.payload.tangent_eye_pos[i] = tangent_eye_pos;//每个顶点的TBN矩阵不一样，因此对应的属性也不同
            shader. payload.tangent_point_light_pos[i] = tangent_light_pos;
        }
    }
}

//see https://www.khronos.org/registry/OpenGL/specs/es/2.0/es_full_spec_2.0.pdf
// is equal to cross(ab,ac).z()<=0
bool Pipeline::is_back_culling(Vector3f ndc_pos[3]) {
    Vector3f a = ndc_pos[0];
    Vector3f b = ndc_pos[1];
    Vector3f c = ndc_pos[2];
    float signed_area = a.x * b.y - a.y * b.x +
                        b.x * c.y - b.y * c.x +
                        c.x * a.y - c.y * a.x;
    return signed_area <= 0;
}

void Pipeline::enable_back_face_culling() {
    back_face_culling = !back_face_culling;
}

Pipeline::Pipeline() = default;
