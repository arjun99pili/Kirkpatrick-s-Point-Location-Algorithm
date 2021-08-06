#include<bits/stdc++.h>

#include<GL/glew.h>
#include<GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include"../include/shader_s.hpp"
#include"../include/model.hpp"
#include"../include/control.hpp"
#include"../include/view.hpp"

#include "glm/ext.hpp" 


using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void handleMouse_callback_func(GLFWwindow* window, int button, int action,int mods)
{
    static_cast<Controller*>(glfwGetWindowUserPointer(window))->mouse_button_callback(window, button, action, mods);
}


int main()
{

    Model *model = new Model();
    
 /*   Tlevel *level = new Tlevel();
    level->triangles.push_back(t1);
    level->triangles.push_back(t2);
    level->triangles.push_back(t3);
    level->triangles.push_back(t4);

    model->levels.push_back(level);

    model->createIS(level);
    model->deleteIS(level);
*/
    glfwInit();
    View scene;
    glfwSetFramebufferSizeCallback(scene.window, framebuffer_size_callback);


/* for(int i=0; i<model->levels.size(); i++)
    {
        cout<<"level "<<i<<endl;
        for(int j=0 ; j<model->levels[i]->triangles.size();j++)
        {
            model->levels[i]->triangles[j]->printTriangle();
            for(int k=0; k<model->levels[i]->triangles[j]->children.size();k++)
            {
                cout<<j<<" "<<k<<endl;
                model->levels[i]->triangles[j]->children[k]->printTriangle();
            }
        }
       // model->levels[i]->print_level();

    }
   
*/
   // Triangle * t = model->searchForpoint(-0.5f, 0.3f);
   // cout<<"printing..."<<endl;
   // t->printTriangle(); 
    //build and compile shader program
    Shader ourShader("./shaderfiles/vertexshader.vs", "./shaderfiles/fragshader.fs");
    scene.setShader(ourShader);


    
    // activate shader
    ourShader.use();
    Controller ctrl(scene.window, model);

    while(!glfwWindowShouldClose(scene.window))
    {
        glClearColor(0.0f, 0.35f, 0.297f, 0.9f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glfwSetWindowUserPointer(scene.window, &ctrl);
        glfwSetMouseButtonCallback(scene.window,handleMouse_callback_func);

        ctrl.readKeyInput();
        ctrl.readMouseInput();

        
        ourShader.use();

        if(ctrl.state == 2)
        {
            model->DrawLevel(ourShader);
            
        }
        
    

        
        glfwPollEvents();
        glfwSwapBuffers(scene.window);

    }

    glfwTerminate();

    return 0;
}




// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}