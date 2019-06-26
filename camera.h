#pragma once

#define FOCUS_LENGTH (5)

class CCamera : public CGameObject
{
private:
	
	
	RECT m_Viewport;

	XMMATRIX worldMatrix;

	XMVECTOR eye;
	XMVECTOR at;
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