//
// Created by dushi on 2023/3/25.
//

#ifndef SOFTRENDERQT_SCENE_H
#define SOFTRENDERQT_SCENE_H

#include <string>
#include "model.hpp"
#include "../shader/shader.h"
#include "../core/camera.h"

class scene {
public:
    static void load_scene_by_name(const std::string &file_name,Model** model,int&m,int& vertex_number,int& face_number);
    static void load_african_head_scene(Model** model,int&m,int& vertex_number,int& face_number);
    static void load_elf_girl_scene(Model** model,int&m,int& vertex_number,int& face_number);
    static void load_vivi_scene(Model** model,int&m,int& vertex_number,int& face_number);
    static void load_witch_scene(Model** model,int&m,int& vertex_number,int& face_number);
    static void load_model_and_texture(Model** model,int&m,int& vertex_number,int& face_number,
                                       const std::string model_obj_name[],
                                       const std::vector<std::vector<std::string>>& model_obj_texture);

};


#endif //SOFTRENDERQT_SCENE_H
