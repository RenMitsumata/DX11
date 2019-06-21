#pragma once

#include "main.h"

class CGameObject
{
protected:
	XMFLOAT3 m_Position;
	XMFLOAT3 m_Rotation;
public:
	CGameObject() {};
	virtual ~CGameObject() {};
	virtual void Init(void) = 0;
	virtual void Uninit(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;
};

