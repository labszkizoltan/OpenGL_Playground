
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
const GLint SCREEN_WIDTH = 500, SCREEN_HEIGHT = 500;

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

		// // combined vertices
		// float vertices[] = {
		// 	// position         // color          // tex coord // ID
		// 	-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,  0.0f,
		// 	 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,  0.0f,
		// 	 0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,  0.0f,
		// 	-0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f,  0.0f
		// };

		float positionVertices[] = {
			// position
			-0.5f, -0.25f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f,
			-0.5f,  0.25f, 0.0f
		};

		float colorVertices[] = {
			// position
			1.0f, 0.0f, 0.0f,
			1.0f, 0.0f, 0.0f,
			1.0f, 0.0f, 1.0f,
			1.0f, 0.0f, 1.0f
		};

		unsigned int indices[] = { 0, 1, 2, 2, 3, 0 };

		glEnable(GL_DEPTH_TEST);

		unsigned int shaderProgram = CreateShader(ParseShader("Resources/Shaders/Multibuf_vert.glsl"), ParseShader("Resources/Shaders/Multibuf_frag.glsl"));

		int  success;
		char infoLog[512];
		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		}

		GLCall(glUseProgram(shaderProgram));

		// VertexBuffer VBO(vertices, sizeof(vertices));

		// Create Vertex Array Object
		unsigned int VAO;
		GLCall(glGenVertexArrays(1, &VAO));

		// Create position and color buffers
		unsigned int posBuf, colBuf;
		glGenBuffers(1, &posBuf);
		// glBindBuffer(GL_ARRAY_BUFFER, posBuf);
		glGenBuffers(1, &colBuf);
		// glBindBuffer(GL_ARRAY_BUFFER, colBuf);

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
		// 2a. upload position data to GPU
		glBindBuffer(GL_ARRAY_BUFFER, posBuf);
		glBufferData(GL_ARRAY_BUFFER, sizeof(positionVertices), positionVertices, GL_STATIC_DRAW);
		// link position data to the appropriate vertex attribute:
		GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0));
		GLCall(glEnableVertexAttribArray(0));

		// 2b. upload color data to GPU
		glBindBuffer(GL_ARRAY_BUFFER, colBuf);
		glBufferData(GL_ARRAY_BUFFER, sizeof(colorVertices), colorVertices, GL_STATIC_DRAW);
		// link color data to the appropriate vertex attribute:
		GLCall(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0));
		GLCall(glEnableVertexAttribArray(1));

		// 3. copy our index array in a element buffer for OpenGL to use
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO));
		GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW));

		//---------------------------------------------//

		int counter = 0;
		float angle = 0.0f;
		GLCall(unsigned int angleLoc = glGetUniformLocation(shaderProgram, "angle"));
		GLCall(glUniform1f(angleLoc, angle));

		// Game loop
		while (!glfwWindowShouldClose(window))
		{
			glfwPollEvents();

			// Render
			// Clear color buffer
			GLCall(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
			GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

			// GLCall( glUseProgram(shaderProgram) );
			// GLCall( glBindVertexArray(VAO) );

			GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));

			angle += 0.01f;
			angle = angle > 6.28318530718f ? angle - 6.28318530718f : angle;
			GLCall(glUniform1f(angleLoc, angle));

			// Swap the screen buffers
			glfwSwapBuffers(window);

			counter++;
		}

	}

	// Terminate GLFW, clearing and resources allocated by GLFW
	glfwTerminate();

	return EXIT_SUCCESS;
}









