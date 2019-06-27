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

CBullet::CBullet(CGameObject* owner, int mgrNum)
{
	this->owner = owner;
	this->mgrNum = mgrNum;
	m_Destroy = false;
}

CBullet::~CBullet()
{
}

void CBullet::Init(void)
{
	
	m_Model = new CModel;
	m_Model->Init("asset/bullet.obj");
	m_Position = { 0.0f,0.0f,0.0f };
	m_Collision = CManager::GetScene()->AddCollision<CColSphere>();
	m_Collision->Init(&m_Position);
	m_Collision->Attach(this);
	m_Collision->SetRadius(2.0f);
}

void CBullet::Uninit(void)
{
	CPlayer::DeleteBullet(mgrNum);
	delete m_Model;
}

void CBullet::Update(void)
{
	XMFLOAT3 delta;
	delta.z = 0.05f;
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

