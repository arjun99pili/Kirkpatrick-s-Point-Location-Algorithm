#include "../include/control.hpp"



using namespace glm;

Controller::Controller(GLFWwindow* win, Model* mod)
{
	window = win;
	model = mod;
	object_controlled = 0;
	curr_time = prev_time = 0;
	toRotate = 0;
	glfwGetCursorPos(window, &x1pos, &y1pos);
	x2pos = x1pos;
	y2pos = y1pos;
	level = 0;
	count_points = 0;
	state = 0;
	testit = 0;
	
}

void Controller::readKeyInput()
{
    //filled and wireframe options.
    if (glfwGetKey( window, GLFW_KEY_W ) == GLFW_PRESS){
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	
	if (glfwGetKey( window, GLFW_KEY_F ) == GLFW_PRESS){
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

    // next level
	if (glfwGetKey( window, GLFW_KEY_LEFT ) == GLFW_PRESS){
		model->curLevel++;

	}
	if (glfwGetKey( window, GLFW_KEY_RIGHT ) == GLFW_PRESS){
		model->curLevel--;		
	}

	if(glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS){
		model->curLevel = 0;
	}

	if(glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS){
		model->curLevel = 1;
	}

	if(glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS){
		model->curLevel = 2;
	}

	if(glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS){
		model->curLevel = 3;
	}

	if(glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS){
		model->curLevel = 4;
	}

	if(glfwGetKey(window,GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if(glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
	{
		model->startbuild();
		state = 2;
	}
	
	if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		state = 3;
	}

	if(glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
	{
		float scale1 = 1400/21;
		float scale2 = 1400/16;
		if(testit == 0)
		{
			Node* t11 = model->createNode(0.0f * scale1, 0.0f * scale2);
    		Node* t12 = model->createNode(0.0f * scale1, 16.0f * scale2);
    		Node* t13 = model->createNode(21.0f * scale1, 0.0f * scale2);
    		Node* t21 = model->createNode(2.0f * scale1, 10.0f * scale2);
    		Node* t23 = model->createNode(5.0f * scale1, 10.0f * scale2);
    		Node* t22 = model->createNode(5.0f * scale1, 6.0f * scale2);

    		model->createTriangle(t11, t12, t13);

    		model->createTriangle(t11, t21, t22);
    		model->createTriangle(t11, t22, t13);
    		model->createTriangle(t11, t12, t21);


    		model->createTriangle(t21, t12, t23);
    		model->createTriangle(t21, t23, t22);

    		model->createTriangle(t22, t23, t13);
    		model->createTriangle(t23, t12, t13);

			model->startbuild();
			state = 2;
			testit = 1;
		}
	}

	
}

void Controller::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		if(state == 3)
		{
			model->searchForpoint(x1pos, y1pos);
			model->setBuffers(1);		
			state = 2;
		}
		else
		{
			model->next_it = false;
			clicks[count_points] = 0;
			points[count_points].x = x1pos;
			points[count_points].y = y1pos;
			count_points++;
			
		}
		
		if(count_points == 3)
		{
			model->getTriangle(points, clicks);
			count_points = 0;
		}
	}
	if(button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
	{
		model->next_it = false;
		clicks[count_points] = 1;
		points[count_points].x = x1pos;
		points[count_points].y = y1pos;
		count_points++;	
		
		if(count_points == 3)
		{
			model->getTriangle(points, clicks);
			count_points = 0;
		}
	}

	
}

void Controller::readMouseInput()
{
	
	glfwGetCursorPos(window, &x1pos, &y1pos);
}



