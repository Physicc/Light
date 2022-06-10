#ifndef __OPENGLSHADER_H__
#define __OPENGLSHADER_H__

#include "core/base.hpp"
#include "light/rendering/shader.hpp"

#include "glad/gl.h"

namespace Light
{
	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const char* shaderPath);
		~OpenGLShader();

		void bind() override;
		void unbind() override;

		inline const std::string& getName() const override { return m_name; }

		void setUniformBool(const std::string& name, bool value) const override;
		void setUniformInt(const std::string& name, int value) const override;
		void setUniformFloat(const std::string& name, float value) const override;
		void setUniformVec2(const std::string& name, const glm::vec2& value) const override;
		void setUniformVec3(const std::string& name, const glm::vec3& value) const override;
		void setUniformVec4(const std::string& name, const glm::vec4& value) const override;
		void setUniformMat2(const std::string& name, const glm::mat2& mat) const override;
		void setUniformMat3(const std::string& name, const glm::mat3& mat) const override;
		void setUniformMat4(const std::string& name, const glm::mat4& mat) const override;

	private:
		void checkCompileErrors(unsigned int shader, GLenum shaderType);

		std::string m_name;
		uint32_t m_rendererId;
	};

}

#endif // __OPENGLSHADER_H__
