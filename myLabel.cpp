//
// Created by dushi on 2023/3/21.
//

#include "myLabel.h"

void myLabel::mousePressEvent(QMouseEvent *ev) {
    if (ev->button()==Qt::LeftButton){
        this->on_mouse_press_left  = true;
    }else if (ev->button()==Qt::MiddleButton){
        on_mouse_press_middle = true;
    }
    this->pre_mouse_x = (float )ev->pos().x();
    this->pre_mouse_y = (float )ev->pos().y();
}

void myLabel::mouseMoveEvent(QMouseEvent *ev) {
    this->cur_mouse_x = (float )ev->pos().x();
    this->cur_mouse_y = (float )ev->pos().y();
    float delta_x = cur_mouse_x - pre_mouse_x;
    float delta_y = cur_mouse_y - pre_mouse_y;
    pre_mouse_x = cur_mouse_x;
    pre_mouse_y = cur_mouse_y;
    if (this->on_mouse_press_left){
        //处理水平角度变化，改变方位角的大小
        if(delta_x!=0)this->camera->rotateAzimuth(-delta_x * move_speed);
        //处理垂直角度变化，改变极角的大小
        if (delta_y!=0)this->camera->rotatePolar(delta_y * move_speed);
        update_frame();
    }else if (this->on_mouse_press_middle){
        if (delta_x!=0)this->camera->moveHorizontal(delta_x*move_speed);
        if (delta_y!=0)this->camera->moveVertical(delta_y*move_speed);
        update_frame();
    }
}

void myLabel::mouseReleaseEvent(QMouseEvent *ev) {
    if (ev->button()==Qt::LeftButton){
        this->on_mouse_press_left = false;
    }else if (ev->button()==Qt::MiddleButton){
        this->on_mouse_press_middle = false;
    }
}

void myLabel::enterEvent(QEnterEvent *event) {

}

void myLabel::leaveEvent(QEvent *event) {

}

void myLabel::wheelEvent(QWheelEvent *event) {
    float degree = (float )event->angleDelta().y()/8.0f;
    this->camera->zoom(degree*0.01f);
    update_frame();
}

myLabel::~myLabel() = default;;

myLabel::myLabel(QWidget *parent) : QLabel(parent) {
    this->on_mouse_press_left = false;
    this->move_speed = 0.01f;
    this->pre_mouse_x = 0.0f;
    this->pre_mouse_y = 0.0f;
    this->model_matrix = mat4_identity();
    camera = new Camera(Vector3f(0),up,3,2,0,0);
    perspective_matrix = mat4_perspective(fov,(float )SCREEN_WIDTH/(float )SCREEN_HEIGHT,near,far);
    filename = "african_head";
    load_scene(filename);
}

void myLabel::show_label() {
    qImage = QImage(image.data, image.cols, image.rows, QImage::Format_RGB888);
    this->setPixmap(QPixmap::fromImage(qImage));
    this->resize(qImage.size());
    this->show();
}

void myLabel::update_frame() {
    update_matrix();
    pipeline = new Pipeline(SCREEN_WIDTH,SCREEN_HEIGHT);
    if (back_culling)pipeline->enable_back_face_culling();
    for (int i = 0; i < model_number; ++i) {
        shader->payload.model = model[i];
        if (wire_frame){
            pipeline->wire_frame(*shader);
        }else{
            pipeline->draw(*shader);
        }
    }
    if (show_depth){
        std::vector<float> depth_buffer = this->pipeline->get_depth_buffer();
        float min_value = *std::min_element(depth_buffer.begin(),depth_buffer.end());
        float max_value = 0.0f;
        for (float i : depth_buffer) {
            if (i!=std::numeric_limits<float>::infinity()&&i>max_value){
                max_value = i;
            }
        }
        for (float & i : depth_buffer) {
            float depth = i;
            if (depth != std::numeric_limits<float>::infinity()){
                depth-=min_value;
                depth/=(max_value-min_value);
                i = depth * 255.0f;
            }
        }
        image = cv::Mat(SCREEN_WIDTH,SCREEN_HEIGHT,CV_32FC1,depth_buffer.data());
        image.convertTo(image,CV_8UC1,1.0f);
        qImage = QImage(image.data, image.cols, image.rows, QImage::Format_Indexed8);//single channel
    }else{
        image = cv::Mat(SCREEN_WIDTH,SCREEN_HEIGHT,CV_32FC3,pipeline->get_color_buffer().data());
        image.convertTo(image,CV_8UC3,1.0f);
        qImage = QImage(image.data, image.cols, image.rows, QImage::Format_RGB888);//rgb channel
    }
    this->setPixmap(QPixmap::fromImage(qImage));
    emit get_cam_pos(get_string_cam_pos());
}

