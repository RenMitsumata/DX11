
#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "Scene.h"

#include "model.h"
#include "CShadow.h"
#include "Field.h"
#include "input.h"
#include <list>
#include "CPlayer.h"

CPlayer::CPlayer()
{
}


CPlayer::~CPlayer()
{
}

void CPlayer::Init(void)
{
	m_Model = new CModel();
	m_Shadow = new CShadow(5.0f);
	m_Position = { 0.0f,0.5f,0.0f };
	const XMFLOAT3 startFront = XMFLOAT3(0.0f, 0.0f, 1.0f);
	front = XMLoadFloat3(&startFront);
	m_Model->Init();
	//m_Model->Init("asset/coaster.obj");
	m_Shadow->Init();
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
	if (CInput::GetKeyTrigger(VK_SPACE)) {
		
		CBullet* bullet = new CBullet(this,m_Position);
		bullet->Init();
		_Bulletlist.push_back(bullet);
		
	}
	for (CBullet* bullet : _Bulletlist) {
		bullet->Update();
	}
	_Bulletlist.remove_if([](CBullet* bullet) {
		return bullet->isDestroy();
	});
	

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
	if (CInput::GetKeyPress(VK_RETURN)) {
		m_Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	}
	
	float newPosY = CField::GetHeight(m_Position);
	//XMFLOAT4 newPosY = CField::GetNormal(&m_Position);
	m_Position.y = newPosY + 0.5f;
	
	
}

void CPlayer::Draw(void)
{
	m_Shadow->Draw(m_Position);
	
	for (CBullet* bullet : _Bulletlist) {
		bullet->Draw();
	}
	
	
	m_Model->Draw(m_Position);
}

