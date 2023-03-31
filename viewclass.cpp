//
// Created by dushi on 2023/3/18.
//

// You may need to build the project (run Qt uic code generator) to get "ui_viewClass.h" resolved

#include "viewclass.h"
#include "ui_viewClass.h"

viewClass::viewClass(QWidget *parent) :
        QWidget(parent), ui(new Ui::viewClass) {
    ui->setupUi(this);
    setWindowFlags(windowFlags()&~Qt::WindowMinMaxButtonsHint);
    setFixedSize(this->width(),this->height());
    ui->blinnphong->setChecked(true);
    ui->fov->setValue(60);
    ui->fovValue->setText(QString::number(60));
    ui->fov->setMinimum(45);
    ui->fov->setMaximum(90);
    ui->head->setChecked(true);
    ui->label->show_label();
    ui->vertex_number->setText(QString::number(ui->label->get_total_vertex_number()));
    ui->face_number->setText(QString::number(ui->label->get_total_face_number()));
    ui->cameraPos->setText(QString::fromStdString(ui->label->get_string_cam_pos()));
    connect(ui->blinnphong,&QRadioButton::clicked,[=](){
        ui->label->set_shading_model("blinn_phong");
        qDebug()<<"blinn_phong without normal mapping";
    });
    connect(ui->tangent,&QRadioButton::clicked,[=](){
        ui->label->set_shading_model("tangent");
        qDebug()<<"blinn_phong with normal mapping";
    });
    connect(ui->wireframe,&QRadioButton::clicked,[=](){
        ui->label->show_wire_frame();
       qDebug()<<"wireframe";
    });
    connect(ui->fov,&QSlider::sliderMoved,[this](int value){
        ui->label->update_fov(value);
        ui->fovValue->setText(QString::number(value));
        qDebug()<<"change fov value as "<<value;
    });
    connect(ui->head,&QRadioButton::clicked,[=](){
        ui->tangent->setVisible(true);
        ui->blinnphong->setChecked(true);
        ui->label->change_model("african_head");
        ui->vertex_number->setText(QString::number(ui->label->get_total_vertex_number()));
        ui->face_number->setText(QString::number(ui->label->get_total_face_number()));
       qDebug()<<"change model to african_head";
    });
    connect(ui->girl,&QRadioButton::clicked,[=](){
        ui->label->change_model("elfgirl");
        ui->tangent->setVisible(false);
        ui->blinnphong->setChecked(true);
        ui->vertex_number->setText(QString::number(ui->label->get_total_vertex_number()));
        ui->face_number->setText(QString::number(ui->label->get_total_face_number()));
        qDebug()<<"change model to elfgirl";
    });
    connect(ui->witch,&QRadioButton::clicked,[=](){
        ui->label->change_model("witch");
        ui->tangent->setVisible(false);
        ui->blinnphong->setChecked(true);
        ui->vertex_number->setText(QString::number(ui->label->get_total_vertex_number()));
        ui->face_number->setText(QString::number(ui->label->get_total_face_number()));
        qDebug()<<"change model to witch";
    });
    connect(ui->depth,&QRadioButton::clicked, [=](){
        ui->label->show_depth_buffer();
       qDebug("draw depth_buffer");
    });
    connect(ui->faceCulling,&QCheckBox::clicked,[=](){
        ui->label->enable_face_culling();
    });
    connect(ui->label,&myLabel::get_cam_pos, [this](const std::string& pos){
        this->ui->cameraPos->setText(QString::fromStdString(pos));
    });
}

viewClass::~viewClass() {
    delete ui;
}



