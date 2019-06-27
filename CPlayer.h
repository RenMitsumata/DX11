#pragma once



class CModel;
class CShadow;
class CBullet;

class CPlayer :	public CGameObject
{
private:
	CModel* m_Model = NULL;
	CShadow* m_Shadow = NULL;
	static CBullet* m_Bullet[10];
	static int bulletCnt;
public:
	CPlayer();
	virtual ~CPlayer();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static void DeleteBullet(int mgrNum);
};

