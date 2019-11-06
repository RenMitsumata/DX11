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

		// ��]��
		XMFLOAT3 vz = XMFLOAT3(1.0f, 0.0f, 0.0f);

		// ��]����XMVECTOR�^�ɕϊ�
		XMVECTOR axis = XMLoadFloat3(&vz);

		// �w�莲��]��XMVECTOR�^�Ƃ��Ď擾
		XMVECTOR rotation = XMQuaternionRotationAxis(axis, 0.2f);

		// ��t�̉�](rotation)��m_Quaternion�ɂ������킹��
		m_Quaternion = XMQuaternionMultiply(m_Quaternion, rotation);

		// ���K��
		m_Quaternion = XMQuaternionNormalize(m_Quaternion);

		// �ړ�
		m_Rotation.z += 50.0f;

	}
	if (CInput::GetKeyPress(VK_DOWN)) {

		// ��]��
		XMFLOAT3 vz = XMFLOAT3(1.0f, 0.0f, 0.0f);

		// ��]����XMVECTOR�^�ɕϊ�
		XMVECTOR axis = XMLoadFloat3(&vz);

		// �w�莲��]��XMVECTOR�^�Ƃ��Ď擾
		XMVECTOR rotation = XMQuaternionRotationAxis(axis, -0.2f);

		// ��t�̉�](rotation)��m_Quaternion�ɂ������킹��
		m_Quaternion = XMQuaternionMultiply(m_Quaternion, rotation);

		// ���K��
		m_Quaternion = XMQuaternionNormalize(m_Quaternion);

		// �ړ�
		m_Rotation.z -= 50.0f;

	}
	if (CInput::GetKeyPress(VK_LEFT)) {

		// ��]��
		XMFLOAT3 vz = XMFLOAT3(0.0f, 0.0f, 1.0f);

		// ��]����XMVECTOR�^�ɕϊ�
		XMVECTOR axis = XMLoadFloat3(&vz);

		// �w�莲��]��XMVECTOR�^�Ƃ��Ď擾
		XMVECTOR rotation = XMQuaternionRotationAxis(axis, 0.2f);

		// ��t�̉�](rotation)��m_Quaternion�ɂ������킹��
		m_Quaternion = XMQuaternionMultiply(m_Quaternion, rotation);

		// ���K��
		m_Quaternion = XMQuaternionNormalize(m_Quaternion);

		// �ړ�
		m_Rotation.x -= 50.0f;

	}
	if (CInput::GetKeyPress(VK_RIGHT)) {

		// ��]��
		XMFLOAT3 vz = XMFLOAT3(0.0f, 0.0f, 1.0f);

		// ��]����XMVECTOR�^�ɕϊ�
		XMVECTOR axis = XMLoadFloat3(&vz);

		// �w�莲��]��XMVECTOR�^�Ƃ��Ď擾
		XMVECTOR rotation = XMQuaternionRotationAxis(axis, -0.2f);

		// ��t�̉�](rotation)��m_Quaternion�ɂ������킹��
		m_Quaternion = XMQuaternionMultiply(m_Quaternion, rotation);

		// ���K��
		m_Quaternion = XMQuaternionNormalize(m_Quaternion);

		// �ړ�
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