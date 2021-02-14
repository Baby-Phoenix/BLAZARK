#include "StaticRenderer.h"

std::vector<Shader*> StaticRenderer::m_shader;

StaticRenderer::StaticRenderer(entt::entity camera, entt::entity entity, const Mesh& mesh, Texture* texture) {
	m_tex = texture;
	m_camera = camera;
	m_entity = entity;
	m_vao = std::make_unique<VertexArray>();
	
	if (!m_shader.size()) {
			m_shader.push_back(new Shader("Resource Files/Shaders/static_shader.vert", "Resource Files/Shaders/static_shader_untextured.frag"));
			m_shader.push_back(new Shader("Resource Files/Shaders/static_shader.vert", "Resource Files/Shaders/static_shader_textured.frag"));
	}

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

void StaticRenderer::toggleTexture() {
	textureToggle = !textureToggle;
}

void StaticRenderer::Draw() {
	auto& transform = GameObject::GetComponent<Transform>(m_entity);

	//TODO: Material/Texture and Shader implementation
	if (m_tex == nullptr || !textureToggle)
		currShader = 0;
	else if (textureToggle && m_tex != nullptr)
		currShader = 1;

	m_shader[currShader]->use();

	if (m_tex != nullptr) {
		m_shader[currShader]->set1i(0, "albedo");
		m_tex->bind(0);
	}

	m_shader[currShader]->setVec3f(GameObject::GetComponent<Transform>(m_camera).GetLocalPos(), "camPos");
	m_shader[currShader]->setMat4fv(GameObject::GetComponent<Camera>(m_camera).GetViewProj(), "ViewProjection");
	m_shader[currShader]->setMat4fv(transform.GetGlobal(), "ModelMatrix");
	m_shader[currShader]->setMat3fv(transform.GetNormal(), "NormalMatrix");
	
	m_vao->DrawArray();
	m_shader[currShader]->unuse();
	glBindVertexArray(GL_NONE);
}
