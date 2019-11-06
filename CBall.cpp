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

		// 回転軸
		XMFLOAT3 vz = XMFLOAT3(1.0f, 0.0f, 0.0f);

		// 回転軸をXMVECTOR型に変換
		XMVECTOR axis = XMLoadFloat3(&vz);

		// 指定軸回転をXMVECTOR型として取得
		XMVECTOR rotation = XMQuaternionRotationAxis(axis, 0.2f);

		// Δtの回転(rotation)をm_Quaternionにかけ合わせる
		m_Quaternion = XMQuaternionMultiply(m_Quaternion, rotation);

		// 正規化
		m_Quaternion = XMQuaternionNormalize(m_Quaternion);

		// 移動
		m_Rotation.z += 50.0f;

	}
	if (CInput::GetKeyPress(VK_DOWN)) {

		// 回転軸
		XMFLOAT3 vz = XMFLOAT3(1.0f, 0.0f, 0.0f);

		// 回転軸をXMVECTOR型に変換
		XMVECTOR axis = XMLoadFloat3(&vz);

		// 指定軸回転をXMVECTOR型として取得
		XMVECTOR rotation = XMQuaternionRotationAxis(axis, -0.2f);

		// Δtの回転(rotation)をm_Quaternionにかけ合わせる
		m_Quaternion = XMQuaternionMultiply(m_Quaternion, rotation);

		// 正規化
		m_Quaternion = XMQuaternionNormalize(m_Quaternion);

		// 移動
		m_Rotation.z -= 50.0f;

	}
	if (CInput::GetKeyPress(VK_LEFT)) {

		// 回転軸
		XMFLOAT3 vz = XMFLOAT3(0.0f, 0.0f, 1.0f);

		// 回転軸をXMVECTOR型に変換
		XMVECTOR axis = XMLoadFloat3(&vz);

		// 指定軸回転をXMVECTOR型として取得
		XMVECTOR rotation = XMQuaternionRotationAxis(axis, 0.2f);

		// Δtの回転(rotation)をm_Quaternionにかけ合わせる
		m_Quaternion = XMQuaternionMultiply(m_Quaternion, rotation);

		// 正規化
		m_Quaternion = XMQuaternionNormalize(m_Quaternion);

		// 移動
		m_Rotation.x -= 50.0f;

	}
	if (CInput::GetKeyPress(VK_RIGHT)) {

		// 回転軸
		XMFLOAT3 vz = XMFLOAT3(0.0f, 0.0f, 1.0f);

		// 回転軸をXMVECTOR型に変換
		XMVECTOR axis = XMLoadFloat3(&vz);

		// 指定軸回転をXMVECTOR型として取得
		XMVECTOR rotation = XMQuaternionRotationAxis(axis, -0.2f);

		// Δtの回転(rotation)をm_Quaternionにかけ合わせる
		m_Quaternion = XMQuaternionMultiply(m_Quaternion, rotation);

		// 正規化
		m_Quaternion = XMQuaternionNormalize(m_Quaternion);

		// 移動
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