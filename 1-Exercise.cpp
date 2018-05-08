#include <iostream>
#define GLEW_STATIC
//#include <glad/glad.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <chrono>


// Function prototypes - зачем?
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
//константы
const GLuint WIDTH = 800, HEIGHT = 600;
//исодный код шейдеров
const GLchar* vertexShaderSource = R"glsl(
#version 330 core
layout (location = 0) in vec3 position;
void main()
{
	gl_Position = vec4(position.x, position.y, position.z, 1.0);
}
)glsl";
const GLchar* fragmentShaderSource = R"glsl(
#version 330 core
out vec4 color;
void main()
{
	color = vec4(1.0f, 1.0f, 0.0f, 1.0f);
}
)glsl";
const GLchar* fragmentShaderSource2 = R"glsl(
#version 330 core
uniform vec3 triangleColor;
out vec4 colorR;
void main()
{
	colorR = vec4(triangleColor, 1.0f);
}
)glsl";
const GLchar* vertexShaderSourceColorfull = R"glsl(
#version 330 core
in vec3 position;
in vec3 color;
out vec3 Color;
void main()
{
	Color = color;
	gl_Position = vec4(position, 1.0);
}
)glsl";
const GLchar* fragmentShaderSourceColorfull = R"glsl(
#version 330 core
in vec3 Color;
out vec4 outColor;
void main()
{
	outColor = vec4(Color, 1.0f);
}
)glsl";

int main()
{
	//время
	auto t_start = std::chrono::high_resolution_clock::now();

	std::cout << "Starting GLFW context, OpenGL 3.3" << std::endl;

	GLint nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;

	//Инициализация GLFW
	glfwInit();
	//Настройка GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	//создание окошка
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		system("pause");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	//инициализация глю
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		system("pause");
		return -1;
	}

	//порт просмотра
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	// Vertex shader - обычный
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	//шейдер для радужного треугольника
	GLuint vertexShaderC = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShaderC, 1, &vertexShaderSourceColorfull, NULL);
	glCompileShader(vertexShaderC);
	// Fragment shader - обычный
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	//фрагментный шейдер для радужного треугольника
	GLuint fragmentShaderC = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderC, 1, &fragmentShaderSourceColorfull, NULL);
	glCompileShader(fragmentShaderC);
	//фрагментный шейдер для изменяемого цвета
	GLuint fragmentShader2 = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader2, 1, &fragmentShaderSource2, NULL);
	glCompileShader(fragmentShader2);

	//цепляем шейдеры к шейдерной программе
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	//шейдерная программа для изменяемого цвета
	GLuint shaderProgram2 = glCreateProgram();
	glAttachShader(shaderProgram2, vertexShader);
	glAttachShader(shaderProgram2, fragmentShader2);
	glLinkProgram(shaderProgram2);
	//шейдерная программа для радужного цвета
	GLuint shaderProgramC = glCreateProgram();
	glAttachShader(shaderProgramC, vertexShaderC);
	glAttachShader(shaderProgramC, fragmentShaderC);
	glLinkProgram(shaderProgramC);
	//удаляем шейдеры
	glDeleteShader(vertexShader);
	glDeleteShader(vertexShaderC);
	glDeleteShader(fragmentShader);
	glDeleteShader(fragmentShaderC);
	glDeleteShader(fragmentShader2);

	GLfloat vertices[] = {
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
		0.0f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
		-0.25f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f
	};
	GLfloat vertices2[] = {
		0.1f, -0.5f, 0.0f,
		0.6f, -0.5f, 0.0f,
		0.35f, 0.0f, 0.0f
	};

	//создадим объект вершинного массива, вершинного буфера
	GLuint VBOs[2], VAOs[2];
	glGenVertexArrays(2, VAOs);
	glGenBuffers(2, VBOs);

	//первый треугольник
	glBindVertexArray(VAOs[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);

	GLint posAttrib = glGetAttribLocation(shaderProgramC, "position");
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (GLvoid*)0);

	GLint colAttrib = glGetAttribLocation(shaderProgramC, "color");
	glEnableVertexAttribArray(colAttrib);
	glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

	glEnableVertexAttribArray(0);
	glBindVertexArray(0); //отвязать вао1

	//второй треугольник
	glBindVertexArray(VAOs[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0); //отвязать вао2

	//функции обратного вызова
	glfwSetKeyCallback(window, key_callback);

	// Get the location of the color uniform
	GLint uniColor = glGetUniformLocation(shaderProgram2, "triangleColor");

	//укажем, как рисовать полигоны - линиями
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//игровой цикл
	while (!glfwWindowShouldClose(window))
	{
		// Проверяем события и вызываем функции обратного вызова.
		glfwPollEvents();
		//устанавливаем, откуда брать юниформ цвета 2-го треугольника
		auto t_now = std::chrono::high_resolution_clock::now();
		float time = std::chrono::duration_cast<std::chrono::duration<float>>(t_now - t_start).count();
		glUniform3f(uniColor, (sin(time * 4.0f) + 1.0f) / 2.0f, 0.0f, 0.0f);
		//чистим буфер цвета и экран
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		//рисуем 1-й треугольник
		glUseProgram(shaderProgramC);
		glBindVertexArray(VAOs[0]);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		//треугольник с другим цветом
		glUseProgram(shaderProgram2);
		glBindVertexArray(VAOs[1]);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);
		//поменять буферы
		glfwSwapBuffers(window);
	}

	//очистка ресурсов
	glDeleteVertexArrays(2, VAOs);
	glDeleteBuffers(2, VBOs);
	glfwTerminate();
	return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	// Когда пользователь нажимает ESC, мы устанавливаем свойство WindowShouldClose в true, 
	// и приложение после этого закроется
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}