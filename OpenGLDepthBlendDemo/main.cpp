//
//  main.cpp
//  OpenGLDepthBlendDemo
//
//  Created by apple on 2020/7/13.
//  Copyright © 2020 yinhe. All rights reserved.
//

#include <stdio.h>
#include "GLMatrixStack.h"
#include "GLFrame.h"
#include "GLFrustum.h"
#include "GLGeometryTransform.h"
#include <GLUT/GLUT.h>


GLFrame viewFrame; // 物体
GLFrame cameraFrame; // 观察者
GLFrustum viewFrustum; // 管擦着投影矩阵
GLTriangleBatch torusBatch; // 甜甜圈批次类
GLMatrixStack modelViewMatrix; // 模型视图矩阵
GLMatrixStack projectionMatrix; // 投影矩阵
GLGeometryTransform transformPipeline; // 集合变换管道
GLShaderManager shaderManager; // 着色器管理类


GLfloat vRed[] = { 0.1f, 0.3f, 0.8f, 1.0f };


void changeSize(int w, int h) {
    // 设置窗口
    glViewport(0, 0, w, h);
    
    // 创建透视投影
    viewFrustum.SetPerspective(35.0, float(w) / float(h), 1.0, 100.0);
    
    // 通过设置的投影方式获得投影矩阵，并将其存入投影矩阵中
    projectionMatrix.LoadMatrix(viewFrustum.GetProjectionMatrix());
}

void setupRC() {
    // 设置背景颜色
    glClearColor(0.3f, 0.3f, 0.3f, 1.0f );
    
    // 初始化着色器管理器
    shaderManager.InitializeStockShaders();
    
    // 从初始化渲染管线
    transformPipeline.SetMatrixStacks(modelViewMatrix, projectionMatrix);
    
    // 将相机向后移动7个单元：肉眼到物体之间的距离
    viewFrame.MoveForward(10);
    
    // 创建一个甜甜圈
    // void gltMakeTorus(GLTriangleBatch& torusBatch, GLfloat majorRadius, GLfloat minorRadius, GLint numMajor, GLint numMinor);
    // 参数1：GLTriangleBatch 容器帮助类
    // 参数2：外边缘半径
    // 参数3：内边缘半径
    // 参数4、5：主半径和从半径的细分单元数量
    gltMakeTorus(torusBatch, 1.0f, 0.3f, 52, 26);
    
    // 点的大小(方便点填充时,肉眼观察)
    glPointSize(4.0f);
}

void renderScene() {
    // 清理缓存区
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    
    // 开启背面剔除
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
    
    // 压栈
    modelViewMatrix.PushMatrix(viewFrame);
    
    // 使用默认光源着色器
    // 参数1：GLT_SHADER_DEFAULT_LIGHT 默认光源着色器
    // 参数2：模型视图矩阵
    // 参数3：投影矩阵
    // 参数4：基本颜色值
    shaderManager.UseStockShader(GLT_SHADER_DEFAULT_LIGHT, transformPipeline.GetModelViewMatrix(), transformPipeline.GetProjectionMatrix(), vRed);
    
    // 绘制
    torusBatch.Draw();
    
    // 出栈
    modelViewMatrix.PopMatrix();
    
    // 交换缓冲区
    glutSwapBuffers();
}

void specialKeys(int key, int x, int y) {
    if(key == GLUT_KEY_UP) {
        viewFrame.RotateWorld(m3dDegToRad(-5.0), 1.0f, 0.0f, 0.0f);
    }
    
    if(key == GLUT_KEY_DOWN) {
        viewFrame.RotateWorld(m3dDegToRad(5.0), 1.0f, 0.0f, 0.0f);
    }
    
    if(key == GLUT_KEY_LEFT) {
        viewFrame.RotateWorld(m3dDegToRad(-5.0), 0.0f, 1.0f, 0.0f);
    }
    
    if(key == GLUT_KEY_RIGHT) {
        viewFrame.RotateWorld(m3dDegToRad(5.0), 0.0f, 1.0f, 0.0f);
    }
    
    glutPostRedisplay();
}

int main(int argc, char* argv[]) {
    gltSetWorkingDirectory(argv[0]);
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_STENCIL);
    glutInitWindowSize(800, 600);
    glutCreateWindow("OpenGL渲染方式");
    
    
    glutReshapeFunc(changeSize);
    glutDisplayFunc(renderScene);
    glutSpecialFunc(specialKeys);
    
    
    GLenum status = glewInit();
    if (status != GLEW_OK) {
        printf("error: %s\n", glewGetString(status));
        return 1;
    }
    
    setupRC();
    
    glutMainLoop();
    return 0;
}
