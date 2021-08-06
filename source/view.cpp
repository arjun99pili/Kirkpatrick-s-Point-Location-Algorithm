#include "../include/view.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <fstream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glm/gtc/matrix_transform.hpp>


View::View()
{
  //  glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
 //   glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    width = 1400;
    height = 1400;  
    window = glfwCreateWindow( width, height, "A-1", NULL, NULL);
    if( window == NULL ){
        fprintf( stderr, "Failed to open GLFW window\n" );
        getchar();
        glfwTerminate();
        
    }
    glfwMakeContextCurrent(window);
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        //return -1;
    }
 glfwMakeContextCurrent(window);

    //Rnable state variable depth(z).
    glEnable(GL_DEPTH_TEST);
   
    // Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS); 

	// Cull triangles whose normal is not towards the camera
	//glEnable(GL_CULL_FACE);

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    
    // Hide the mouse and enable unlimited mouvement
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    projectionMatrix = mat4(1.0f);
    viewMatrix = mat4(1.0f);


}


void View::setShader(Shader ourShader)
{
    ourShader.use();

    //setting up camera
	vec3 position = glm::vec3( 0.0f, 0.0f, 2.0f );
    // Initial horizontal angle : toward -Z
    float horizontalAngle = 3.14f;
    // Initial vertical angle : none
    float verticalAngle = -0.0f;
   
	float speed = 5.0f; // 3 units / second
    float mouseSpeed = 0.0002f;

	// Get mouse position
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	// Reset mouse position for next frame
	glfwSetCursorPos(window, 1024/2, 768/2);
	
	
	// Compute new orientation
	//horizontalAngle += mouseSpeed * float(1024/2 - xpos );
	//verticalAngle   += mouseSpeed * float( 768/2 - ypos );

	// Direction : Spherical coordinates to Cartesian coordinates conversion
	glm::vec3 direction(
		cos(verticalAngle) * sin(horizontalAngle), 
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle)
	);
	
	// Right vector
	glm::vec3 right = glm::vec3(
		sin(horizontalAngle - 3.14f/2.0f), 
		0,
		cos(horizontalAngle - 3.14f/2.0f)
	);

	// down vector
	glm::vec3 down = glm::vec3(
		0,
		sin(verticalAngle - 3.14f/2.0f),
		cos(verticalAngle - 3.14f/2.0f)
	);
	
	// Up vector
	glm::vec3 up = glm::cross( right, direction );

	// viewMatrix = glm::lookAt(
	// 					position,           // Camera is here
	// 					position+direction, // and looks here : at the same position, plus "direction"
	// 					up                  // Head is up (set to 0,-1,0 to look upside-down)
	// 					   );

   // viewMatrix = glm::lookAt(glm::vec3(camX, 0.0, camZ), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));  
        viewMatrix = glm::lookAt(glm::vec3(0.0f, 0.0f, 2.0f), 
  		   glm::vec3(0.0f, 0.0f, 0.0f), 
  		   glm::vec3(0.0f, 1.0f, 0.0f));
    ourShader.setMat4("view", viewMatrix);

    float FoV = 45.0f;
    projectionMatrix = glm::perspective(glm::radians(FoV), 3.0f / 3.0f, 0.1f, 1000.0f);

    ourShader.setMat4("projection", projectionMatrix);

    


}