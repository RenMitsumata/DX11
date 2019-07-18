
#include "main.h"
#include "renderer.h"
#include "manager.h"


#include "Scene.h"
#include "GameObject.h"
#include "CCollision.h"
#include "CColSphere.h"
#include "CBullet.h"
#include "camera.h"
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
	m_Camera = CManager::GetScene()->AddGameObject<CCamera>(0);
	m_Position = { 0.0f,0.5f,0.0f };
	const XMFLOAT3 startFront = XMFLOAT3(0.0f, 0.0f, 1.0f);
	front = XMLoadFloat3(&startFront);
	const XMFLOAT3 startUp = XMFLOAT3(0.0f, 1.0f, 0.0f);
	up = XMLoadFloat3(&startUp);
	m_Model->Init();
	m_Model->Init("asset/miku_01.obj");
	m_Shadow->Init();
	distance = 0.0f;
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
	XMFLOAT3 frontPos;
	XMFLOAT3 upPos;
	XMStoreFloat3(&frontPos, front);
	XMStoreFloat3(&upPos, up);
	frontPos = frontPos * (-6.0f) + upPos * 2.0f;
	frontPos = m_Position + frontPos;
	m_Camera->Update(frontPos,front,up);
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
	
	/*
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
	*/
	distance += 0.25f;
	pCource = CManager::GetScene()->GetGameObject<Cource>(1);
	m_Position = pCource->GetCource(distance);
	front = pCource->GetTild(distance);
	//float newPosY = CField::GetHeight(m_Position);
	//XMFLOAT4 newPosY = CField::GetNormal(&m_Position);
	m_Position.y += 0.5f;
	
	
}

void CPlayer::Draw(void)
{
	m_Shadow->Draw(m_Position);
	
	for (CBullet* bullet : _Bulletlist) {
		bullet->Draw();
	}
	if (pCource == NULL) {
		pCource = CManager::GetScene()->GetGameObject<Cource>(1);
	}
	m_Model->Draw(m_Position, pCource->GetPitchYawRoll(distance),CModel::e_FILEOBJ);
}

