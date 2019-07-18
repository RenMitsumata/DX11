
#include "manager.h"
#include "Scene.h"
#include "input.h"
#include "CGame.h"
#include "CTitle.h"



CTitle::CTitle()
{
}


CTitle::~CTitle()
{
}

void CTitle::Init(void)
{
	title = AddGameObject<CPolygon>(4);
	title->Set("asset/titleImage.png");
}

void CTitle::Update(void)
{
	if (CInput::GetKeyTrigger(VK_RETURN)) {
		CManager::SetScene<CGame>();
	}
}

