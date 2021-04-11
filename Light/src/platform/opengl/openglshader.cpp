#include "platform/opengl/openglshader.hpp"

#include "glad/glad.h"

namespace Light
{
	Shader* Shader::create(const char* vertexPath, const char* fragmentPath)
	{
		return new OpenGLShader(vertexPath, fragmentPath);
	}

	// constructor generates the shader on the fly
	// ------------------------------------------------------------------------
	OpenGLShader::OpenGLShader(const char* vertexPath, const char* fragmentPath) 
	{
		// 1. retrieve the vertex/fragment source code from filePath
		std::string vertexCode;
		std::string fragmentCode;
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;
		// ensure ifstream objects can throw exceptions:
		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try
		{
			// open files
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);
			std::stringstream vShaderStream, fShaderStream;
			// read file's buffer contents into streams
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();
			// close file handlers
			vShaderFile.close();
			fShaderFile.close();
			// convert stream into string
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
		}
		catch (std::ifstream::failure& e)
		{
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		}
		const char* vShaderCode = vertexCode.c_str();
		const char* fShaderCode = fragmentCode.c_str();
		// 2. compile shaders
		unsigned int vertex, fragment;
		// vertex shader
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, NULL);
		glCompileShader(vertex);
		checkCompileErrors(vertex, "VERTEX");
		// fragment Shader
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, NULL);
		glCompileShader(fragment);
		checkCompileErrors(fragment, "FRAGMENT");
		// shader Program
		rendererId = glCreateProgram();
		glAttachShader(rendererId, vertex);
		glAttachShader(rendererId, fragment);
		glLinkProgram(rendererId);
		checkCompileErrors(rendererId, "PROGRAM");
		// delete the shaders as they're linked into our program now and no longer necessary
		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}

	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(rendererId);
	}

	void OpenGLShader::bind() 
	{
		glUseProgram(rendererId);
	}
	
	void OpenGLShader::unbind() 
	{
		glUseProgram(0);
	}
	

	// utility function for checking shader compilation/linking errors.
	// ------------------------------------------------------------------------
	void OpenGLShader::checkCompileErrors(unsigned int shader, std::string type) 
	{
		int success;
		char infoLog[1024];
		if (type != "PROGRAM")
		{
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(shader, 1024, NULL, infoLog);
				std::cerr << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
				exit(1);
			}
		}
		else
		{
			glGetProgramiv(shader, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetProgramInfoLog(shader, 1024, NULL, infoLog);
				std::cerr << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
				exit(1);
			}
		}
	}

	void OpenGLShader::setUniformBool(const std::string& name, bool value) const
	{
		glUniform1i(glGetUniformLocation(rendererId, name.c_str()), (int)value);
	}
	// ------------------------------------------------------------------------
	void OpenGLShader::setUniformInt(const std::string& name, int value) const
	{
		glUniform1i(glGetUniformLocation(rendererId, name.c_str()), value);
	}
	// ------------------------------------------------------------------------
	void OpenGLShader::setUniformFloat(const std::string& name, float value) const
	{
		glUniform1f(glGetUniformLocation(rendererId, name.c_str()), value);
	}
	// ------------------------------------------------------------------------
	void OpenGLShader::setUniformVec2(const std::string& name, const glm::vec2& value) const
	{
		glUniform2fv(glGetUniformLocation(rendererId, name.c_str()), 1, &value[0]);
	}

	// ------------------------------------------------------------------------
	void OpenGLShader::setUniformVec3(const std::string& name, const glm::vec3& value) const
	{
		glUniform3fv(glGetUniformLocation(rendererId, name.c_str()), 1, &value[0]);
	}

	// ------------------------------------------------------------------------
	void OpenGLShader::setUniformVec4(const std::string& name, const glm::vec4& value) const
	{
		glUniform4fv(glGetUniformLocation(rendererId, name.c_str()), 1, &value[0]);
	}

	// ------------------------------------------------------------------------
	void OpenGLShader::setUniformMat2(const std::string& name, const glm::mat2& mat) const
	{
		glUniformMatrix2fv(glGetUniformLocation(rendererId, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}
	// ------------------------------------------------------------------------
	void OpenGLShader::setUniformMat3(const std::string& name, const glm::mat3& mat) const
	{
		glUniformMatrix3fv(glGetUniformLocation(rendererId, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}
	// ------------------------------------------------------------------------
	void OpenGLShader::setUniformMat4(const std::string& name, const glm::mat4& mat) const
	{
		glUniformMatrix4fv(glGetUniformLocation(rendererId, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}
}