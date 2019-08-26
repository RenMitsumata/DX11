
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
#include "SkyDome.h"
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
	//m_Model->Init();
	m_Model->Init("asset/coaster.fbx");
	m_Shadow->Init();
	skydome = CManager::GetScene()->AddGameObject<SkyDome>(1);
	skydome->Init(50.0f);
	//playerUI = CManager::GetScene()->AddGameObject<CPolygon>(4);
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
	
	
	
	
	
	if (CInput::GetKeyPress(VK_LEFT)) {
		m_Rotation.x -= 0.05f;
	}
	
	if (CInput::GetKeyPress(VK_RIGHT)) {
		m_Rotation.x += 0.05f;
	}
	
	
	distance += 0.25f;
	pCource = CManager::GetScene()->GetGameObject<Cource>(1);
	m_Position = pCource->GetCource(distance);
	XMFLOAT3 upFor;
	XMStoreFloat3(&upFor,up);
	m_Position.x += upFor.x;
	m_Position.y += upFor.y;
	m_Position.z += upFor.z;
	
	XMVECTOR curFront = pCource->GetTild(distance);
	XMMATRIX curMat = XMMatrixIdentity();
	curMat = XMMatrixRotationY(m_Rotation.x);
	curFront = XMVector3TransformNormal(curFront, curMat);
	front = curFront;
	for (CBullet* bullet : _Bulletlist) {
		bullet->Update();
	}
	_Bulletlist.remove_if([](CBullet* bullet) {
		return bullet->isDestroy();
	});

	if (CInput::GetKeyTrigger(VK_SPACE)) {
		XMFLOAT3 velocity;
		XMStoreFloat3(&velocity, front);
		// 機体の正面を持ってきたい
		XMVECTOR vectorFront = { 0.0f,0.0f,1.0f };
		XMMATRIX matFront = XMMatrixIdentity();
		XMFLOAT3 YPR = pCource->GetPitchYawRoll(distance);
		matFront *= XMMatrixRotationRollPitchYaw(YPR.x, YPR.y, YPR.z);
		XMVector3TransformNormal(vectorFront, matFront);


		XMFLOAT3 calcPos;
		XMStoreFloat3(&calcPos, vectorFront + front * 2 + up);
		CBullet* bullet = new CBullet(this, m_Position + calcPos, velocity * 2.0f);	// m_Positionに大砲のフロントを足す
		bullet->Init();
		_Bulletlist.push_back(bullet);

	}
	

	//front = pCource->GetTild(distance);
	//float newPosY = CField::GetHeight(m_Position);
	//XMFLOAT4 newPosY = CField::GetNormal(&m_Position);
	//m_Position.y += 0.5f;
	
	
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
	/*
	XMFLOAT3 buffer;
	XMStoreFloat3(&buffer,front);
	*/
	XMFLOAT3 curRotate;
	XMStoreFloat3(&curRotate, front);

	XMMATRIX transform = XMMatrixIdentity();	
	transform *= XMMatrixRotationNormal(up, m_Rotation.x);
	XMFLOAT3 YPR = pCource->GetPitchYawRoll(distance);
	transform *= XMMatrixRotationRollPitchYaw(YPR.x, YPR.y, YPR.z);
	transform *= XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);
	m_Model->Draw(0,m_Position ,YPR, m_Rotation.x);

	
}

