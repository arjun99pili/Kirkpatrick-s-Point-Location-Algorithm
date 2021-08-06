#ifndef View_H
#define View_H
#pragma once

// Include GLEW
#include <GL/glew.h>

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <iostream>

#include"shader_s.hpp"


using namespace glm;

class View
{
    public:
        GLuint programID;
        GLFWwindow* window;
        int width, height;
        View();

        mat4 viewMatrix;
        mat4 projectionMatrix;

        mat4 getProjection()
        {
            return projectionMatrix;
        }
        mat4 getView()
        {
            return viewMatrix;
        }

        void setView(mat4 matrix)
        {
            viewMatrix = matrix;
        }
        void setProjection(mat4 matrix)
        {
            projectionMatrix = matrix;
        }
        void setShader(Shader ourShader);



};


#endif