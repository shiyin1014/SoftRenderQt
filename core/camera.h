//
// Created by dushi on 2023/3/16.
//

#ifndef SOFTRENDERERQT_CAMERA_H
#define SOFTRENDERERQT_CAMERA_H

#include "maths.h"

class Camera {
public:
    Camera();
    Camera(const Vector3f& center,const Vector3f& up_dir, float radius,float minRadius,float azimuthAngle,float polarAngle);

    void rotateAzimuth(float radians);
    void rotatePolar(float radians);
    void zoom(float by);

    //这两个是改变中心位置的
    void moveHorizontal(float distance);
    void moveVertical(float distance);

    mat4 getViewMatrix()const;
    Vector3f getEye()const;
    Vector3f getViewPoint()const;//看向的点，即private中的_center变量
    Vector3f getUpDir()const;
    Vector3f getNormalizedViewVector() const;
    float getAzimuthAngle()const;
    float getPolarAngle()const;
    float getRadius()const;

private:
    Vector3f _center;//轨道相机球体的中心，即相机看向的点的坐标
    Vector3f _upVector;//相机的向上方向
    float _radius{};//轨道相机球体的半径
    float _minRadius{};//轨道相机球体的最小半径
    float _azimuthAngle{};//方位角---->与x轴正方向之间的夹角，当在x轴正方向上时候为0，方位角的范围可以任意大，即可以绕着y轴无限绕圈圈
    float _polarAngle{};//极角---->y轴为向上的轴，点p投影在zx平面的线段op'与线段op之间的夹角，极角限定在上下Pi/2以内
};


#endif //SOFTRENDERERQT_CAMERA_H
