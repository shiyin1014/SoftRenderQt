//
// Created by dushi on 2023/1/26.
//

#include "triangle.h"

Triangle::Triangle(){
    this->v[0] = Vector4f();
    this->v[1] = Vector4f();
    this->v[2] = Vector4f();

    this->normal[0] = Vector3f();
    this->normal[1] = Vector3f();
    this->normal[2] = Vector3f();

    this->color[0] = Vector3f();
    this->color[1] = Vector3f();
    this->color[2] = Vector3f();

    this->tex_coords[0] = Vector2f();
    this->tex_coords[1] = Vector2f();
    this->tex_coords[2] = Vector2f();
}

void Triangle::setVertex(int index,Vector4f vector4F){
    v[index] = vector4F;
}
void Triangle::setNormal(int index, Vector3f n){
    normal[index] = n;
}
void Triangle::setTexCoords(int index,Vector2f tex){
    tex_coords[index] = tex;
}
void Triangle::setColor(int index, Vector3f rgb){
    color[index] = rgb;
}

void Triangle::setVertices(std::vector<Vector3f> &vertices) {
    for (int i = 0; i < vertices.size(); ++i) {
        setVertex(i,Vector4f(vertices[i].x,vertices[i].y,vertices[i].z,1));
    }
}

void Triangle::setNormals(std::vector<Vector3f> &normals) {
    for (int i = 0; i < normals.size(); ++i) {
        setNormal(i,normals[i]);
    }
}


