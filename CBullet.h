#pragma once



class CModel;
class CBullet : public CGameObject
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
	bool isDestroy(void) { return m_Destroy; }
};

