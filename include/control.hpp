#ifndef Control_H_
#define Control_H_

#pragma once
#define GLM_ENABLE_EXPERIMENTAL

// Include GLEW
#include <GL/glew.h>

// Include GLM

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/norm.hpp>


#include <GLFW/glfw3.h>
#include <iostream>


#include"model.hpp"
#include"utils.hpp"
#include"shader_s.hpp"

using namespace glm;

class Controller
{
    public :
        GLFWwindow* window;
        Model* model;
        int object_controlled, numOfObjects;

        //For rotation alone.
        int toRotate;
        double x1pos, y1pos, z1pos, x2pos, y2pos, z2pos;
        double curr_time, prev_time;
        float dist1, dist2, angle;
        vec3 ini_point, final_point, normal;

        Point points[3];
        int clicks[3];
        int count_points;

        int level;

        int state, testit;  //0 - input stage, 1 - point selection stage, 2 - output stage


    
        Controller(GLFWwindow* win, Model* mod);
        void readKeyInput();
        void readMouseInput();
        void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
        void setNumObj(int num)
        {
            numOfObjects = num;
        }


};



#endif