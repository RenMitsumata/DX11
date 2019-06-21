#pragma once
class CModel;
class CShadow;

class CEnemy : public CGameObject
{
private:
	CModel* m_Model = NULL;
	CShadow* m_Shadow = NULL;
public:
	CEnemy();
	virtual ~CEnemy();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
};

