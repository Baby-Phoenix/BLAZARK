#include "StaticRenderer.h"

StaticRenderer::StaticRenderer(GameObject* camera, GameObject* entity, const Mesh& mesh, Texture* texture) {
	m_tex = texture;
	m_camera = camera;
	m_entity = entity;
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
	shader->use();

	m_tex->bind(0);
	shader->setVec3f(m_camera->GetComponent<Transform>().GetLocalPos(), "camPos");
	shader->setMat4fv(m_camera->GetComponent<Camera>().GetViewProj(), "ViewProjection");
	shader->setMat4fv(transform.GetGlobal(), "ModelMatrix");
	shader->setMat3fv(transform.GetNormal(), "NormalMatrix");

	
	m_vao->DrawArray();
	//shader->unuse();
	//glBindVertexArray(GL_NONE);

}
