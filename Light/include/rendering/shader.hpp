#ifndef __SHADER_H__
#define __SHADER_H__

#include "core/base.hpp"

namespace Light
{
	class Shader
	{
	public:
		Shader() {}
		virtual ~Shader() = default;

		static Shader* create(const char* vertexPath, const char* fragmentPath);

		virtual void bind() = 0;
		virtual void unbind() = 0;
	};

}

#endif // __SHADER_H__