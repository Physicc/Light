#ifndef __OPENGLSHADER_H__
#define __OPENGLSHADER_H__

#include "core/base.hpp"
#include "rendering/shader.hpp"

#include "glad/glad.h"

namespace Light
{
	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const char* shaderPath);
		~OpenGLShader();

		void bind() override;
		void unbind() override;

		inline const std::string& getName() const override { return name; }

		void setUniformBool(const std::string& name, bool value) const;
		void setUniformInt(const std::string& name, int value) const;
		void setUniformFloat(const std::string& name, float value) const;
		void setUniformVec2(const std::string& name, const glm::vec2& value) const;
		void setUniformVec3(const std::string& name, const glm::vec3& value) const;
		void setUniformVec4(const std::string& name, const glm::vec4& value) const;
		void setUniformMat2(const std::string& name, const glm::mat2& mat) const;
		void setUniformMat3(const std::string& name, const glm::mat3& mat) const;
		void setUniformMat4(const std::string& name, const glm::mat4& mat) const;

	private:
		void checkCompileErrors(unsigned int shader, GLenum shaderType);

		std::string name;
		uint32_t rendererId;
	};

}

#endif // __OPENGLSHADER_H__