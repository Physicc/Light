#ifndef __MESH_H__
#define __MESH_H__

#include "light/rendering/vertexarray.hpp"

#include <vector>
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

	private:
		std::vector<glm::vec3> m_vertices;
		std::vector<glm::vec4> m_colors;
		std::vector<glm::vec3> m_normals;
		std::vector<unsigned int> m_indices;

		std::shared_ptr<VertexArray> m_vao;
	};

}


#endif // __MESH_H__