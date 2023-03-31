//
// Created by dushi on 2023/3/25.
//

#include "scene.h"

void scene::load_scene_by_name(const std::string &file_name,Model** model,int&m,int& vertex_number,int& face_number) {
    if (file_name=="african_head"){
        load_african_head_scene(model,m,vertex_number,face_number);
    }
    if (file_name=="elfgirl"){
        load_elf_girl_scene(model,m,vertex_number,face_number);
    }
    if (file_name=="vivi"){
        load_vivi_scene(model,m,vertex_number,face_number);
    }
    if (file_name=="witch"){
        load_witch_scene(model,m,vertex_number,face_number);
    }
}

void scene::load_african_head_scene(Model **model, int &m,int& vertex_number,int& face_number) {
    m = 1;
    std::string model_obj_name[]{"../obj/african_head/african_head.obj"};
    std::vector<std::vector<std::string>> model_obj_texture({{"../obj/african_head/african_head_diffuse.jpg",
                                                              "../obj/african_head/african_head_specular.jpg",
                                                              "../obj/african_head/african_head_normal.jpg"},});
    load_model_and_texture(model,m,vertex_number,face_number,model_obj_name,model_obj_texture);
}

void scene::load_elf_girl_scene(Model **model, int &m, int &vertex_number, int &face_number) {
    m = 7;
    std::string model_obj_name[]{"../obj/elfgirl/elfgirl_base.obj",
                                 "../obj/elfgirl/elfgirl_body0.obj",
                                 "../obj/elfgirl/elfgirl_body1.obj",
                                 "../obj/elfgirl/elfgirl_body2.obj",
                                 "../obj/elfgirl/elfgirl_face0.obj",
                                 "../obj/elfgirl/elfgirl_face1.obj",
                                 "../obj/elfgirl/elfgirl_hair.obj"};

    std::vector<std::vector<std::string>> model_obj_texture({{"../obj/elfgirl/elfgirl_base_diffuse.jpg"},
                                                             {"../obj/elfgirl/elfgirl_body_diffuse.jpg"},
                                                             {"../obj/elfgirl/elfgirl_body_diffuse.jpg"},
                                                             {"../obj/elfgirl/elfgirl_body_diffuse.jpg"},
                                                             {"../obj/elfgirl/elfgirl_face_diffuse.jpg"},
                                                             {"../obj/elfgirl/elfgirl_face_diffuse.jpg"},
                                                             {"../obj/elfgirl/elfgirl_hair_diffuse.jpg"}});

    load_model_and_texture(model,m,vertex_number,face_number,model_obj_name,model_obj_texture);
}

void scene::load_vivi_scene(Model **model, int &m, int &vertex_number, int &face_number) {
    m = 6;
    std::string model_obj_name[]{"../obj/vivi/vivi0.obj",
                                 "../obj/vivi/vivi1.obj",
                                 "../obj/vivi/vivi2.obj",
                                 "../obj/vivi/vivi3.obj",
                                 "../obj/vivi/vivi4.obj",
                                 "../obj/vivi/vivi5.obj",};

    std::vector<std::vector<std::string>> model_obj_texture({{"../obj/vivi/vivi_diffuse.jpg"},
                                                             {"../obj/vivi/vivi_diffuse.jpg"},
                                                             {"../obj/vivi/vivi_diffuse.jpg"},
                                                             {"../obj/vivi/vivi_diffuse.jpg"},
                                                             {"../obj/vivi/vivi_diffuse.jpg"},
                                                             {"../obj/vivi/floor_diffuse.jpg"},});

    load_model_and_texture(model,m,vertex_number,face_number,model_obj_name,model_obj_texture);
}

void scene::load_witch_scene(Model **model, int &m, int &vertex_number, int &face_number) {
    m = 2;
    std::string model_obj_name[]{"../obj/witch/witch.obj",
                                 "../obj/witch/object.obj"};

    std::vector<std::vector<std::string>> model_obj_texture({{"../obj/witch/witch_diffuse.jpg"},
                                                             {"../obj/witch/object_diffuse.jpg"}});

    load_model_and_texture(model,m,vertex_number,face_number,model_obj_name,model_obj_texture);
}


void
scene::load_model_and_texture(Model **model, int &m, int &vertex_number, int &face_number,const std::string *model_obj_name,
                              const std::vector<std::vector<std::string>> &model_obj_texture) {
    std::cout<<"========================================load model start"<<std::endl;
    for (int i = 0; i < m; ++i) {
        std::cout<<"loading the "<< i<<"-th model_obj------------>>>"<<std::endl;
        model[i] = new Model(model_obj_name[i]);
        model[i]->load_texture(model_obj_texture[i]);
        vertex_number+=model[i]->number_vertices();
        face_number+=model[i]->number_faces();
        std::cout<<"the "<<i<<"-th model_obj load finished-------->>>"<<std::endl;
    }
    std::cout<<"total_vertex_number = "<<vertex_number<<std::endl;
    std::cout<<"total_face_number = "<<face_number<<std::endl;
    std::cout<<"========================================load model finished"<<std::endl;
}







