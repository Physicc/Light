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

		virtual const std::string& getName() const = 0;

		static std::shared_ptr<Shader> create(const char* shaderPath);

		virtual void bind() = 0;
		virtual void unbind() = 0;
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
		std::unordered_map<std::string, std::shared_ptr<Shader>> shaders;
	};

}

#endif // __SHADER_H__