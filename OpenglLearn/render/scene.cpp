//
//  scene.cpp
//  OpenglLearn
//
//  Created by liuhaifeng02 on 2024/1/25.
//

#include "scene.hpp"
#include <glm/glm.hpp>
#include <vector>
#include "model/model.hpp"
#include "shape/ImageRectangle.hpp"
#include "shape/Line.hpp"
#include "image.hpp"
#include "Light.h"

const GLuint SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

Scene& Scene::getScene() {
    static Scene instance;
    return instance;
}

Scene::Scene() {
    glGenFramebuffers(1, &_depthMapFBO);
    glGenTextures(1, &_depthTexture);
    glBindTexture(GL_TEXTURE_2D, _depthTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 
                 SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    createObjs();
}

void Scene::createObjs() {
    // 绘制坐标轴
    std::vector<unsigned int> indices = {0, 1};
    
    float len = 15;
    glm::vec3 pZero = {0, 0, 0};
    glm::vec3 pX = {len, 0, 0};
    glm::vec3 pY = {0, len, 0};
    glm::vec3 pZ = {0, 0, len};
    // 3个轴
    std::shared_ptr<Line> line_x = std::make_shared<Line>();
    std::shared_ptr<Line> line_y = std::make_shared<Line>();
    std::shared_ptr<Line> line_z = std::make_shared<Line>();
    line_x->setData({pZero, pX}, indices);
    line_y->setData({pZero, pY}, indices);
    line_z->setData({pZero, pZ}, indices);
    
    // 颜色
    line_x->setColor({1, 0, 0});
    line_y->setColor({0, 1, 0});
    line_z->setColor({0, 0, 1});
    
    
    // 地面
    std::shared_ptr<ImageRectangle> objGround = std::make_shared<ImageRectangle>();
    float ground_width = 10;
    objGround->setImagePath("/Users/liuhaifeng/personal/OpenglLearnMac/OpenglLearn/res/textures/", "bricks2.jpg", "bricks2_normal.jpg", "bricks2_disp.jpg");// bricks2_disp.jpg
    objGround->setSetp(5, 5);
    glm::vec3 p1(-ground_width, 0,  ground_width);
    glm::vec3 p2(ground_width, 0,  ground_width);
    glm::vec3 p3(ground_width, 0, -ground_width);
    glm::vec3 p4(-ground_width, 0, -ground_width);
    objGround->setPoints(p1, p2, p3, p4);
    objGround->calculate();
    
    // 模型
    std::shared_ptr<Model> objModel = std::make_shared<Model>();
    //objModel->LoadFile("/Users/liuhaifeng/personal/OpenglLearnMac/OpenglLearn/res/model/rp_nathan_animated_003_walking.fbx");
    //objModel->LoadFile("/Users/liuhaifeng/personal/OpenglLearnMac/OpenglLearn/res/model/OBJ/spider.obj");
    //objModel->LoadFile("/Users/liuhaifeng/personal/OpenglLearnMac/OpenglLearn/res/model/duck.dae");
    objModel->LoadFile("/Users/liuhaifeng/personal/OpenglLearnMac/OpenglLearn/res/model/backpack/backpack.obj");
    //objModel->LoadFile("/Users/liuhaifeng/personal/OpenglLearnMac/OpenglLearn/res/model/nanosuit/nanosuit.obj");
    objModel->setCount(1);
    objModel->setPosition(0, {0, 0.5, 0});
    objModel->setScale(0, 1);
    // 光源模型
    std::shared_ptr<Model> objLight = std::make_shared<Model>();
    objLight->LoadFile("/Users/liuhaifeng/personal/OpenglLearnMac/OpenglLearn/res/model/OBJ/box.obj");
    objLight->setCount(1);
    objLight->setLightOpen(false);
    auto lightPos = Light::GlobalLight().position;
    objLight->setPosition(0, {lightPos.x, lightPos.y, lightPos.z});
    objLight->setScale(0, 0.5);
    
    // debug deep
    std::shared_ptr<Image> objImage = std::make_shared<Image>();
    objImage->setTextureID(GetShadowTexture());
    objImage->setShaderType(ShaderType::Debug_DeepTexture);
    
    // push 
    m_vec_drawobj.push_back(objGround);
    //m_vec_drawobj.push_back(objModel);
    m_vec_drawobj.push_back(objLight);
    m_vec_drawobj.push_back(line_x);
    m_vec_drawobj.push_back(line_y);
    m_vec_drawobj.push_back(line_z);
    m_vec_drawobj.push_back(objImage);
}

void Scene::update() {
    for (auto obj : m_vec_drawobj) {
        obj->update();
    }
}

void Scene::draw() {
    glViewport(0, 0, SCR_WIDTH*2, SCR_HEIGHT*2);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    for (auto obj : m_vec_drawobj) {
        obj->draw();
    }
}

void Scene::drawShadow() {
    // attach到fbo
    glBindFramebuffer(GL_FRAMEBUFFER, _depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _depthTexture, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    // bind
    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, _depthMapFBO);
    glClear(GL_DEPTH_BUFFER_BIT);
    
    // draw
    for (auto obj : m_vec_drawobj) {
        obj->drawShadow();
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

Matrix Scene::GetLightVPMatrix() {
    if (_lightVPMatrix) {
        return *_lightVPMatrix;
    }
    Matrix lightProjection, lightView;
    glm::vec3 lightPos = Light::GlobalLight().position;
    float near_plane = 1.0f, far_plane = 15.f;
    float width = 10.f;
    
    lightView = Camera::LookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
    lightProjection = Camera::ortho(-width, width, -width, width, near_plane, far_plane);
    if (!_lightVPMatrix) {
        _lightVPMatrix = new Matrix;
        *_lightVPMatrix = lightProjection * lightView;
    }
    return *_lightVPMatrix;
}
