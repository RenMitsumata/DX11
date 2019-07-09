#pragma once



class CModel;
class CShadow;
class CBullet;

class CPlayer :	public CGameObject
{
private:
	CModel* m_Model = NULL;
	CShadow* m_Shadow = NULL;
	XMVECTOR front;
	XMVECTOR up;
	std::list<CBullet*> _Bulletlist;
public:
	CPlayer();
	virtual ~CPlayer();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
};

