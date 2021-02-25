#pragma once

#include "glm.hpp"
#include "Vertex.h"
#include "Material.h"

#include <string>
#include <memory>

class Mesh {
public:
	enum class VertexAttrib {
		POSITION = 0,
		NORMAL,
		TEXCOORD,
		SPECULARSHININESS,
		AMBIENTCOLOUR,
		DIFFUSECOLOUR,
		SPECULARCOLOUR,
		DISSOLVE
	};

	Mesh() = default;
	virtual ~Mesh() = default;

	// Getters //
	const VertexBuffer* GetVBO(VertexAttrib bufferAttrib) const;

	// Setters //
	template<typename dataType>
	void SetVBO(VertexAttrib bufferAttrib, const std::vector<dataType>& data, GLint typeLen);
	void SetPositions(const std::vector<glm::vec3>& positions);
	void SetNormals(const std::vector<glm::vec3>& normals);
	void SetTexCoords(const std::vector<glm::vec2>& texCoords);
	void SetMaterials(const std::vector<Material>& materials);

	float width, height, depth;

private:
	std::map<VertexAttrib, std::unique_ptr<VertexBuffer>> m_vboDict;
	
};

template<typename dataType>
inline void Mesh::SetVBO(VertexAttrib bufferAttrib, const std::vector<dataType>& data, GLint typeLen) {
	if (data.size() == 0) {
		m_vboDict.erase(bufferAttrib);
	}
	else {
		auto iterator = m_vboDict.find(bufferAttrib);

		if (iterator == m_vboDict.end()) {
			m_vboDict.insert({ bufferAttrib, std::make_unique<VertexBuffer>(data, typeLen) });
		}
		else {
			iterator->second->UpdateBufferData(data);
		}
	}
}
