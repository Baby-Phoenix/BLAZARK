#include "DynamicRenderer.h"

DynamicRenderer::DynamicRenderer(entt::entity camera, entt::entity entity, const Mesh& mesh, Texture* texture, bool lightSource) {
	m_lightSource = lightSource;
	m_tex = texture;
	m_camera = camera;
	m_entity = entity;
	m_vao = std::make_unique<VertexArray>();

	if (!m_shader.size()) {
		m_shader.push_back(new Shader("Resource Files/Shaders/static_shader.vert", "Resource Files/Shaders/static_shader_untex_unlit.frag"));
		m_shader.push_back(new Shader("Resource Files/Shaders/static_shader.vert", "Resource Files/Shaders/static_shader_untex_lit.frag"));
		m_shader.push_back(new Shader("Resource Files/Shaders/static_shader.vert", "Resource Files/Shaders/static_shader_tex_unlit.frag"));
		m_shader.push_back(new Shader("Resource Files/Shaders/static_shader.vert", "Resource Files/Shaders/static_shader_tex_lit.frag"));
		m_shader.push_back(new Shader("Resource Files/Shaders/dynamic_shader.vert", "Resource Files/Shaders/dynamic_shader_untex_unlit.frag"));
	}

	const VertexBuffer* vbo;
	if ((vbo = mesh.GetVBO(Mesh::VertexAttrib::TEXCOORD)) != nullptr)
		m_vao->BindBuffer(*vbo, (GLint)VertexAttrib::TEXCOORD);

	UpdateVAO(mesh, mesh, 0.0f);
}

void DynamicRenderer::UpdateVAO(const Mesh& frameS, const Mesh& frameE, float t) {
	const VertexBuffer* vbo;

	if ((vbo = frameS.GetVBO(Mesh::VertexAttrib::POSITION)) != nullptr)
		m_vao->BindBuffer(*vbo, (GLint)VertexAttrib::POSITION0);
	if ((vbo = frameE.GetVBO(Mesh::VertexAttrib::POSITION)) != nullptr)
		m_vao->BindBuffer(*vbo, (GLint)VertexAttrib::POSITION1);

	if ((vbo = frameS.GetVBO(Mesh::VertexAttrib::NORMAL)) != nullptr)
		m_vao->BindBuffer(*vbo, (GLint)VertexAttrib::NORMAL0);
	if ((vbo = frameE.GetVBO(Mesh::VertexAttrib::NORMAL)) != nullptr)
		m_vao->BindBuffer(*vbo, (GLint)VertexAttrib::NORMAL1);

	if ((vbo = frameS.GetVBO(Mesh::VertexAttrib::SPECULARSHININESS)) != nullptr)
		m_vao->BindBuffer(*vbo, (GLint)VertexAttrib::SPECULARSHININESS0);

	if ((vbo = frameS.GetVBO(Mesh::VertexAttrib::AMBIENTCOLOUR)) != nullptr)
		m_vao->BindBuffer(*vbo, (GLint)VertexAttrib::AMBIENTCOLOUR0);

	if ((vbo = frameS.GetVBO(Mesh::VertexAttrib::DIFFUSECOLOUR)) != nullptr)
		m_vao->BindBuffer(*vbo, (GLint)VertexAttrib::DIFFUSECOLOUR0);

	if ((vbo = frameS.GetVBO(Mesh::VertexAttrib::SPECULARCOLOUR)) != nullptr)
		m_vao->BindBuffer(*vbo, (GLint)VertexAttrib::SPECULARCOLOUR0);

	if ((vbo = frameS.GetVBO(Mesh::VertexAttrib::DISSOLVE)) != nullptr)
		m_vao->BindBuffer(*vbo, (GLint)VertexAttrib::DISSOLVE0);

	m_t = t;
}

void DynamicRenderer::Draw() {
	auto& transform = GameObject::GetComponent<Transform>(m_entity);

	currShader = 4;

	m_shader[currShader]->use();

	if (m_tex != nullptr) {
		m_shader[currShader]->set1i(0, "albedo");
		m_tex->bind(0);
	}

	m_shader[currShader]->setVec3f(GameObject::GetComponent<Transform>(m_camera).GetLocalPos(), "camPos");
	m_shader[currShader]->setMat4fv(GameObject::GetComponent<Camera>(m_camera).GetViewProj(), "ViewProjection");
	m_shader[currShader]->setMat4fv(transform.GetGlobal(), "ModelMatrix");
	m_shader[currShader]->setMat3fv(transform.GetNormal(), "NormalMatrix");
	m_shader[currShader]->set1f(m_t, "t");

	m_vao->DrawArray();
	m_shader[currShader]->unuse();
	glBindVertexArray(GL_NONE);
}
