#include "StaticRenderer.h"

StaticRenderer::StaticRenderer(std::unique_ptr<GameObject>& entity, const Mesh& mesh, Texture* texture) {
	m_tex = texture;
	m_entity = &entity;
	m_vao = std::make_unique<VertexArray>();
	SetVAO(mesh);
}

void StaticRenderer::SetVAO(const Mesh& mesh) {
	const VertexBuffer* vbo;

	if ((vbo = mesh.GetVBO(Mesh::VertexAttrib::POSITION)) != nullptr)
		m_vao->BindBuffer(*vbo, (GLint)Mesh::VertexAttrib::POSITION);

	if ((vbo = mesh.GetVBO(Mesh::VertexAttrib::NORMAL)) != nullptr)
		m_vao->BindBuffer(*vbo, (GLint)Mesh::VertexAttrib::NORMAL);

	if ((vbo = mesh.GetVBO(Mesh::VertexAttrib::TEXCOORD)) != nullptr)
		m_vao->BindBuffer(*vbo, (GLint)Mesh::VertexAttrib::TEXCOORD);
}

void StaticRenderer::Draw(Shader* shader) {
	auto& transform = m_entity->GetComponent<Transform>();

	//TODO: Material/Texture and Shader implementation
	m_tex->bind(0);

	shader->use();

	shader->setMat4fv(transform.GetGlobal(), "ModelMatrix");
	shader->setMat3fv(transform.GetNormal(), "NormalMatrix");

	m_vao->DrawArray();
}
