#pragma once


class CCamera : public CGameObject
{
private:
	
	XMFLOAT3					m_Position;
	XMFLOAT3					m_Rotation;
	
	RECT m_Viewport;

	XMMATRIX worldMatrix;

	XMVECTOR eye;
	XMVECTOR focus;
	XMVECTOR up;


	float    fov;
	float    aspect;
	float    nearZ;
	float    farZ;

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();


};