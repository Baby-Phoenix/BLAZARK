#include "Sprite2D.h"

Mesh* Sprite2D::m_planeMesh = nullptr;
VertexArray* Sprite2D::VAO = nullptr;
Shader* Sprite2D::m_Sprite2D_shader = nullptr;

Sprite2D::Sprite2D(Texture* tex, entt::entity entity, float width, float height, float transparency)
{

	m_texture = tex;
	m_width = width;
	m_height = height;
	m_transparency = transparency;
	m_entity = entity;

	if (m_Sprite2D_shader == nullptr) 
		m_Sprite2D_shader = new Shader("Resource Files/Shaders/Sprite2D_vert.glsl", "Resource Files/Shaders/Sprite2D_frag.glsl");
	

	if (m_planeMesh == nullptr) {
	
		m_planeMesh = new Mesh();
		loadOBJ("Resource Files/OBJFiles/plane.obj", *m_planeMesh);
	}

	if (VAO == nullptr) {
		VAO = new VertexArray();

		const VertexBuffer* vbo;

		if ((vbo = m_planeMesh->GetVBO(Mesh::VertexAttrib::POSITION)) != nullptr)
			VAO->BindBuffer(*vbo, (GLint)Mesh::VertexAttrib::POSITION);

		if ((vbo = m_planeMesh->GetVBO(Mesh::VertexAttrib::TEXCOORD)) != nullptr)
			VAO->BindBuffer(*vbo, (GLint)Mesh::VertexAttrib::TEXCOORD);

		if ((vbo = m_planeMesh->GetVBO(Mesh::VertexAttrib::NORMAL)) != nullptr)
			VAO->BindBuffer(*vbo, (GLint)Mesh::VertexAttrib::NORMAL);
	}

}


void Sprite2D::Draw(Camera* cam)
{
	auto& transform = GameObject::GetComponent<Transform>(m_entity);

	cam->OrthographicProj(1.0f, 10000.0f, -100.0f, 100.0f, -100.0f, 100.0f);

	transform.SetLocalScale(glm::vec3(m_width,1, m_height));
	transform.SetLocalRot(glm::vec3(90, 0, 180));
	transform.UpdateGlobal();
	//shader stuff
	m_Sprite2D_shader->use();

	m_Sprite2D_shader->set1i(0, "uTex");
	m_texture->bind(0);
	m_Sprite2D_shader->set1f(m_transparency, "uTransparency");
	m_Sprite2D_shader->setMat4fv(cam->GetViewProj(), "ViewProjection");
	m_Sprite2D_shader->setMat4fv(transform.GetGlobal(), "ModelMatrix");
	m_Sprite2D_shader->setMat4fv(cam->GetProj(), "Projection");
	m_Sprite2D_shader->setMat4fv(cam->GetView(), "View");
	//binds and draws
	VAO->DrawArray();

	//unbinds the vao
	glBindVertexArray(GL_NONE);

	cam->PerspectiveProj(1.0f, 100000.0f, Application::GetWindowWidth() / Application::GetWindowHeight(), 100.0f);
	m_texture->unbind();

}
