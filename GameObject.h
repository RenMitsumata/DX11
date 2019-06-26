#pragma once

#include "main.h"

class CGameObject
{
private:
	bool m_Destroy = false;
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
	void SetDestroy(void) { m_Destroy = true; }
	bool Destroy(void);
	XMFLOAT3 GetPos(void) { return m_Position; }
};

