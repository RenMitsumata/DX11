#pragma once

#include "CShadow.h"
#include "model.h"

#include "GameObject.h"


class CPlayer :	public CGameObject
{
private:
	CModel* m_Model = NULL;
	CShadow* m_Shadow = NULL;
public:
	CPlayer();
	virtual ~CPlayer();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
};

