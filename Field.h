#pragma once
#include "main.h"
class CField : public CGameObject
{
private:
	ID3D11Buffer* m_VertexBuffer = NULL;
	ID3D11Buffer* m_IndexBuffer = NULL;
	CTexture* m_Texture = NULL;
	CTexture* m_WallTexture = NULL;
public:
	CField();
	~CField();
	void Init();
	void Uninit();
	void Update();
	void Draw();
};