void myLabel::set_shading_model(const std::string &name) {
    this->show_depth = false;
    this->wire_frame = false;
    if (name=="blinn_phong"){
        shader = new BlinnPhongShader();
    }else if(name=="tangent"){
        shader = new BlinnPhongTangentShader();
    }
    shader->payload.perspective_matrix = perspective_matrix;
    shader->payload.model_matrix = model_matrix;
    shader->payload.point_light.position = lightPos;
    shader->payload.camera = camera;
    update_frame();
}

void myLabel::update_fov(const int &_fov) {
    this->fov = (float )_fov;
    this->perspective_matrix = mat4_perspective(fov,(float )SCREEN_WIDTH/(float )SCREEN_HEIGHT,near,far);
    update_frame();
}

void myLabel::update_matrix() {
    view_matrix = camera->getViewMatrix();
    mat4 mv = mat4_mul_mat4(view_matrix,model_matrix);
    mvp_matrix = mat4_mul_mat4(perspective_matrix,mv);
    shader->payload.mvp_matrix = mvp_matrix;
    shader->payload.model_matrix = model_matrix;
    shader->payload.view_matrix = view_matrix;
}

void myLabel::load_scene(const std::string& model_name) {
    // init data
    vertex_number = 0;
    face_number = 0;
    for (int i = 0; i < model_number; ++i) {
        model[i] = nullptr;
    }
    scene::load_scene_by_name(model_name,model,model_number,vertex_number,face_number);
    set_shading_model("blinn_phong");
}

int myLabel::get_total_vertex_number() const {
    return this->vertex_number;
}

int myLabel::get_total_face_number() const {
    return this->face_number;
}

void myLabel::change_model(const std::string &model_name) {
    filename = model_name;
    //re-setting the camera when change the model
    camera = new Camera(Vector3f(0),up,3,1,0,0);
    model_matrix = mat4_identity();
    if (filename=="elfgirl"){
        mat4 translation = mat4_translation(-0.07f,1.0f,-0.06f);
        mat4 scale = mat4_scale(0.05,0.05,0.05);
        mat4 rotate = mat4_rotate_x(-90);
        mat4 temp  = mat4_mul_mat4(rotate,scale);
        model_matrix = mat4_mul_mat4(translation,temp);
    }else if (filename=="vivi"){
        mat4 translation = mat4_translation(0,0.5f,0);
        mat4 scale = mat4_scale(0.65,0.65,0.65);
        mat4 rotate = mat4_rotate_x(-90);
        mat4 temp  = mat4_mul_mat4(rotate,scale);
        model_matrix = mat4_mul_mat4(translation,temp);
    }else if (filename=="witch"){
        mat4 translation = mat4_translation(0.7f,1.5f,-0.7f);
        mat4 scale = mat4_scale(0.05,0.05,0.05);
        mat4 rotate = mat4_rotate_x(-90);
        mat4 temp  = mat4_mul_mat4(rotate,scale);
        model_matrix = mat4_mul_mat4(translation,temp);
    }
    load_scene(model_name);
}

void myLabel::show_depth_buffer() {
    this->show_depth = true;
    this->wire_frame = false;
    update_frame();
}

void myLabel::show_wire_frame() {
    this->show_depth = false;
    this->wire_frame = true;
    update_frame();
}

void myLabel::enable_face_culling() {
    back_culling = !back_culling;
    if (back_culling)std::cout<<"back_culling enabled."<<std::endl;
    if(!back_culling)std::cout<<"back_culling disabled."<<std::endl;
    update_frame();
}

std::string myLabel::get_string_cam_pos() {
    std::string pos;
    Vector3f camera_pos = this->camera->getEye();
    std::string x = std::to_string(camera_pos.x);
    auto x_point_index = x.find('.');
    x = x.substr(0,x_point_index+4);
    std::string y = std::to_string(camera_pos.y);
    auto y_point_index = y.find('.');
    y = y.substr(0,y_point_index+4);
    std::string z = std::to_string(camera_pos.z);
    auto z_point_index = z.find('.');
    z = z.substr(0,z_point_index+4);
    pos = "("+x+" ,"+ y+" ,"+ z+")";
    return pos;
}
