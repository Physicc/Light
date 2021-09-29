#include "light/rendering/mesh.hpp"
#include "light/rendering/buffer.hpp"

#include "core/assert.hpp"
#include "core/logging.hpp"

namespace Light
{
	Mesh::Mesh(const std::vector<glm::vec3> &vertices,
			const std::vector<glm::vec4> &colors,
			const std::vector<glm::vec3> &normals,
			const std::vector<unsigned int> &indices)
		: m_vertices(vertices), m_colors(colors), m_normals(normals), m_indices(indices), m_vao(Light::VertexArray::create())
	{
		LIGHT_ASSERT(vertices.size() == colors.size() && vertices.size() == normals.size());

		std::vector<float> vertex_data(vertices.size() * (3 + 4 + 3));

		int num_verts = vertices.size();

		for (int i = 0; i < num_verts; i++)
		{
			vertex_data[10 * i] = m_vertices[i].x;
			vertex_data[10 * i + 1] = m_vertices[i].y;
			vertex_data[10 * i + 2] = m_vertices[i].z;

			vertex_data[10 * i + 3] = m_colors[i].r;
			vertex_data[10 * i + 4] = m_colors[i].g;
			vertex_data[10 * i + 5] = m_colors[i].b;
			vertex_data[10 * i + 6] = m_colors[i].a;

			vertex_data[10 * i + 7] = m_normals[i].x;
			vertex_data[10 * i + 8] = m_normals[i].y;
			vertex_data[10 * i + 9] = m_normals[i].z;
		}

		Light::BufferLayout layout({
				{ Light::ShaderDataType::Float3, "a_Position" },
				{ Light::ShaderDataType::Float4, "a_Color" },
				{ Light::ShaderDataType::Float3, "a_Normal" }
			});

		std::shared_ptr<Light::VertexBuffer> vbo(Light::VertexBuffer::create(vertex_data.data(), vertex_data.size() * sizeof(vertex_data[0])));
		vbo->setLayout(layout);

		std::shared_ptr<Light::IndexBuffer> ibo(Light::IndexBuffer::create(m_indices.data(), m_indices.size()));

		m_vao->addVertexBuffer(vbo);
		m_vao->setIndexBuffer(ibo);
	}

	void MeshLibrary::add(const std::string& name, std::shared_ptr<Mesh>& mesh) 
	{
		if(m_meshes.find(name) != m_meshes.end())
		{
			LIGHT_CORE_ERROR("Mesh already exists");
		}

		m_meshes[name] = mesh;
	}
	
	void MeshLibrary::add(const std::string& name,
				const std::vector<glm::vec3>& vertices,
				const std::vector<glm::vec4>& colors,
				const std::vector<glm::vec3>& normals,
				const std::vector<unsigned int>& indices) 
	{
		if(m_meshes.find(name) != m_meshes.end())
		{
			LIGHT_CORE_ERROR("Mesh already exists");
		}

		m_meshes[name] = std::make_shared<Mesh>(vertices, colors, normals, indices);
	}
	
	std::shared_ptr<Mesh> MeshLibrary::get(const std::string& name)
	{
		if(m_meshes.find(name) == m_meshes.end())
		{
			LIGHT_CORE_ERROR("Mesh does not exist");
		}

		return m_meshes[name];
	}
}
