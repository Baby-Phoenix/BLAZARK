#include "Mesh.h"

const VertexBuffer* Mesh::GetVBO(VertexAttrib bufferAttrib) const
{
	auto iterator = m_vboDict.find(bufferAttrib);

	if (iterator == m_vboDict.end())
		return nullptr;
	else
		return iterator->second.get();
}

void Mesh::SetPositions(const std::vector<glm::vec3>& positions)
{
	SetVBO(VertexAttrib::POSITION, positions, 3);
}

void Mesh::SetNormals(const std::vector<glm::vec3>& normals)
{
	SetVBO(VertexAttrib::NORMAL, normals, 3);
}

void Mesh::SetTexCoords(const std::vector<glm::vec2>& texCoords)
{
	SetVBO(VertexAttrib::TEXCOORD, texCoords, 2);
}
