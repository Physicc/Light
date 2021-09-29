#ifndef __MESH_H__
#define __MESH_H__

#include "light/rendering/vertexarray.hpp"

#include <vector>
#include <unordered_map>
#include <memory>
#include "glm/glm.hpp"

namespace Light
{
	class Mesh
	{
	public:
		Mesh(const std::vector<glm::vec3> &vertices,
			const std::vector<glm::vec4> &colors,
			const std::vector<glm::vec3> &normals,
			const std::vector<unsigned int> &indices);
		~Mesh() = default;

		inline std::shared_ptr<VertexArray> getVao() const { return m_vao; }

	private:
		std::vector<glm::vec3> m_vertices;
		std::vector<glm::vec4> m_colors;
		std::vector<glm::vec3> m_normals;
		std::vector<unsigned int> m_indices;

		std::shared_ptr<VertexArray> m_vao;
	};

	class MeshLibrary
	{
	public:
		void add(const std::string& name, std::shared_ptr<Mesh>& mesh);
		void add(const std::string& name,
			const std::vector<glm::vec3> &vertices,
			const std::vector<glm::vec4> &colors,
			const std::vector<glm::vec3> &normals,
			const std::vector<unsigned int> &indices);

		std::shared_ptr<Mesh> get(const std::string &name);
	
	private:
		std::unordered_map<std::string, std::shared_ptr<Mesh>> m_meshes;
	};
}


#endif // __MESH_H__