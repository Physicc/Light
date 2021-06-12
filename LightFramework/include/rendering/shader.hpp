#ifndef __SHADER_H__
#define __SHADER_H__

#include "core/base.hpp"

#include <unordered_map>

namespace Light
{
	class Shader
	{
	public:
		Shader() = default;
		virtual ~Shader() = default;

		virtual const std::string& getName() const = 0;

		static std::shared_ptr<Shader> create(const char* shaderPath);

		virtual void bind() = 0;
		virtual void unbind() = 0;

		virtual void setUniformBool(const std::string& name, bool value) const = 0;
		virtual void setUniformInt(const std::string& name, int value) const = 0;
		virtual void setUniformFloat(const std::string& name, float value) const = 0;
		virtual void setUniformVec2(const std::string& name, const glm::vec2& value) const = 0;
		virtual void setUniformVec3(const std::string& name, const glm::vec3& value) const = 0;
		virtual void setUniformVec4(const std::string& name, const glm::vec4& value) const = 0;
		virtual void setUniformMat2(const std::string& name, const glm::mat2& mat) const = 0;
		virtual void setUniformMat3(const std::string& name, const glm::mat3& mat) const = 0;
		virtual void setUniformMat4(const std::string& name, const glm::mat4& mat) const = 0;
	};

	class ShaderLibrary
	{
	public:
		void add(const std::shared_ptr<Shader>& shader);
		void add(const std::string& name, const std::shared_ptr<Shader>& shader);

		std::shared_ptr<Shader> load(const std::string& filepath);
		std::shared_ptr<Shader> load(const std::string& name, const std::string& filepath);

		std::shared_ptr<Shader> get(const std::string& name);

	private:
		std::unordered_map<std::string, std::shared_ptr<Shader>> m_shaders;
	};

}

#endif // __SHADER_H__