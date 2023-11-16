#pragma once // or #ifnded Shader.H \n #define Shader.H ... #endif


#include <glad/glad.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <tuple>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>



class MyShader
{
public:
	unsigned int ID;
	std::pair<const char*, const char*> Path;

	MyShader(const char* VertexShaderSourcePath, const char* FragmentShaderSourcePath)
		:Path(VertexShaderSourcePath, FragmentShaderSourcePath)
	{
		std::ifstream VS_path;
		std::ifstream FS_path;

		std::string VS;
		std::string FS;

		std::string vsHolder;
		std::string fsHolder;

		std::stringstream fsStream; // vsStream

		// just ensure that the streams will throw exceptions

		VS_path.exceptions(std::ifstream::failbit | std::ifstream::badbit); // or another way to do this is remove failbit 
		FS_path.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try
		{
			VS_path.open(VertexShaderSourcePath);
			FS_path.open(FragmentShaderSourcePath);

			try
			{
				if (VS_path.is_open())
				{
					while (std::getline(VS_path, vsHolder))
					{
						VS += vsHolder + "\n";
					}
				}
			}
			catch (std::ifstream::failure e) {}

			//vsStream << VS_path.rdbuf();
			fsStream << FS_path.rdbuf();

			VS_path.close();
			FS_path.close();

			//vsHolder = vsStream.str();
			fsHolder = fsStream.str();
		}
		catch (std::ifstream::failure e)
		{
			std::cout << "ERROR::FAILED::OPEN::SHADER_FILES\n" << std::endl;
		}

		const char* vShaderCode = VS.c_str();
		const char* fShaderCode = fsHolder.c_str();

		unsigned int vertex, fragment;

		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, NULL);
		glCompileShader(vertex);

		CheckError(vertex, 1);

		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, NULL);
		glCompileShader(fragment);

		CheckError(fragment, 2);

		MyShader::ID = glCreateProgram();
		glAttachShader(MyShader::ID, vertex);
		glAttachShader(MyShader::ID, fragment);
		glLinkProgram(MyShader::ID);

		CheckError(MyShader::ID, 3);

		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}
	~MyShader() { glDeleteProgram(MyShader::ID); }
	void use()
	{
		glUseProgram(MyShader::ID);
	}
	void SetUniform1f(const char* uniform_name, float x)
	{
		glUniform1f(glGetUniformLocation(MyShader::ID, uniform_name), x);
	}
	void SetUniform1i(const char* uniform_name, int Vx)
	{
		glUniform1i(glGetUniformLocation(MyShader::ID, uniform_name), Vx);
	}
	void SetUniform3f(const char* uniform_name, float x, float y, float z)
	{
		glUniform3f(glGetUniformLocation(MyShader::ID, uniform_name), x, y, z);
	}
	void SetUniform3f(const char* uniform_name, glm::vec3 vector)
	{
		glUniform3f(glGetUniformLocation(MyShader::ID, uniform_name), vector.x, vector.y, vector.z);
	}
	void SetUniformMat4fv(const char* MAT_uniform_name, int count, glm::mat4 matrix, bool transpose)
	{
		if(transpose)
			glUniformMatrix4fv(glGetUniformLocation(MyShader::ID, MAT_uniform_name), count, GL_TRUE, glm::value_ptr(matrix));
		else 
			glUniformMatrix4fv(glGetUniformLocation(MyShader::ID, MAT_uniform_name), count, GL_FALSE, glm::value_ptr(matrix));
	}
private:

	void CheckError(unsigned int shader, int type)
	{
		// 1 : vs, 2 : fs, else ps
		int success;
		char InfoLog[512];

		if (type == 1)
		{
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(shader, 512, NULL, InfoLog);
				std::cout << "ERROR::VERTEX::SHADER::FAILED_COMPILATION\n" << InfoLog ;
				std::cout << "THE_PATH : " << MyShader::Path.first << "\n" << std::endl;
			}
		}
		else if (type == 2)
		{
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(shader, 512, NULL, InfoLog);
				std::cout << "ERROR::FRAGMENT::SHADER::FAILED_COMPILATION\n" << InfoLog ;
				std::cout << "THE_PATH : " << MyShader::Path.second << "\n" << std::endl;
			}
		}
		else
		{
			glGetProgramiv(shader, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetProgramInfoLog(shader, 512, NULL, InfoLog);
				std::cout << "ERROR::PROGRAM::SHADER::FAILED_LINKING\n" << InfoLog ;
			}
		}
	}

};


