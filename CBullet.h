#pragma once



class CModel;
class CBullet
{
private:
	CModel* m_Model;
	CColSphere* m_Collision;
	XMFLOAT3 m_Position;
	CGameObject* owner;
public:
	CBullet();
	CBullet(CGameObject* owner);
	virtual ~CBullet();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
};

