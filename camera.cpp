
#include "main.h"
#include "renderer.h"
#include "GameObject.h"
#include "camera.h"
#include "input.h"




void CCamera::Init()
{

	m_Position = XMFLOAT3( 0.0f, 5.0f, -10.0f );
	m_Rotation = XMFLOAT3( 0.5f, 0.0f, 0.0f );

	worldMatrix = XMMatrixTranslation(0.0f, 0.0f, 0.0f);

	eye = XMVectorSet(2.0f, 2.0f, -2.0f, 0.0f);
	focus = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	

	fov = XMConvertToRadians(45.0f);
	aspect = SCREEN_WIDTH / SCREEN_HEIGHT;
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
		m_Position.z += 0.5f;
	}
	if (CInput::GetKeyPress(VK_DOWN)) {
		m_Position.z -= 0.5f;
	}
	if (CInput::GetKeyPress(VK_LEFT)) {
		m_Position.x -= 0.5f;
	}
	if (CInput::GetKeyPress(VK_RIGHT)) {
		m_Position.x += 0.5f;
	}
	XMMATRIX viewMatrix = XMMatrixLookAtLH(eye, focus, up);
	XMMATRIX projMatrix = XMMatrixPerspectiveFovLH(fov, aspect, nearZ, farZ);

}



void CCamera::Draw()
{

	XMMATRIX	m_ViewMatrix;
	XMMATRIX	m_InvViewMatrix;
	XMMATRIX	m_ProjectionMatrix;



	// �r���[�|�[�g�ݒ�
	D3D11_VIEWPORT dxViewport;
	dxViewport.Width = (float)(m_Viewport.right - m_Viewport.left);
	dxViewport.Height = (float)(m_Viewport.bottom - m_Viewport.top);
	dxViewport.MinDepth = 0.0f;
	dxViewport.MaxDepth = 1.0f;
	dxViewport.TopLeftX = (float)m_Viewport.left;
	dxViewport.TopLeftY = (float)m_Viewport.top;

	CRenderer::GetDeviceContext()->RSSetViewports(1, &dxViewport);



	// �r���[�}�g���N�X�ݒ�
	m_InvViewMatrix = XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z);
	m_InvViewMatrix *= XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);

	XMVECTOR det;
	m_ViewMatrix = XMMatrixInverse(&det, m_InvViewMatrix);

	CRenderer::SetViewMatrix(&m_ViewMatrix);



	// �v���W�F�N�V�����}�g���N�X�ݒ�
	m_ProjectionMatrix = XMMatrixPerspectiveFovLH(1.0f, dxViewport.Width / dxViewport.Height, 1.0f, 1000.0f);

	CRenderer::SetProjectionMatrix(&m_ProjectionMatrix);



}

