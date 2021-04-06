#include "StaticRenderer.h"

std::vector<Shader*> StaticRenderer::m_shader;

StaticRenderer::StaticRenderer(entt::entity camera, entt::entity entity, const Mesh& mesh, Texture* texture, bool lightSource) {
	m_lightSource = lightSource;
	m_tex = texture;
	m_camera = camera;
	m_entity = entity;
	m_vao = std::make_unique<VertexArray>();

	if (!m_shader.size()) {
		m_shader.push_back(new Shader("Resource Files/Shaders/static_shader.vert", "Resource Files/Shaders/untextured_unlit.frag"));
		m_shader.push_back(new Shader("Resource Files/Shaders/static_shader.vert", "Resource Files/Shaders/untextured_lit.frag"));
		m_shader.push_back(new Shader("Resource Files/Shaders/static_shader.vert", "Resource Files/Shaders/textured_unlit.frag"));
		m_shader.push_back(new Shader("Resource Files/Shaders/static_shader.vert", "Resource Files/Shaders/textured_lit.frag"));
		m_shader.push_back(new Shader("Resource Files/Shaders/dynamic_shader.vert", "Resource Files/Shaders/untextured_unlit.frag"));
		m_shader.push_back(new Shader("Resource Files/Shaders/depth.vert", "Resource Files/Shaders/depth.frag", "Resource Files/Shaders/depth.geom"));
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

	if ((vbo = mesh.GetVBO(Mesh::VertexAttrib::SPECULARSHININESS)) != nullptr)
		m_vao->BindBuffer(*vbo, (GLint)Mesh::VertexAttrib::SPECULARSHININESS);

	if ((vbo = mesh.GetVBO(Mesh::VertexAttrib::AMBIENTCOLOUR)) != nullptr)
		m_vao->BindBuffer(*vbo, (GLint)Mesh::VertexAttrib::AMBIENTCOLOUR);

	if ((vbo = mesh.GetVBO(Mesh::VertexAttrib::DIFFUSECOLOUR)) != nullptr)
		m_vao->BindBuffer(*vbo, (GLint)Mesh::VertexAttrib::DIFFUSECOLOUR);

	if ((vbo = mesh.GetVBO(Mesh::VertexAttrib::SPECULARCOLOUR)) != nullptr)
		m_vao->BindBuffer(*vbo, (GLint)Mesh::VertexAttrib::SPECULARCOLOUR);

	if ((vbo = mesh.GetVBO(Mesh::VertexAttrib::DISSOLVE)) != nullptr)
		m_vao->BindBuffer(*vbo, (GLint)Mesh::VertexAttrib::DISSOLVE);
}

void StaticRenderer::toggleTexture() {
	m_textureToggle = !m_textureToggle;
}

void StaticRenderer::Draw(std::vector<glm::mat4> shadowTransformations, FrameBuffer* depthBuffer, bool depthRender) {
	auto& transform = GameObject::GetComponent<Transform>(m_entity);

	if (depthRender) {
		currShader = 5;
	}
	else {
		if (m_tex == nullptr || !m_textureToggle) {
			if (!m_lightSource)
				currShader = 0;
			else
				currShader = 1;
		}
		else if (m_textureToggle && m_tex != nullptr) {
			if (!m_lightSource)
				currShader = 2;
			else
				currShader = 3;
		}
	}

	m_shader[currShader]->use();

	if (!depthRender) {
		if (m_tex != nullptr) {
			m_shader[currShader]->set1i(0, "albedo");
			m_tex->bind(0);
		}

		m_shader[currShader]->setVec3f(GameObject::GetComponent<Transform>(m_camera).GetLocalPos(), "camPos");
		m_shader[currShader]->setMat4fv(GameObject::GetComponent<Camera>(m_camera).GetViewProj(), "ViewProjection");
		m_shader[currShader]->setMat3fv(transform.GetNormal(), "NormalMatrix");
		m_shader[currShader]->set1i(false, "Shadows");
	}
	else if (depthRender) {
		for (int i = 0; i < 6; ++i)
			m_shader[currShader]->setMat4fv(shadowTransformations[i], ("ShadowMatrices[" + std::to_string(i) + "]").c_str());
	}
	m_shader[currShader]->setMat4fv(transform.GetGlobal(), "ModelMatrix");
	m_shader[currShader]->set1f(1000.0f, "Far_Plane");

	if (depthBuffer != nullptr)
		//depthBuffer->BindDepthAsTexture(15);

	m_vao->DrawArray();

	if (depthBuffer != nullptr)
		//depthBuffer->UnbindTexture(15);
	m_shader[currShader]->unuse();
	glBindVertexArray(GL_NONE);
}
