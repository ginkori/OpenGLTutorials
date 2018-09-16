#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glad/glad.h>

class Shader
{
public:
	// ������������� ���������
	GLuint Program;
	// ����������� ��������� � �������� ������
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath)
	{
		// 1. �������� �������� ��� ������� �� filePath
		std::string vertexCode;
		std::string fragmentCode;
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;
		// �������������, ��� ifstream ������� ����� ���������� ����������
		vShaderFile.exceptions(std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::badbit);
		try
		{
			// ��������� �����
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);
			std::stringstream vShaderStream, fShaderStream;
			// ��������� ������ � ������
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();
			// ��������� �����
			vShaderFile.close();
			fShaderFile.close();
			// ��������������� ������ � ������ GLchar
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
		}
		catch (std::ifstream::failure e)
		{
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		}
		const GLchar* vShaderCode = vertexCode.c_str();
		const GLchar* fShaderCode = fragmentCode.c_str();

		// 2. ������ ��������
		GLuint vertex, fragment;
		GLint success;
		GLchar infoLog[512];

		// ��������� ������
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, NULL);
		glCompileShader(vertex);
		// ���� ���� ������ - ������� ��
		glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vertex, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		};

		// ���������� ��� ������������ �������
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, NULL);
		glCompileShader(fragment);
		// ���� ���� ������ - ������� ��
		glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fragment, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		}

		// ��������� ���������
		this->Program = glCreateProgram();
		glAttachShader(this->Program, vertex);
		glAttachShader(this->Program, fragment);
		glLinkProgram(this->Program);
		//���� ���� ������ - ������� ��
		glGetProgramiv(this->Program, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(this->Program, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		}

		// ������� �������, ��������� ��� ��� � ��������� � ��� ������ �� �����.
		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}
	// ����������� ��� ��������������� �������
	Shader(const GLchar* computePath)
	{
		// 1. �������� �������� ��� ������� �� filePath
		std::string computeCode;
		std::ifstream cShaderFile;

		// �������������, ��� ifstream ������� ����� ���������� ����������
		cShaderFile.exceptions(std::ifstream::badbit);
		try
		{
			// ��������� �����
			cShaderFile.open(computePath);
			std::stringstream cShaderStream;
			// ��������� ������ � ������
			cShaderStream << cShaderFile.rdbuf();
			// ��������� �����
			cShaderFile.close();
			// ��������������� ������ � ������ GLchar
			computeCode = cShaderStream.str();
		}
		catch (std::ifstream::failure e)
		{
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		}
		const GLchar* cShaderCode = computeCode.c_str();

		// 2. ������ ��������
		GLuint compute;
		GLint success;
		GLchar infoLog[512];

		// ��������� ������
		compute = glCreateShader(GL_COMPUTE_SHADER);
		glShaderSource(compute, 1, &cShaderCode, NULL);
		glCompileShader(compute);
		// ���� ���� ������ - ������� ��
		glGetShaderiv(compute, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(compute, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		};

		// ��������� ���������
		this->Program = glCreateProgram();
		glAttachShader(this->Program, compute);
		glLinkProgram(this->Program);
		//���� ���� ������ - ������� ��
		glGetProgramiv(this->Program, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(this->Program, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		}

		// ������� �������, ��������� ��� ��� � ��������� � ��� ������ �� �����.
		glDeleteShader(compute);
	}

	// �������� ������ ��� �������������
	void Use()
	{
		glUseProgram(this->Program);
	}
};

#endif