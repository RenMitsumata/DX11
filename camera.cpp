#include "main.h"
#include "renderer.h"
#include "GameObject.h"
#include "input.h"
#include "camera.h"





void CCamera::Init()
{

	m_Position = XMFLOAT3( 0.0f, 5.0f, -10.0f );
	m_Rotation = XMFLOAT3( 0.5f, 0.0f, 0.0f );

	worldMatrix = XMMatrixTranslation(0.0f, 0.0f, 0.0f);

	at = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	eye = XMVectorSet(2.0f, 0.0f, -2.0f, 0.0f);
	eye = XMVector3Normalize(eye);
	eye = at - (at - eye) * FOCUS_LENGTH;
	up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMVector3Normalize(up);

	fov = XMConvertToRadians(45.0f);
	aspect = (float)SCREEN_WIDTH / SCREEN_HEIGHT;
	nearZ = 0.1f;
	farZ = 100.0f;
	
	m_Viewport.left = 0;
	m_Viewport.top = 0;
	m_Viewport.right = SCREEN_WIDTH;
	m_Viewport.bottom = SCREEN_HEIGHT;

}


void CCamera::Uninit()
{


}


void CCamera::Update()
{
	
	if (CInput::GetKeyPress(VK_UP)) {
		 XMVECTOR delta = XMVector3Normalize(at - eye) * 0.2f;
		 eye += delta;
		 at += delta;
	}
	if (CInput::GetKeyPress(VK_DOWN)) {
		XMVECTOR delta = XMVector3Normalize(at - eye) * 0.2f;
		eye -= delta;
		at -= delta;
	}
	if (CInput::GetKeyPress(VK_LEFT)) {
		XMVECTOR delta = XMVector3Cross(XMVector3Normalize(at - eye),up) * 0.2f;
		eye += delta;
		at += delta;
	}
	if (CInput::GetKeyPress(VK_RIGHT)) {
		XMVECTOR delta = XMVector3Cross(XMVector3Normalize(at - eye),up) * 0.2f;
		eye -= delta;
		at -= delta;
	}
	if (CInput::GetKeyPress(VK_UP) && (CInput::GetKeyPress(VK_LSHIFT) || CInput::GetKeyPress(VK_RSHIFT))) {
		XMVECTOR delta = up * 0.2f;
		eye += delta;
		at += delta;
	}
	if (CInput::GetKeyPress(VK_DOWN) && (CInput::GetKeyPress(VK_LSHIFT) || CInput::GetKeyPress(VK_RSHIFT))) {
		XMVECTOR delta = up * 0.2f;
		eye -= delta;
		at -= delta;
	}

	if (CInput::GetKeyPress(VK_NUMPAD8)) {
		// カメラ→注視点のベクトルを求める
		XMVECTOR front = eye - at;
		front = XMVector3Normalize(front);
		// eyeを回転させる
		XMVECTOR right = XMVector3Cross(up,front);
		XMVECTOR delta = XMQuaternionRotationAxis(right, -0.05f);
		XMVECTOR move = XMVector3Rotate(front, delta);
		// 位置を更新
		eye = at + move * FOCUS_LENGTH;
		up = XMVector3Cross(front,right);
		
	}
	if (CInput::GetKeyPress(VK_NUMPAD2)) {
		// カメラ→注視点のベクトルを求める
		XMVECTOR front = eye - at;
		front = XMVector3Normalize(front);
		// eyeを回転させる
		XMVECTOR right = XMVector3Cross(up, front);
		XMVECTOR delta = XMQuaternionRotationAxis(right, 0.05f);
		XMVECTOR move = XMVector3Rotate(front, delta);
		// 位置を更新
		eye = at + move * FOCUS_LENGTH;
		up = XMVector3Cross(front, right);

	}
	if (CInput::GetKeyPress(VK_NUMPAD4)) {
		// カメラ→注視点のベクトルを求める
		XMVECTOR front = eye - at;
		front = XMVector3Normalize(front);
		// eyeを回転させる
	//	XMVECTOR right = XMVector3Cross(up, front);
		XMVECTOR delta = XMQuaternionRotationAxis(up, 0.05f);
		XMVECTOR move = XMVector3Rotate(front, delta);
		// 位置を更新
		eye = at + move * FOCUS_LENGTH;
	//	up = XMVector3Cross(front, right);

	}
	if (CInput::GetKeyPress(VK_NUMPAD6)) {
		// カメラ→注視点のベクトルを求める
		XMVECTOR front = eye - at;
		front = XMVector3Normalize(front);
		// eyeを回転させる
	//	XMVECTOR right = XMVector3Cross(up, front);
		XMVECTOR delta = XMQuaternionRotationAxis(up, -0.05f);
		XMVECTOR move = XMVector3Rotate(front, delta);
		// 位置を更新
		eye = at + move * FOCUS_LENGTH;
	//	up = XMVector3Cross(front, right);

	}
	
	
}

void CCamera::Update(XMFLOAT3 newPos)
{
	eye = XMLoadFloat3(&newPos);
}

void CCamera::Update(XMFLOAT3 newPos,XMVECTOR frontVec,XMVECTOR upVec)
{
	eye = XMLoadFloat3(&newPos);
	at = eye + frontVec * FOCUS_LENGTH;
	up = upVec;
}



void CCamera::Draw()
{
	/*
	XMMATRIX	m_ViewMatrix;
	XMMATRIX	m_InvViewMatrix;
	XMMATRIX	m_ProjectionMatrix;
	*/
	XMMATRIX viewMatrix = XMMatrixLookAtLH(eye, at, up);
	XMMATRIX projMatrix = XMMatrixPerspectiveFovLH(fov, aspect, nearZ, farZ);


	// ビューポート設定
	D3D11_VIEWPORT dxViewport;
	dxViewport.Width = (float)(m_Viewport.right - m_Viewport.left);
	dxViewport.Height = (float)(m_Viewport.bottom - m_Viewport.top);
	dxViewport.MinDepth = 0.0f;
	dxViewport.MaxDepth = 1.0f;
	dxViewport.TopLeftX = (float)m_Viewport.left;
	dxViewport.TopLeftY = (float)m_Viewport.top;

	CRenderer::GetDeviceContext()->RSSetViewports(1, &dxViewport);


	/*
	// ビューマトリクス設定
	m_InvViewMatrix = XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z);
	m_InvViewMatrix *= XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);

	XMVECTOR det;
	m_ViewMatrix = XMMatrixInverse(&det, m_InvViewMatrix);
	*/
	CRenderer::SetViewMatrix(&viewMatrix);



	// プロジェクションマトリクス設定
	//m_ProjectionMatrix = XMMatrixPerspectiveFovLH(1.0f, dxViewport.Width / dxViewport.Height, 1.0f, 1000.0f);

	CRenderer::SetProjectionMatrix(&projMatrix);



}

