#pragma once



class CModel;
class CBullet :	public CGameObject
{
private:
	CModel* m_Model;
public:
	CBullet();
	virtual ~CBullet();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
};

