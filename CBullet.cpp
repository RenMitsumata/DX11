#include "main.h"
#include "renderer.h"
#include "manager.h"

#include "Scene.h"
#include "CCollision.h"
#include "CColSphere.h"
#include "GameObject.h"
#include "CPlayer.h"
#include "model.h"
#include "CBullet.h"


CBullet::CBullet()
{
}

CBullet::CBullet(CGameObject* owner,XMFLOAT3 pos)
{
	this->owner = owner;
	m_Position = pos;
	m_Destroy = false;
}

CBullet::~CBullet()
{
}

void CBullet::Init(void)
{
	
	m_Model = new CModel;
	m_Model->Init("asset/bullet.obj",m_Position);
	//m_Position = { 0.0f,0.0f,0.0f };
	m_Collision = CManager::GetScene()->AddCollision<CColSphere>();
	m_Collision->Init(&m_Position);
	m_Collision->SetRadius(2.0f);
	m_Collision->Attach(this);
}

void CBullet::Uninit(void)
{
	delete m_Model;
}

void CBullet::Update(void)
{
	XMFLOAT3 delta;
	delta.z = 0.1f;
	delta.y = 0.00f;
	delta.x = 0.00f;
	m_Position.x += delta.x;
	m_Position.y += delta.y;
	m_Position.z += delta.z;
	m_Model->Update(delta);
	
}

void CBullet::Draw(void)
{
	m_Model->Draw();
}

