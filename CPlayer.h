#pragma once



class CModel;
class CShadow;
class CCamera;
class CBullet;
class Cource;
class SkyDome;
class CPolygon;

class CPlayer :	public CGameObject
{
private:
	CModel* m_Model = NULL;
	CShadow* m_Shadow = NULL;
	CCamera* m_Camera = NULL;
	Cource* pCource = NULL;
	SkyDome* skydome = NULL;
	CPolygon* playerUI = NULL;
	XMVECTOR front;
	XMVECTOR up;
	float distance;
	std::list<CBullet*> _Bulletlist;
public:
	CPlayer();
	virtual ~CPlayer();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
};

