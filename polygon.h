#pragma once
#include "main.h"

class CPolygon : public CGameObject{
private:
	ID3D11Buffer* m_VertexBuffer = NULL;
	CTexture* m_Texture = NULL;

public:
	CPolygon();
	virtual ~CPolygon();
	void Init();
	void Uninit();
	void Update();
	void Draw();
};

