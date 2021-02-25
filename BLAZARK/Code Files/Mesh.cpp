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

void Mesh::SetMaterials(const std::vector<Material>& materials)
{
	std::vector<float> specularShininess;
	std::vector<glm::vec3> ambientColour;
	std::vector<glm::vec3> diffuseColour;
	std::vector<glm::vec3> specularColour;
	std::vector<float> dissolve;
	for (int i = 0; i < materials.size(); i++) {
		specularShininess.push_back(materials[i].specularShininess);
		ambientColour.push_back(materials[i].ambientColour);
		diffuseColour.push_back(materials[i].diffuseColour);
		specularColour.push_back(materials[i].specularColour);
		dissolve.push_back(materials[i].dissolve);
	}
	SetVBO(VertexAttrib::SPECULARSHININESS, specularShininess, 1);
	SetVBO(VertexAttrib::AMBIENTCOLOUR, ambientColour, 3);
	SetVBO(VertexAttrib::DIFFUSECOLOUR, diffuseColour, 3);
	SetVBO(VertexAttrib::SPECULARCOLOUR, specularColour, 3);
	SetVBO(VertexAttrib::DISSOLVE, dissolve, 1);
}

void Mesh::SetWidth(float width)
{
	m_width = width;
}

void Mesh::SetHeight(float height)
{
	m_height = height;
}

void Mesh::SetDepth(float depth)
{
	m_depth = depth;
}

float Mesh::GetWidth()
{
	return m_width;
}

float Mesh::GetHeight()
{
	return m_height;
}

float Mesh::GetDepth()
{
	return m_depth;
}
