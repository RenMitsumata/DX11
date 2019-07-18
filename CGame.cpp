#include "manager.h"
#include "Scene.h"
#include "input.h"
#include "CResult.h"
#include "audio_clip.h"
#include "CGame.h"


CGame::CGame()
{
}


CGame::~CGame()
{
}

void CGame::Init(void)
{

	AddGameObject<Cource>(1);
	AddGameObject<CField>(1);
	AddGameObject<CPlayer>(2);
	AddGameObject<CEnemy>(2);
	AddGameObject<CEnemy>(2);
	AddGameObject<CEnemy>(2);
	//AddGameObject<CPolygon>(4);
	
	m_BGM = new CAudioClip;
	m_BGM->Load("sound/Swan Lake.wav");
	//m_BGM->Play(true);
	
}

void CGame::Uninit(void)
{
	CScene::Uninit();
	m_BGM->Stop();
	m_BGM->Unload();
	delete m_BGM;
}

void CGame::Update(void)
{
	CScene::Update();
	if (CInput::GetKeyTrigger(VK_LSHIFT)) {
		CManager::SetScene<CResult>();
	}
}

