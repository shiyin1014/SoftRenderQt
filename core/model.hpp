//
// Created by dushi on 2023/1/26.
//

#ifndef SOFTRENDERERQT_MODEL_HPP
#define SOFTRENDERERQT_MODEL_HPP

#include <fstream>
#include <sstream>
#include <iostream>
#include "vector"
#include "maths.h"
#include "opencv2/opencv.hpp"
#include "triangle.h"

enum {
   DIFFUSE,
   SPECULAR,
   NORMAL
};

class Model{

public:
    std::vector<Vector3f> vertices;//模型的所有顶点坐标数据
    std::vector<Vector2f> tex_coords;//顶点对应的纹理坐标数据
    std::vector<Vector3f> normals;//顶点的法向量
    std::vector<std::vector<Vector3i>> faces;//面

    std::vector<Triangle*> triangleList;//将加载的数据解析为自定义的三角形数据加载进来

    std::vector<unsigned int > indices;//顶点序号

    cv::Mat* diffuse_texture{};
    cv::Mat* specular_texture{};
    cv::Mat* normal_tangent_texture{};

    static int get_real_texture_name(const std::string& name){
        if (name.find("diffuse")!=std::string::npos)return DIFFUSE;
        if (name.find("specular")!=std::string::npos)return SPECULAR;
        if (name.find("normal")!=std::string::npos)return NORMAL;
        return -1;
    }

    //纹理加载
    void load_texture(const std::vector<std::string>& filename){
        for (const auto & i : filename) {
            if (cv::imread(i).data!= nullptr){
                int index = get_real_texture_name(i);
                if (index==DIFFUSE){
                    diffuse_texture = new cv::Mat(cv::imread(i));
                    cv::cvtColor(*diffuse_texture,*diffuse_texture,cv::COLOR_BGR2RGB);//转换为rgb顺序
                    std::cout<<"load_texture : "<<i<<"  finished."<<std::endl;
                }else if (index==SPECULAR){
                    specular_texture = new cv::Mat(cv::imread(i));
                    cv::cvtColor(*specular_texture,*specular_texture,cv::COLOR_BGR2RGB);//转换为rgb顺序
                    std::cout<<"load_texture : "<<i<<"  finished."<<std::endl;
                }else if (index==NORMAL){
                    normal_tangent_texture = new cv::Mat(cv::imread(i));
                    cv::cvtColor(*normal_tangent_texture,*normal_tangent_texture,cv::COLOR_BGR2RGB);//转换为rgb顺序
                    std::cout<<"load_texture : "<<i<<"  finished."<<std::endl;
                }
            }
        }
    }

    explicit Model(const std::string& filename){
        std::ifstream ifs;//输入文件流
        const std:: string& obj_filename = filename;
        ifs.open(obj_filename,std::ifstream::in);//打开obj文件，并以读的方式
        if (ifs.fail())return;
        std::string line;//用来接收每一行读取的数据
        while (!ifs.eof()){//读到文件结束符时返回true,即文件最后一个字符的下一个字符
            getline(ifs,line);//读取一行数据并保存在line中
            std::istringstream iss(line);//istringstream的目的是从字符串中按照空格分割字符串
            char trash;
            //判断该行是否为顶点坐标数据
            if (!line.compare(0,2,"v ")){//string.compare(index,length,str)函数比较从index开始长度为length的字串和str进行比较
                iss>>trash;//这里其实是将第一个字符’v‘给了trash这个变量
                Vector3f v;
                iss >>v.x;
                iss >>v.y;
                iss >>v.z;
                vertices.push_back(v);
            } else if (!line.compare(0,3,"vn ")){//顶点的法向量
                iss>>trash>>trash;
                Vector3f normal;
                iss>>normal.x;
                iss>>normal.y;
                iss>>normal.z;
                normals.push_back(normal);
            } else if (!line.compare(0,3,"vt ")){//顶点的纹理坐标
                iss>>trash>>trash;
                Vector2f tex_coord;
                iss>>tex_coord.x;
                iss>>tex_coord.y;
                tex_coord.y = 1-tex_coord.y;//由于纹理坐标轴从左下角开始，而屏幕空间以及图片起始点在左上角，
                // 所以上下颠倒了，因此这里进行y轴的反转，或者再使用的时候再减去也可以，但一般都是加载模型时减去
                tex_coords.push_back(tex_coord);
            } else if (!line.compare(0,2,"f ")){//面face
//                int v,t,n;//f v/t/n 分别对应顶点索引/纹理索引/法线索引
                iss>>trash;//先把字符f扔掉
                int count = 0;
                Vector3i face;
                std::vector<Vector3i> f;
                while (iss>>face.x>>trash>>face.y>>trash>>face.z){
                    face.x--;
                    face.y--;
                    face.z--;
                    f.push_back(face);
                    count++;
                }
                faces.push_back(f);
                if (count!=3){
                    std::cerr <<"Error: the obj file is supposed to be triangulated" << std::endl;
                    return;
                }
            }
        }

        std::cout<<"file_name : "<<filename<<std::endl;
        std::cout<<"vertex_number = "<<vertices.size()<<std::endl;
        std::cout<<"vertex_normal_number = "<<normals.size()<<std::endl;
        std::cout<<"vertex_tex_coord_number = "<<tex_coords.size()<<std::endl;
        std::cout<<"faces_number = "<<faces.size()<<std::endl;
        load_triangleList();
    }

    void load_triangleList(){
        triangleList.clear();
        for (int i = 0; i < this->number_faces(); ++i) {
            std::vector<Vector3i> face = this->faces[i];//face : v/t/n 分别对应：顶点索引/纹理索引/法线索引 即face.x/y/z依次对应
            auto* triangle = new Triangle();
            for (int j = 0; j < 3; ++j) {
                //三角形顶点坐标
                triangle->setVertex(j,Vector4f(this->vertices[face[j].x].x,
                                               this->vertices[face[j].x].y,
                                               this->vertices[face[j].x].z,1.0f));
                //顶点法线
                triangle->setNormal(j,this->normals[face[j].z]);
                //顶点纹理坐标
                triangle->setTexCoords(j,this->tex_coords[face[j].y]);
            }
            triangleList.push_back(triangle);//加入集合
        }
    }

    Model()= default;

    ~Model(){
        delete this->diffuse_texture;
        delete this->specular_texture;
        delete this->normal_tangent_texture;
    };

    [[nodiscard]] int number_vertices() const{
        return (int )vertices.size();
    }

    [[nodiscard]] int number_faces() const{
        return (int )faces.size();
    }

};

#endif //SOFTRENDERERQT_MODEL_HPP
