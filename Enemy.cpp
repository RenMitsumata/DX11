#include "main.h"
#include "renderer.h"
#include "GameObject.h"
#include "model.h"

#include "CShadow.h"

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
	m_Position = { 0.0f,0.5f,0.0f };
	m_Model->Init();
	m_Shadow->Init();
}

void CEnemy::Uninit(void)
{
}

void CEnemy::Update(void)
{
}

void CEnemy::Draw(void)
{
}
