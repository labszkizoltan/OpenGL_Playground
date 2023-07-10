
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

#include "shape_creator.h"

#include <math.h>

// GLFW
#include <GLFW/glfw3.h>

// Window size
const GLint SCREEN_WIDTH = 1200, SCREEN_HEIGHT = 600;
//const GLint SCREEN_WIDTH = 400, SCREEN_HEIGHT = 200;
//

int main()
{

	Shape3D quad = CreateQuad();
	quad.simplify(0.001f);
	Shape3D plane = CreatePlane(2, 3);
	Shape3D cube = CreateCube(1);
	Shape3D sphere = CreateSphere(5);


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

		glfwSwapInterval(1); // synchronize frame rate with the monitor

		// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
		glewExperimental = GL_TRUE;
		// Initialize GLEW to set up the OpenGL function pointers
		if (GLEW_OK != glewInit())
		{
			std::cout << "Failed to initialize GLEW \n";
			return EXIT_FAILURE;
		}

		// Define the viewport dimensions
		glViewport(0, 0, screenWidth, screenHeight);


		float vertices[] = {
			// position			 // color		   //texture coords  // ID
			// first cube
			-0.5f, -0.5f, -0.5f, 0.8f, 0.0f, 0.0f, 0.0f, 0.0f,       0.0f,
			-0.5f, -0.5f,  0.5f, 0.8f, 0.0f, 0.0f, 0.0f, 0.0f,       0.0f,
			-0.5f,  0.5f, -0.5f, 0.8f, 0.0f, 0.0f, 0.0f, 0.0f,       0.0f,
			-0.5f,  0.5f,  0.5f, 0.8f, 0.0f, 0.0f, 0.0f, 0.0f,       0.0f,
			 0.5f, -0.5f, -0.5f, 0.0f, 0.8f, 0.0f, 0.0f, 0.0f,       0.0f,
			 0.5f, -0.5f,  0.5f, 0.0f, 0.8f, 0.0f, 0.0f, 0.0f,       0.0f,
			 0.5f,  0.5f, -0.5f, 0.0f, 0.8f, 0.0f, 0.0f, 0.0f,       0.0f,
			 0.5f,  0.5f,  0.5f, 0.0f, 0.8f, 0.0f, 0.0f, 0.0f,       0.0f,
			 // second cube
			-0.5f, -0.5f, -0.5f, 0.0f, 0.7f, 0.1f, 0.0f, 0.0f,       1.0f,
			-0.5f, -0.5f,  0.5f, 0.0f, 0.7f, 0.1f, 0.0f, 0.0f,       1.0f,
			-0.5f,  0.5f, -0.5f, 0.0f, 0.7f, 0.1f, 0.0f, 0.0f,       1.0f,
			-0.5f,  0.5f,  0.5f, 0.0f, 0.7f, 0.1f, 0.0f, 0.0f,       1.0f,
			 0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.8f, 0.0f, 0.0f,       1.0f,
			 0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 0.8f, 0.0f, 0.0f,       1.0f,
			 0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 0.8f, 0.0f, 0.0f,       1.0f,
			 0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 0.8f, 0.0f, 0.0f,       1.0f,
			 // third cube
			-0.5f, -0.5f, -0.5f, 0.2f, 0.2f, 0.5f, 0.0f, 0.0f,       2.0f,
			-0.5f, -0.5f,  0.5f, 0.2f, 0.2f, 0.5f, 0.0f, 0.0f,       2.0f,
			-0.5f,  0.5f, -0.5f, 0.3f, 0.2f, 0.5f, 0.0f, 0.0f,       2.0f,
			-0.5f,  0.5f,  0.5f, 0.3f, 0.2f, 0.5f, 0.0f, 0.0f,       2.0f,
			 0.5f, -0.5f, -0.5f, 0.4f, 0.2f, 0.5f, 0.0f, 0.0f,       2.0f,
			 0.5f, -0.5f,  0.5f, 0.4f, 0.2f, 0.5f, 0.0f, 0.0f,       2.0f,
			 0.5f,  0.5f, -0.5f, 0.5f, 0.2f, 0.5f, 0.0f, 0.0f,       2.0f,
			 0.5f,  0.5f,  0.5f, 0.5f, 0.2f, 0.5f, 0.0f, 0.0f,       2.0f
		};

		unsigned int indices[] = {  // note that we start from 0!
			0, 2, 1, 1, 2, 3, // left side
			0, 4, 2, 2, 4, 6, // front side
			0, 1, 4, 1, 5, 4, // bottom
			4, 5, 6, 5, 7, 6, // right side
			2, 6, 3, 3, 6, 7, // top
			1, 3, 5, 3, 7, 5,  // bottom

			8, 10, 9, 9, 10, 11, // left side
			8, 12, 10, 10, 12, 14, // front side
			8, 9, 12, 9, 13, 12, // bottom
			12, 13, 14, 13, 15, 14, // right side
			10, 14, 11, 11, 14, 15, // top
			9, 11, 13, 11, 15, 13,  // bottom

			16, 18, 17, 17, 18, 19, // left side
			16, 20, 18, 18, 20, 22, // front side
			16, 17, 20, 17, 21, 20, // bottom
			20, 21, 22, 21, 23, 22, // right side
			18, 22, 19, 19, 22, 23, // top
			17, 19, 21, 19, 23, 21  // bottom
		};


		glEnable(GL_DEPTH_TEST);
//		glEnable(GL_BLEND);
//		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

//		order independent blending from http://www.openglsuperbible.com/2013/08/20/is-order-independent-transparency-really-necessary/
//		(not looking very good)
//		glBlendFunc(GL_ZERO, GL_SRC_COLOR);
//		glBlendFunc(GL_ONE, GL_ONE);
	
		// Turn on wireframe mode
		glPolygonMode(GL_FRONT, GL_LINE);
		glPolygonMode(GL_BACK, GL_LINE);

		// Turn off wireframe mode
//		glPolygonMode(GL_FRONT, GL_FILL);
//		glPolygonMode(GL_BACK, GL_FILL);


		unsigned int shaderProgram = CreateShader(ParseShader("Resources/Shaders/VertexShader.glsl"), ParseShader("Resources/Shaders/FragmentShader.glsl"));

		int  success;
		char infoLog[512];
		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		}

		GLCall(glUseProgram(shaderProgram));

		/*	*/

	//	GLCall( glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0) );
	//	GLCall( glEnableVertexAttribArray(0) );

	//	OpenGL functions will be called through these objects:
	//	VertexBuffer VB(vertices, sizeof(vertices));
	//	IndexBuffer IB(indices, sizeof(indices)/sizeof(unsigned int));
	//	Texture texture("assets/saucer_blue.png");
	//	texture.Bind();


		VertexBuffer VBO(vertices, sizeof(vertices));
		
