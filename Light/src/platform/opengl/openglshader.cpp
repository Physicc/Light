#include "platform/opengl/openglshader.hpp"

namespace Light
{
	static GLenum findShaderType(std::string typeStr)
	{
		if(typeStr == " fragment")
		{
			return GL_FRAGMENT_SHADER;
		}
		else if(typeStr == " vertex")
		{
			return GL_VERTEX_SHADER;
		}

		std::cerr << "Shader type not supported!" << std::endl;
		exit(1);
	}

	std::shared_ptr<Shader> Shader::create(const char* shaderPath)
	{
		return std::make_shared<OpenGLShader>(shaderPath);
	}

	OpenGLShader::OpenGLShader(const char* shaderPath)
	{
		std::string pathStr(shaderPath);
		auto lastSlash = pathStr.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		auto lastDot = pathStr.find_last_of(".");
		lastDot = lastDot == std::string::npos ? pathStr.length() - 1 : lastDot;
		name = pathStr.substr(lastSlash, lastDot - lastSlash);

		std::string code;
		std::ifstream shaderFile;
		shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try
		{
			shaderFile.open(shaderPath);
			std::stringstream sstream;
			sstream << shaderFile.rdbuf();
			shaderFile.close();
			code = sstream.str();
		}
		catch (std::ifstream::failure& e)
		{
			std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
		}

		rendererId = glCreateProgram();

		std::vector<uint32_t> shaderIds;

		std::string typeToken = "#type";
		size_t i = code.find(typeToken, 0);
		std::string eolToken = "\r\n";
		if (i == std::string::npos)
		{
			std::cerr << "ERROR::SHADER::Cannot find any shaders" << std::endl;
		}
		while(true)
		{
			size_t eol = code.find_first_of(eolToken, i);
			GLenum shaderType = findShaderType(code.substr(i + typeToken.length(), eol - i - typeToken.length()));
			auto j = code.find(typeToken, eol);
			std::string shaderCode = code.substr(eol + 1,j - eol - 1);
			const char* shaderCodeCstr = shaderCode.c_str();
			i = j;
			uint32_t shaderId;
			shaderId = glCreateShader(shaderType);
			shaderIds.push_back(shaderId);
			glShaderSource(shaderId, 1, &shaderCodeCstr, NULL);
			glCompileShader(shaderId);
			checkCompileErrors(shaderId, shaderType);
			glAttachShader(rendererId, shaderId);
			if(j == std::string::npos)
				break;
		}

		glLinkProgram(rendererId);
		checkCompileErrors(rendererId, GL_NONE);

		for(auto id : shaderIds)
		{
			glDeleteShader(id);
		}
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
	void OpenGLShader::checkCompileErrors(unsigned int shader, GLenum shaderType) 
	{
		int success;
		char infoLog[1024];
		if (shaderType == GL_VERTEX_SHADER || shaderType == GL_FRAGMENT_SHADER)
		{
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(shader, 1024, NULL, infoLog);
				std::string type = shaderType == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT";
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
				std::cerr << "ERROR::PROGRAM_LINKING_ERROR of type: " << "PROGRAM" << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
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