#pragma once
#include "Scene.h"
class CTitle :
	public CScene
{
private:
	CPolygon* title = NULL;
public:
	CTitle();
	virtual ~CTitle();
	void Init(void);
	void Update(void);
};