//		unsigned int VBO;
//		GLCall(glGenBuffers(1, &VBO));
//		GLCall(glBindBuffer(GL_ARRAY_BUFFER, VBO));
//		GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW));

		unsigned int VAO;
		GLCall(glGenVertexArrays(1, &VAO));

		unsigned int EBO;
		GLCall(glGenBuffers(1, &EBO));

		// Setting up textures

		unsigned char* m_LocalBuffer;
		stbi_set_flip_vertically_on_load(1);
		int texture_w, texture_h, texture_bpp;
		m_LocalBuffer = stbi_load("assets/saucer_blue.png", &texture_w, &texture_h, &texture_bpp, 4);

		unsigned int TextureID;
		GLCall(glGenTextures(1, &TextureID));
		GLCall(glBindTexture(GL_TEXTURE_2D, TextureID));


		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, texture_w, texture_h, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer));
		GLCall(glBindTexture(GL_TEXTURE_2D, 0));

		if (m_LocalBuffer)
		{
			stbi_image_free(m_LocalBuffer);
		}

		GLCall(glBindTexture(GL_TEXTURE_2D, TextureID)); // hmm, for some reason, this second binding is necessary, otherwise I get black rectangles

		// ..:: Initialization code (done once (unless your object frequently changes)) :: ..
		// 1. bind Vertex Array Object
		GLCall(glBindVertexArray(VAO));
		// 2. copy our vertices array in a buffer for OpenGL to use
	//	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		// 3. copy our index array in a element buffer for OpenGL to use
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO));
		GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW));
		// 4. then set the vertex attributes pointers
		// position attribute
		GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0));
		GLCall(glEnableVertexAttribArray(0));
		// color attribute
		GLCall(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float))));
		GLCall(glEnableVertexAttribArray(1));
		// texture coordinate attribute
		GLCall(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float))));
		GLCall(glEnableVertexAttribArray(2));
		// ID attribute
		GLCall(glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(8 * sizeof(float))));
		GLCall(glEnableVertexAttribArray(3));

		//---------------------------------------------//

		int counter = 0;
		GLCall(unsigned int translateLoc = glGetUniformLocation(shaderProgram, "translation"));

		//	glm::vec3 translate[2] = { glm::vec3(-0.25f, -0.25f, 0.1f), glm::vec3(0.25f, 0.4f, -0.1f) };
