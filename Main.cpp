
// opengl32.lib is part of windows: https://stackoverflow.com/questions/1236670/how-to-make-opengl-apps-in-64-bit-windows

// GLEW
#define GLEW_STATIC
//#include <GL/glew.h>



#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "renderer.h"
#include "vertex_buffer.h"
#include "index_buffer.h"
#include "shader_handler.h"
#include "texture.h"

// GLFW
#include <GLFW/glfw3.h>

// Window size
const GLint SCREEN_WIDTH = 800, SCREEN_HEIGHT = 800;
//

int main()
{
	// Init GLFW
	glfwInit();

	// Set all required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Create a GLFWwindow object that we can use for GLFWs functions
	GLFWwindow * window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "LearnOpenGL", nullptr, nullptr);

	int screenWidth, screenHeight;
	glfwGetFramebufferSize(window, &screenWidth, &screenHeight);

	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window \n";
		glfwTerminate();
		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to set up the OpenGL function pointers
	if (GLEW_OK != glewInit())
	{
		std::cout << "Feiled to initialize GLEW \n";
		return EXIT_FAILURE;
	}

	// Define the viewport dimensions
	glViewport(0, 0, screenWidth, screenHeight);

	// Hello Triangle //

	// Vertices with their own color:
	float vertices[] = {
		// position			// color		  // ID
		// first quad
		-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		-0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		 0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,

		//second quad
		-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		-0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
		 0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		 0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,

		 //third quad
		-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 2.0f,
		-0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 2.0f,
		 0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 2.0f,
		 0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 2.0f

	};


	unsigned int indices[] = {  // note that we start from 0!
		0, 1, 2,   // first triangle of first quad
		2, 1, 3,   // second triangle of first quad

		4, 5, 6,   // first triangle of second quad
		6, 5, 7,   // second triangle of second quad

		8, 9, 10,  // first triangle of third quad
		10, 9, 11  // second triangle of third quad
	};

	glEnable(GL_DEPTH_TEST);

	/**/
	
	unsigned int shaderProgram = CreateShader(ParseShader("Resources/Shaders/VertexShader.glsl"), ParseShader("Resources/Shaders/FragmentShader.glsl"));
	
	int  success;
	char infoLog[512];
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
	}

	GLCall( glUseProgram(shaderProgram) );

	/*	*/

//	GLCall( glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0) );
//	GLCall( glEnableVertexAttribArray(0) );

//	OpenGL functions will be called through these objects:
//	VertexBuffer VB(vertices, sizeof(vertices));
//	IndexBuffer IB(indices, sizeof(indices)/sizeof(unsigned int));

	Texture texture("assets/saucer_blue.png");
	texture.Bind();

	unsigned int VBO;
	GLCall( glGenBuffers(1, &VBO) );
	GLCall( glBindBuffer(GL_ARRAY_BUFFER, VBO) );
	GLCall( glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW) );

	unsigned int VAO;
	GLCall( glGenVertexArrays(1, &VAO) );

	unsigned int EBO;
	GLCall( glGenBuffers(1, &EBO) );

	// ..:: Initialization code (done once (unless your object frequently changes)) :: ..
	// 1. bind Vertex Array Object
	GLCall(glBindVertexArray(VAO) );
	// 2. copy our vertices array in a buffer for OpenGL to use
//	glBindBuffer(GL_ARRAY_BUFFER, VBO);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// 3. copy our index array in a element buffer for OpenGL to use
	GLCall( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO) );
	GLCall( glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW) );
	// 4. then set the vertex attributes pointers
	// position attribute
	GLCall( glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0) );
	GLCall( glEnableVertexAttribArray(0) );
	// color attribute
	GLCall( glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3 * sizeof(float))) );
	GLCall( glEnableVertexAttribArray(1) );
	// ID attribute
	GLCall( glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(6 * sizeof(float))) );
	GLCall( glEnableVertexAttribArray(2) );

	//---------------------------------------------//

	int counter = 0;
	GLCall(unsigned int translateLoc = glGetUniformLocation(shaderProgram, "translation"));

//	glm::vec3 translate[2] = { glm::vec3(-0.25f, -0.25f, 0.1f), glm::vec3(0.25f, 0.4f, -0.1f) };
	glm::vec3 translate[3] = { {-0.25f, -0.25f, 0.1f}, {0.25f, 0.4f, -0.1f}, {-0.3f, 0.25f, 0.0f} };

	GLCall( glUniform3fv(translateLoc, 3, glm::value_ptr(translate[0])) );
	
	// Game loop
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		// Render
		// Clear color buffer
		GLCall( glClearColor(0.1f, 0.1f, 0.1f, 1.0f) );
		GLCall( glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) );

//		GLCall( glUseProgram(shaderProgram) );
//		GLCall( glBindVertexArray(VAO) );

		GLCall( glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, 0) );

		// Swap the screen buffers
		glfwSwapBuffers(window);

		std::cout << "loop completed: " << counter << "\n";
		counter++;
	}

	// Terminate GLFW, clearing and resources allocated by GLFW
	glfwTerminate();

	return EXIT_SUCCESS;
}



// 
// // uniform errors:
// https://www.khronos.org/registry/OpenGL-Refpages/es2.0/xhtml/glUniform.xml
// 
// Errors
// GL_INVALID_OPERATION is generated if there is no current program object.
// GL_INVALID_OPERATION is generated if the size of the uniform variable declared in the shader does not match the size indicated by the glUniform command.
// GL_INVALID_OPERATION is generated if one of the integer variants of this function is used to load a uniform variable of type float, vec2, vec3, vec4, or an array of these, or if one of the floating - point variants of this function is used to load a uniform variable of type int, ivec2, ivec3, or ivec4, or an array of these.
// GL_INVALID_OPERATION is generated if location is an invalid uniform location for the current program object and location is not equal to - 1.
// GL_INVALID_VALUE is generated if count is less than 0.
// GL_INVALID_VALUE is generated if transpose is not GL_FALSE.
// GL_INVALID_OPERATION is generated if count is greater than 1 and the indicated uniform variable is not an array variable.
// GL_INVALID_OPERATION is generated if a sampler is loaded using a command other than glUniform1i and glUniform1iv.
// 
// and Im getting this error: #define GL_INVALID_OPERATION 0x0502
// 








