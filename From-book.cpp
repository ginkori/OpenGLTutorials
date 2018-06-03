#include <iostream>
#include "stb_image.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shader_class.h"
// glm
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

const GLuint WIDTH = 800, HEIGHT = 600;

float mixValue = 0.2f;
// For camera
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
// For camera speed
float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame
// For mouse movement
bool firstMouse = true;
float lastX = 400, lastY = 300;
float yaw = -90.0f;
float pitch = 0.0f;
float fov = 45.0f;

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	glfwSetKeyCallback(window, key_callback);

	//For mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	//

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glEnable(GL_DEPTH_TEST);

	Shader ourShader("Shaders/from-book.vs", "Shaders/from-book.fs");

	GLfloat vertices[] = {
		// Positions         // Colors          //Text. coord.
		0.5f, -0.5f, 0.0f,   1.0f, 0.0f, 0.0f,  1.0f, 0.0f, // Bottom Right
		-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f, // Bottom Left
		0.0f,  0.5f, 0.0f,   0.0f, 0.0f, 1.0f,  0.5f, 1.0f, // Top

		0.0f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,  1.0f, 0.0f,
		0.5f, -0.5f, 0.0f,   1.0f, 0.0f, 0.0f,  0.0f, 0.0f,
		0.0f, 0.0f, 1.0f,    1.0f, 0.0f, 0.0f,  0.5f, 1.0f,

		-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f,
		0.0f,  0.5f, 0.0f,   0.0f, 1.0f, 0.0f,  0.0f, 0.0f,
		0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 0.0f,  0.5f, 1.0f,

		0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,  1.0f, 0.0f,
		-0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f,
		0.0f, 0.0f, 1.0f,    0.0f, 0.0f, 1.0f,  0.5f, 1.0f
	};

	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// Color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	// Texture attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);

	//Create a sampler object
	unsigned int sampl;
	glCreateSamplers(1, &sampl);
	//Set the parameters for texture
	glSamplerParameteri(sampl, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glSamplerParameteri(sampl, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glSamplerParameteri(sampl, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glSamplerParameteri(sampl, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Generate a texture index
	unsigned int texture1, texture2;
	// 1 - texture
	glGenTextures(1, &texture1);
	// Bind it
	glBindTexture(GL_TEXTURE_2D, texture1);
	// Set the texture wrapping/filtering options (bind the sampler object)
	glBindSampler(0, sampl);
	// Load image and gen a texture
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char *data = stbi_load("Textures/container.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		// Create a mipmap for this texture
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	// Free the image memory
	stbi_image_free(data);
	// 2 - texture
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glBindSampler(1, sampl);
	data = stbi_load("Textures/awesomeface.png", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Bind texture to a texture unit, it will assign the texture to the fragment shader's sampler
		glBindTextureUnit(0, texture1);
		glBindTextureUnit(1, texture2);
		// Set the texture mix value in the shader
		glUniform1f(6, mixValue);

		ourShader.Use();

		// Projection matrix
		glm::mat4 projection = glm::mat4(1.0f);
		projection = glm::perspective(glm::radians(fov), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
		// Matrix uniforms
		glUniformMatrix4fv(5, 1, GL_FALSE, glm::value_ptr(projection));

		// For camera speed - time logic
		float currentFrame = (float)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		// For camera matrix
		glm::mat4 view = glm::mat4(1.0f);
		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

		// Draw triangles
		glBindVertexArray(VAO);
		for (unsigned int i = 0; i < 10; i++)
		{
			// For mv matrix
			float f = (float)glfwGetTime() * 3.14159265359f * 0.1f + (float)i;
			float angle = (float)glfwGetTime();

			glm::mat4 mv = glm::mat4(1.0f);
			mv = glm::translate(mv, glm::vec3(sinf(2.1f*f)*2.0f, cosf(1.7f*f)*2.0f, sinf(1.3f*f)*cosf(1.5f*f)*2.0f));
			mv = glm::rotate(mv, glm::radians(angle)*45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
			mv = glm::rotate(mv, glm::radians(angle)*21.0f, glm::vec3(1.0f, 0.0f, 0.0f));
			mv = view * mv;
			glUniformMatrix4fv(4, 1, GL_FALSE, glm::value_ptr(mv));

			glDrawArrays(GL_TRIANGLES, 0, 12);
		}
		glBindVertexArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// de-allocate all resources
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glfwTerminate();
	return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (key == GLFW_KEY_UP && action == GLFW_PRESS)
	{
		mixValue += 0.1f;
		if (mixValue >= 1.0f)
			mixValue = 1.0f;
	}

	if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
	{
		mixValue -= 0.1f;
		if (mixValue <= 0.0f)
			mixValue = 0.0f;
	}

	float cameraSpeed = 15.5f * deltaTime;
	if (key == GLFW_KEY_W && action == GLFW_PRESS)
		cameraPos += cameraSpeed * cameraFront;
	if (key == GLFW_KEY_S && action == GLFW_PRESS)
		cameraPos -= cameraSpeed * cameraFront;
	if (key == GLFW_KEY_A && action == GLFW_PRESS)
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (key == GLFW_KEY_D && action == GLFW_PRESS)
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	// Prevent a large sudden jump 
	if (firstMouse)
	{
		lastX = (float)xpos;
		lastY = (float)ypos;
		firstMouse = false;
	}

	// Calculate the offset movement between the last and current frame
	float xoffset = (float)xpos - lastX;
	// Reversed y-coordinates
	float yoffset = lastY - (float)ypos;
	lastX = (float)xpos;
	lastY = (float)ypos;

	float sensitivity = 0.05f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	// Prevent a weird camera movements 
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	// Calculate the actual direction vector for lookAt
	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(front);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (fov >= 1.0f && fov <= 45.0f)
		fov -= yoffset;
	if (fov <= 1.0f)
		fov = 1.0f;
	if (fov >= 45.0f)
		fov = 45.0f;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}