//		glm::vec3 translate[3] = { {-0.25f, -0.25f, 0.1f}, {0.25f, 0.4f, 0.0f}, {-0.3f, 0.25f, -0.1f} };
		glm::vec3 translate[3] = { {-0.25f, -0.25f, 2.1f}, {0.25f, 0.4f, 2.0f}, {-0.3f, 0.25f, 1.9f} };

		GLCall(glUniform3fv(translateLoc, 3, glm::value_ptr(translate[0])));

		// Game loop
		while (!glfwWindowShouldClose(window))
		{
			glfwPollEvents();

			glViewport(0, 0, screenWidth/2, screenHeight);

			// Render
			// Clear color buffer
			GLCall(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
			GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

			//		GLCall( glUseProgram(shaderProgram) );
			//		GLCall( glBindVertexArray(VAO) );

			// Draw the left side
			glm::vec3 translate[3] = {
				{-0.5f*cos(0.05 * (float)counter), -0.25f, 4.0f},
				{1.0f*sin(0.025 * (float)counter), 1.0*sin(0.0125 * (float)counter), 3.0f},
				{-0.3f, 0.25f, 4.0f+1.0*cos(0.01 * (float)counter)}
			};
			GLCall(glUniform3fv(translateLoc, 3, glm::value_ptr(translate[0])));
			GLCall(glDrawElements(GL_TRIANGLES, 108, GL_UNSIGNED_INT, 0));

			float z_move = 5.0f;
			glm::vec3 translate_b[3] = {
				{-0.5f*cos(0.05 * (float)counter), -0.25f + z_move / 2.0, 4.0f+z_move},
				{1.0f*sin(0.025 * (float)counter), 1.0*sin(0.0125 * (float)counter) + z_move/2.0, 3.0f + z_move},
				{-0.3f, 0.25f + z_move / 2.0, 4.0f + 1.0*cos(0.01 * (float)counter + z_move)}
			};
			GLCall(glUniform3fv(translateLoc, 3, glm::value_ptr(translate_b[0])));
			GLCall(glDrawElements(GL_TRIANGLES, 108, GL_UNSIGNED_INT, 0));


			// Draw the right side
			glViewport(screenWidth / 2, 0, screenWidth / 2, screenHeight);

			float x_move = 0.4;
			glm::vec3 translate2[3] = {
				{-0.5f*cos(0.05 * (float)counter), x_move -0.25f, 4.0f},
				{1.0f*sin(0.025 * (float)counter), x_move +2.0*sin(0.0125 * (float)counter), 3.0f},
				{-0.3f, x_move +0.25f, 4.0f + 1.0*cos(0.01 * (float)counter)}
			};
			GLCall(glUniform3fv(translateLoc, 3, glm::value_ptr(translate2[0])));
			GLCall(glDrawElements(GL_TRIANGLES, 108, GL_UNSIGNED_INT, 0));

			glm::vec3 translate2_b[3] = {
				{-0.5f*cos(0.05 * (float)counter), -0.25f + z_move / 2.0+ x_move, 4.0f + z_move},
				{1.0f*sin(0.025 * (float)counter), 1.0*sin(0.0125 * (float)counter) + z_move / 2.0 + x_move, 3.0f + z_move},
				{-0.3f, 0.25f + z_move / 2.0 + x_move, 4.0f + 1.0*cos(0.01 * (float)counter + z_move)}
			};
			GLCall(glUniform3fv(translateLoc, 3, glm::value_ptr(translate2_b[0])));
			GLCall(glDrawElements(GL_TRIANGLES, 108, GL_UNSIGNED_INT, 0));



			// Swap the screen buffers
			glfwSwapBuffers(window);

			std::cout << "loop completed: " << counter << "\n";
			counter++;
		}

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








