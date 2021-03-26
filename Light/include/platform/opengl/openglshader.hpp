#ifndef __OPENGLSHADER_H__
#define __OPENGLSHADER_H__

#include "core/base.hpp"
#include "rendering/shader.hpp"

namespace Light
{
	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const char* vertexPath, const char* fragmentPath);
		~OpenGLShader();

		void bind() override;
		void unbind() override;

	private:
		void checkCompileErrors(unsigned int shader, std::string type);

		uint32_t rendererId;
	};	

}

#endif // __OPENGLSHADER_H__