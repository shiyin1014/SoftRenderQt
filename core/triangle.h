//
// Created by dushi on 2023/1/26.
//

#ifndef SOFTRENDERERQT_TRIANGLE_H
#define SOFTRENDERERQT_TRIANGLE_H

#include "vector"
#include "maths.h"

class Triangle {
public:

    Vector4f v[3];//顶点坐标
    Vector3f normal[3];//顶点的法向量
    Vector2f tex_coords[3];//顶点纹理坐标
    Vector3f color[3];//顶点颜色数据

    Triangle();

    void setVertex(int index,Vector4f vector4F);
    void setNormal(int index, Vector3f n);
    void setTexCoords(int index,Vector2f tex);
    void setColor(int index, Vector3f rgb);

    void setVertices(std::vector<Vector3f>& vertices);
    void setNormals(std::vector<Vector3f>& normals);

};


#endif //SOFTRENDERERQT_TRIANGLE_H
