//
// Created by dushi on 2023/3/21.
//

#ifndef SOFTRENDERQT_MYLABEL_H
#define SOFTRENDERQT_MYLABEL_H
#include "QLabel"
#include "opencv2/opencv.hpp"
#include "QMouseEvent"
#include "core/camera.h"
#include "core/triangle.h"
#include "core/model.hpp"
#include "shader/shader.h"
#include "core/pipeline.h"
#include <QWheelEvent>
#include "core/scene.h"
#include "core/macro.h"

class myLabel : public QLabel{
    Q_OBJECT
public:

    explicit myLabel(QWidget *parent = nullptr);
    ~myLabel() override;

    void show_label();
    void update_frame();
    void update_matrix();
    void set_shading_model(const std::string& name);
    void update_fov(const int& _fov);
    void load_scene(const std::string& model_name);
    int get_total_vertex_number() const;
    int get_total_face_number() const;
    void change_model(const std::string& model_name);
    void show_depth_buffer();
    void show_wire_frame();
    void enable_face_culling();
    std::string get_string_cam_pos();

    void mousePressEvent(QMouseEvent *ev) override;
    void mouseMoveEvent(QMouseEvent *ev) override;
    void mouseReleaseEvent(QMouseEvent *ev) override;
    void enterEvent(QEnterEvent* event) override;
    void leaveEvent(QEvent* event) override;
    void wheelEvent(QWheelEvent *event)override;

    signals:
    void get_cam_pos(std::string pos);

private:
    bool on_mouse_press_left;
    bool on_mouse_press_middle{};
    float move_speed;
    float pre_mouse_x;
    float pre_mouse_y;
    float cur_mouse_x{};
    float cur_mouse_y{};
    cv::Mat image;
    Vector3f lightPos{3, 3, 3};
    Vector3f up{0,1,0};
    float near = 0.1f;
    float far = 1000.0f;
    Model* model[10]{};
    Camera* camera;
    mat4 model_matrix{};
    mat4 view_matrix{};
    mat4 perspective_matrix{};
    mat4 mvp_matrix{};
    float fov = 60.0f;
    std::string filename;
    Shader *shader{};
    QImage qImage;
    Pipeline* pipeline{};
    int model_number = 0;
    int vertex_number = 0;
    int face_number = 0;
    bool show_depth = false;
    bool wire_frame = false;
    bool back_culling = false;
};


#endif //SOFTRENDERQT_MYLABEL_H
