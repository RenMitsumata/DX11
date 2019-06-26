#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "Scene.h"
#include "GameObject.h"
#include "model.h"

#include "CShadow.h"
#include "CCollision.h"
#include "CColSphere.h"
#include "Enemy.h"



CEnemy::CEnemy()
{
}


CEnemy::~CEnemy()
{
}

void CEnemy::Init(void)
{
	m_Model = new CModel;
	m_Shadow = new CShadow(5.0f);
	m_Position = { 0.0f,0.5f,10.0f };
	m_Col = CManager::GetScene()->AddCollision<CColSphere>();
	m_Col->Init(&m_Position);
	m_Col->Attach(this);
	m_Model->Init();
	m_Shadow->Init();
}

void CEnemy::Uninit(void)
{
	delete m_Shadow;
	delete m_Model;
}

void CEnemy::Update(void)
{
}

void CEnemy::Draw(void)
{
	m_Shadow->Draw(m_Position);

	m_Model->Draw(m_Position);
}
