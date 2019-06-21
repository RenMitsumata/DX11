#include "main.h"
#include "renderer.h"
#include "GameObject.h"
#include "model.h"
#include "CBullet.h"


CBullet::CBullet()
{
}


CBullet::~CBullet()
{
}

void CBullet::Init(void)
{
	
	m_Model = new CModel;
	m_Model->Init("asset/bullet.obj");
	m_Position = { 0.0f,0.0f,0.0f };
}

void CBullet::Uninit(void)
{
	delete m_Model;
}

void CBullet::Update(void)
{
	m_Position.z = 0.05f;
	m_Position.y = 0.05f;
	m_Model->Update(m_Position);
}

void CBullet::Draw(void)
{
	m_Model->Draw();
}

