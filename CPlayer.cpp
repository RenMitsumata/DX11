
#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "Scene.h"

#include "model.h"

#include "CShadow.h"
#include "input.h"
#include "CPlayer.h"




CPlayer::CPlayer()
{
}


CPlayer::~CPlayer()
{
}

void CPlayer::Init(void)
{
	m_Model = new CModel;
	m_Shadow = new CShadow(5.0f);
	m_Position = { 0.0f,0.5f,0.0f };
	m_Model->Init();
	m_Shadow->Init();
	bulletCnt = 0;
}

void CPlayer::Uninit(void)
{
	m_Shadow->Uninit();
	delete m_Shadow;
	m_Model->Uninit();
	delete m_Model;
}

void CPlayer::Update(void)
{
	m_Model->Update();
	for (int i = 0; i < bulletCnt; i++) {
		m_Bullet[i]->Update();
	}
	if (CInput::GetKeyTrigger(VK_SPACE)) {
		
		m_Bullet[bulletCnt] = new CBullet(this);
		m_Bullet[bulletCnt]->Init();
		bulletCnt++;
		
	}
	

	if (CInput::GetKeyPress('W')) {
		m_Position.z += 0.05f;
	}
	if (CInput::GetKeyPress('A')) {
		m_Position.x -= 0.05f;
	}
	if (CInput::GetKeyPress('S')) {
		m_Position.z -= 0.05f;
	}
	if (CInput::GetKeyPress('D')) {
		m_Position.x += 0.05f;
	}
	
}

void CPlayer::Draw(void)
{
	m_Shadow->Draw(m_Position);
	for (int i = 0; i < bulletCnt; i++) {
		m_Bullet[i]->Draw();
	}
	
	m_Model->Draw(m_Position);
}
