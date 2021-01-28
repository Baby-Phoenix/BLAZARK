#include "Sprite2D.h"

Mesh* Sprite2D::m_planeMesh = nullptr;

Sprite2D::Sprite2D(Texture* tex, GameObject* entity, float width, float height, float transparency)
{

	m_texture = tex;
	m_width = width;
	m_height = height;
	m_transparency = transparency;
	m_entity = entity;

	if (m_planeMesh == nullptr) {
	
		m_planeMesh = new Mesh();
		loadOBJ("Resource Files/OBJFiles/plane.obj", *m_planeMesh);
	}

}

void Sprite2D::Draw(Shader* shader, Camera* cam)
{
	auto& transform = m_entity->GetComponent<Transform>();

	

	VertexArray* temp_vao = new VertexArray();

		const VertexBuffer* vbo;

	if ((vbo = m_planeMesh->GetVBO(Mesh::VertexAttrib::POSITION)) != nullptr)
		temp_vao->BindBuffer(*vbo, (GLint)Mesh::VertexAttrib::POSITION);

	if ((vbo = m_planeMesh->GetVBO(Mesh::VertexAttrib::TEXCOORD)) != nullptr)
		temp_vao->BindBuffer(*vbo, (GLint)Mesh::VertexAttrib::TEXCOORD);

	if ((vbo = m_planeMesh->GetVBO(Mesh::VertexAttrib::NORMAL)) != nullptr)
		temp_vao->BindBuffer(*vbo, (GLint)Mesh::VertexAttrib::NORMAL);
	//shader stuff
	shader->use();

	shader->set1i(0, "uTex");
	m_texture->bind(0);
	shader->set1f(m_transparency, "uTransparency");
	shader->setMat4fv(cam->GetViewProj(), "ViewProjection");
	shader->setMat4fv(transform.GetGlobal(), "ModelMatrix");
	shader->setMat4fv(cam->GetProj(), "Projection");

	//binds and draws
	temp_vao->DrawArray();

	//unbinds the vao
	glBindVertexArray(GL_NONE);

	m_texture->unbind();

	shader->unuse();
	delete temp_vao;
}
