#define NOMINMAX

#include "main.h"
#include "renderer.h"
#include "texture.h"
#include "GameObject.h"
#include "model.h"
#include "input.h"
#include <string>
#include <vector>

#include <unordered_map>
#include <assimp\cimport.h>
#include <assimp\scene.h>
#include <assimp\postprocess.h>
#include <assimp\matrix4x4.h>
#pragma comment (lib,"assimp.lib")

#include "CBall.h"



CBall::CBall()
{
}


CBall::~CBall()
{
}

void CBall::Init() {
	m_Quaternion = XMQuaternionIdentity();
	m_Position = XMFLOAT3(0.0f, 100.0f, 0.0f);
	m_Model = new CModel;
	m_Model->Load("asset/ball.fbx");
}

void CBall::Update() {
	if (CInput::GetKeyPress(VK_UP)) {

		// ‰ñ“]Ž²
		XMFLOAT3 vz = XMFLOAT3(1.0f, 0.0f, 0.0f);

		// ‰ñ“]Ž²‚ðXMVECTORŒ^‚É•ÏŠ·
		XMVECTOR axis = XMLoadFloat3(&vz);

		// Žw’èŽ²‰ñ“]‚ðXMVECTORŒ^‚Æ‚µ‚ÄŽæ“¾
		XMVECTOR rotation = XMQuaternionRotationAxis(axis, 0.2f);

		// ƒ¢t‚Ì‰ñ“](rotation)‚ðm_Quaternion‚É‚©‚¯‡‚í‚¹‚é
		m_Quaternion = XMQuaternionMultiply(m_Quaternion, rotation);

		// ³‹K‰»
		m_Quaternion = XMQuaternionNormalize(m_Quaternion);

		// ˆÚ“®
		m_Rotation.z += 50.0f;

	}
	if (CInput::GetKeyPress(VK_DOWN)) {

		// ‰ñ“]Ž²
		XMFLOAT3 vz = XMFLOAT3(1.0f, 0.0f, 0.0f);

		// ‰ñ“]Ž²‚ðXMVECTORŒ^‚É•ÏŠ·
		XMVECTOR axis = XMLoadFloat3(&vz);

		// Žw’èŽ²‰ñ“]‚ðXMVECTORŒ^‚Æ‚µ‚ÄŽæ“¾
		XMVECTOR rotation = XMQuaternionRotationAxis(axis, -0.2f);

		// ƒ¢t‚Ì‰ñ“](rotation)‚ðm_Quaternion‚É‚©‚¯‡‚í‚¹‚é
		m_Quaternion = XMQuaternionMultiply(m_Quaternion, rotation);

		// ³‹K‰»
		m_Quaternion = XMQuaternionNormalize(m_Quaternion);

		// ˆÚ“®
		m_Rotation.z -= 50.0f;

	}
	if (CInput::GetKeyPress(VK_LEFT)) {

		// ‰ñ“]Ž²
		XMFLOAT3 vz = XMFLOAT3(0.0f, 0.0f, 1.0f);

		// ‰ñ“]Ž²‚ðXMVECTORŒ^‚É•ÏŠ·
		XMVECTOR axis = XMLoadFloat3(&vz);

		// Žw’èŽ²‰ñ“]‚ðXMVECTORŒ^‚Æ‚µ‚ÄŽæ“¾
		XMVECTOR rotation = XMQuaternionRotationAxis(axis, 0.2f);

		// ƒ¢t‚Ì‰ñ“](rotation)‚ðm_Quaternion‚É‚©‚¯‡‚í‚¹‚é
		m_Quaternion = XMQuaternionMultiply(m_Quaternion, rotation);

		// ³‹K‰»
		m_Quaternion = XMQuaternionNormalize(m_Quaternion);

		// ˆÚ“®
		m_Rotation.x -= 50.0f;

	}
	if (CInput::GetKeyPress(VK_RIGHT)) {

		// ‰ñ“]Ž²
		XMFLOAT3 vz = XMFLOAT3(0.0f, 0.0f, 1.0f);

		// ‰ñ“]Ž²‚ðXMVECTORŒ^‚É•ÏŠ·
		XMVECTOR axis = XMLoadFloat3(&vz);

		// Žw’èŽ²‰ñ“]‚ðXMVECTORŒ^‚Æ‚µ‚ÄŽæ“¾
		XMVECTOR rotation = XMQuaternionRotationAxis(axis, -0.2f);

		// ƒ¢t‚Ì‰ñ“](rotation)‚ðm_Quaternion‚É‚©‚¯‡‚í‚¹‚é
		m_Quaternion = XMQuaternionMultiply(m_Quaternion, rotation);

		// ³‹K‰»
		m_Quaternion = XMQuaternionNormalize(m_Quaternion);

		// ˆÚ“®
		m_Rotation.x += 50.0f;
	}
}

void CBall::Draw() {

	XMMATRIX world;
	world = XMMatrixIdentity();
	world *= XMMatrixScaling(0.01f,0.01f,0.01f);
	world *= XMMatrixRotationQuaternion(m_Quaternion);
	world *= XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);
	CRenderer::SetWorldMatrix(&world);
	m_Model->Draw(true);
}

void CBall::Uninit() {
	if (m_Model) {
		m_Model->Unload();
		delete m_Model;
	}
}