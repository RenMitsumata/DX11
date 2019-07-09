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

int CEnemy::Cnt = 0;

CEnemy::CEnemy()
{
}


CEnemy::~CEnemy()
{
}

void CEnemy::Init(void)
{
	m_Model = new CModel;
	m_Model->Init("asset/miku_01.obj",m_Position);
	m_Shadow = new CShadow(5.0f);
	m_Position = { 0.0f,0.5f,(Cnt + 1) * 5.0f };
	m_Col = CManager::GetScene()->AddCollision<CColSphere>();
	m_Col->Init(&m_Position);
	m_Col->Attach(this);
	m_Shadow->Init();
	Cnt++;
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
