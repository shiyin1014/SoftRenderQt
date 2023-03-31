//
// Created by dushi on 2023/3/19.
//

#ifndef SOFTRENDERQT_PIPELINE_H
#define SOFTRENDERQT_PIPELINE_H

#include "camera.h"
#include "maths.h"
#include "../shader/shader.h"
#include "triangle.h"
#include "opencv2/opencv.hpp"
#include "macro.h"
#include <cmath>


typedef enum{
    W_PLANE,
    X_LEFT,
    X_RIGHT,
    Y_TOP,
    Y_BOTTOM,
    Z_NEAR,
    Z_FAR
}clip_plane;


class Pipeline {
public:

    Pipeline();
    Pipeline(int w,int h);

    void clear_buffer();
    void draw(Shader& shader);
    void wire_frame(Shader& shader);
    void draw_line(const Vector3f& begin,const Vector3f& end);
    static int homogeneous_clipping(payloadInfo& payload);
    static int clip_with_plane(clip_plane plane,int number_vertex,payloadInfo& payload);
    static int is_inside_plane(clip_plane plane,Vector4f vertex);
    static float get_intersect_ratio(Vector4f pre,Vector4f cur, clip_plane plane);
    static void transform_attribute(Shader& shader,int index0,int index1,int index2);
    static bool is_back_culling(Vector3f ndc_pos[3]);
    void enable_back_face_culling();

    void set_depth_value(const Vector2i& coordinate, const float& depth);
    void set_pixel_color(const Vector2i& coordinate,const Vector3f& color);
    void rasterizer_triangle(const Vector3f screen_pos[],Shader& shader);

    std::vector<float> get_depth_buffer();
    std::vector<Vector3f>& get_color_buffer();
    [[nodiscard]] int get_index(int x,int y) const;


private:
    int width{};
    int height{};

    std::vector<float> depth_buffer;
    std::vector<Vector3f> color_buffer;
    bool back_face_culling = false;

};


#endif //SOFTRENDERQT_PIPELINE_H
