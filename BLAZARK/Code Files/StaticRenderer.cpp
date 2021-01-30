#include "StaticRenderer.h"

Shader* StaticRenderer::m_static_shader = nullptr;

StaticRenderer::StaticRenderer(GameObject* camera, GameObject* entity, const Mesh& mesh, Texture* texture) {
	m_tex = texture;
	m_camera = camera;
	m_entity = entity;
	m_vao = std::make_unique<VertexArray>();
	
	if (m_static_shader == nullptr)
		m_static_shader = new Shader("Resource Files/Shaders/static_shader_vert.glsl", "Resource Files/Shaders/static_shader_frag.glsl");

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

void StaticRenderer::Draw() {
	auto& transform = m_entity->GetComponent<Transform>();

	//TODO: Material/Texture and Shader implementation
	m_static_shader->use();

	m_tex->bind(0);
	m_static_shader->setVec3f(m_camera->GetComponent<Transform>().GetLocalPos(), "camPos");
	m_static_shader->setMat4fv(m_camera->GetComponent<Camera>().GetViewProj(), "ViewProjection");
	m_static_shader->setMat4fv(transform.GetGlobal(), "ModelMatrix");
	m_static_shader->setMat3fv(transform.GetNormal(), "NormalMatrix");

	
	m_vao->DrawArray();
	//shader->unuse();
	//glBindVertexArray(GL_NONE);

}
