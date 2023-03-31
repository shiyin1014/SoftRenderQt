//
// Created by dushi on 2023/3/16.
//

#include "camera.h"

#include <cmath>
#include <iostream>

Camera::Camera() = default;

Camera::Camera(const Vector3f &center, const Vector3f &up_dir, float radius, float minRadius, float azimuthAngle,
               float polarAngle) {
    this->_center = center;
    this->_upVector = up_dir;
    this->_radius = radius;
    this->_minRadius = minRadius;
    this->_azimuthAngle = azimuthAngle;
    this->_polarAngle = polarAngle;
}

void Camera::rotateAzimuth(const float radians) {
    _azimuthAngle+=radians;
    //将方位角限定在0-2Pi之内
    const auto fullCircle = 2.0*M_PI;
    _azimuthAngle = fmodf(_azimuthAngle,(float )fullCircle);
    if(_azimuthAngle < 0.0f){
        _azimuthAngle = (float )fullCircle + _azimuthAngle;
    }
//    std::cout<<"_azimuthAngle : "<<_azimuthAngle<<std::endl;
}

void Camera::rotatePolar(const float radians) {
    _polarAngle+=radians;
    //将极角限定在0-Pi/2内
    const auto polarCap = M_PI/2.0f - 0.001f;
    if (_polarAngle > polarCap){
        _polarAngle = (float )polarCap;
    }
    if (_polarAngle < -polarCap){
        _polarAngle = -(float )polarCap;
    }
}

void Camera::zoom(const float by) {
    _radius-=by;
    if (_radius<=_minRadius){
        _radius = _minRadius;
    }
}

void Camera::moveHorizontal(const float distance) {
    const auto position = getEye();//获取当前坐标位置
    const Vector3f view_vector = getNormalizedViewVector();
    const Vector3f strafe_vector = (view_vector^_upVector).normalized();
    Vector3f temp = strafe_vector * distance;
    _center = _center + temp;
}

void Camera::moveVertical(const float distance) {
    Vector3f temp = _upVector * distance;
    _center = _center + temp;
}

mat4 Camera::getViewMatrix() const {
    return mat4_lookAt(getEye(),_center,_upVector);
}

Vector3f Camera::getEye() const {
    const auto sineAzimuth = std::sin(_azimuthAngle);
    const auto cosineAzimuth = std::cos(_azimuthAngle);
    const auto sinePolar = std::sin(_polarAngle);
    const auto cosinePolar = std::cos(_polarAngle);
    //将球体坐标转变为三维坐标
    const auto x = _center.x + _radius * cosinePolar * sineAzimuth;
    const auto y = _center.y + _radius * sinePolar;
    const auto z = _center.z + _radius * cosinePolar * cosineAzimuth;
    return {x,y,z};
}

Vector3f Camera::getViewPoint() const {
    return _center;
}

Vector3f Camera::getUpDir() const {
    return _upVector;
}

Vector3f Camera::getNormalizedViewVector() const  {
    Vector3f eye_pos = getEye();
    Vector3f target_pos = getViewPoint();
    Vector3f view_dir((eye_pos-target_pos).normalized());
    return view_dir;
}

float Camera::getAzimuthAngle() const {
    return _azimuthAngle;
}

float Camera::getPolarAngle() const {
    return _polarAngle;
}

float Camera::getRadius() const {
    return _radius;
}
