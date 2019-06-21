#pragma once



class CModel;
class CShadow;
class CBullet;

class CPlayer :	public CGameObject
{
private:
	CModel* m_Model = NULL;
	CShadow* m_Shadow = NULL;
	CBullet* m_Bullet = NULL;
public:
	CPlayer();
	virtual ~CPlayer();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
};

