#pragma once

class CModel;

class CBall : public CGameObject
{
private:
	XMVECTOR m_Quaternion;
	CModel* m_Model = NULL;
public:
	CBall();
	virtual ~CBall();
	void Init();
	void Uninit();
	void Update();
	void Draw();
};

