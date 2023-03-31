/********************************************************************************
** Form generated from reading UI file 'viewclass.ui'
**
** Created by: Qt User Interface Compiler version 6.4.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VIEWCLASS_H
#define UI_VIEWCLASS_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QWidget>
#include "myLabel.h"

QT_BEGIN_NAMESPACE

class Ui_viewClass
{
public:
    myLabel *label;
    QSlider *fov;
    QGroupBox *groupBox;
    QRadioButton *head;
    QRadioButton *girl;
    QRadioButton *witch;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *vertex_number;
    QLabel *face_number;
    QGroupBox *groupBox_2;
    QRadioButton *blinnphong;
    QRadioButton *tangent;
    QRadioButton *wireframe;
    QRadioButton *depth;
    QCheckBox *faceCulling;
    QLabel *label_4;
    QLabel *fovValue;
    QLabel *label_5;
    QLabel *cameraPos;

    void setupUi(QWidget *viewClass)
    {
        if (viewClass->objectName().isEmpty())
            viewClass->setObjectName("viewClass");
        viewClass->resize(800, 600);
        label = new myLabel(viewClass);
        label->setObjectName("label");
        label->setGeometry(QRect(260, 70, 500, 500));
        fov = new QSlider(viewClass);
        fov->setObjectName("fov");
        fov->setGeometry(QRect(30, 440, 121, 16));
        fov->setOrientation(Qt::Horizontal);
        groupBox = new QGroupBox(viewClass);
        groupBox->setObjectName("groupBox");
        groupBox->setGeometry(QRect(260, 10, 491, 41));
        head = new QRadioButton(groupBox);
        head->setObjectName("head");
        head->setGeometry(QRect(50, 10, 95, 20));
        girl = new QRadioButton(groupBox);
        girl->setObjectName("girl");
        girl->setGeometry(QRect(200, 10, 95, 20));
        witch = new QRadioButton(groupBox);
        witch->setObjectName("witch");
        witch->setGeometry(QRect(350, 10, 95, 20));
        label_2 = new QLabel(viewClass);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(30, 50, 101, 21));
        label_3 = new QLabel(viewClass);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(30, 90, 101, 21));
        vertex_number = new QLabel(viewClass);
        vertex_number->setObjectName("vertex_number");
        vertex_number->setGeometry(QRect(140, 50, 81, 21));
        face_number = new QLabel(viewClass);
        face_number->setObjectName("face_number");
        face_number->setGeometry(QRect(140, 90, 81, 21));
        groupBox_2 = new QGroupBox(viewClass);
        groupBox_2->setObjectName("groupBox_2");
        groupBox_2->setGeometry(QRect(20, 140, 151, 261));
        blinnphong = new QRadioButton(groupBox_2);
        blinnphong->setObjectName("blinnphong");
        blinnphong->setGeometry(QRect(20, 20, 95, 20));
        tangent = new QRadioButton(groupBox_2);
        tangent->setObjectName("tangent");
        tangent->setGeometry(QRect(20, 70, 95, 20));
        wireframe = new QRadioButton(groupBox_2);
        wireframe->setObjectName("wireframe");
        wireframe->setGeometry(QRect(20, 120, 95, 20));
        depth = new QRadioButton(groupBox_2);
        depth->setObjectName("depth");
        depth->setGeometry(QRect(20, 170, 95, 20));
        faceCulling = new QCheckBox(groupBox_2);
        faceCulling->setObjectName("faceCulling");
        faceCulling->setGeometry(QRect(20, 220, 91, 20));
        label_4 = new QLabel(viewClass);
        label_4->setObjectName("label_4");
        label_4->setGeometry(QRect(50, 410, 31, 31));
        fovValue = new QLabel(viewClass);
        fovValue->setObjectName("fovValue");
        fovValue->setGeometry(QRect(90, 410, 31, 31));
        label_5 = new QLabel(viewClass);
        label_5->setObjectName("label_5");
        label_5->setGeometry(QRect(30, 500, 61, 31));
        cameraPos = new QLabel(viewClass);
        cameraPos->setObjectName("cameraPos");
        cameraPos->setGeometry(QRect(100, 500, 131, 31));

        retranslateUi(viewClass);

        QMetaObject::connectSlotsByName(viewClass);
    } // setupUi

    void retranslateUi(QWidget *viewClass)
    {
        viewClass->setWindowTitle(QCoreApplication::translate("viewClass", "SoftRender", nullptr));
        label->setText(QCoreApplication::translate("viewClass", "Image", nullptr));
        groupBox->setTitle(QString());
        head->setText(QCoreApplication::translate("viewClass", "head", nullptr));
        girl->setText(QCoreApplication::translate("viewClass", "girl", nullptr));
        witch->setText(QCoreApplication::translate("viewClass", "witch", nullptr));
        label_2->setText(QCoreApplication::translate("viewClass", "vertex number : ", nullptr));
        label_3->setText(QCoreApplication::translate("viewClass", "face number : ", nullptr));
        vertex_number->setText(QCoreApplication::translate("viewClass", "vertex_number", nullptr));
        face_number->setText(QCoreApplication::translate("viewClass", "face_number", nullptr));
        groupBox_2->setTitle(QString());
        blinnphong->setText(QCoreApplication::translate("viewClass", "blinnphong", nullptr));
        tangent->setText(QCoreApplication::translate("viewClass", "tangent", nullptr));
        wireframe->setText(QCoreApplication::translate("viewClass", "wire frame", nullptr));
        depth->setText(QCoreApplication::translate("viewClass", "depth", nullptr));
        faceCulling->setText(QCoreApplication::translate("viewClass", "face culling", nullptr));
        label_4->setText(QCoreApplication::translate("viewClass", "fov\357\274\232", nullptr));
        fovValue->setText(QCoreApplication::translate("viewClass", "fov", nullptr));
        label_5->setText(QCoreApplication::translate("viewClass", "cam_pos :", nullptr));
        cameraPos->setText(QCoreApplication::translate("viewClass", "cameraPos", nullptr));
    } // retranslateUi

};

namespace Ui {
    class viewClass: public Ui_viewClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VIEWCLASS_H
