
#include "main.h"
#include "renderer.h"
#include "GameObject.h"
#include "CPlayer.h"
#include "polygon.h"
#include "camera.h"
#include "Field.h"
#include "Scene.h"
#include "input.h"
#include "manager.h"



static CInput* g_Input;
static CScene* g_Scene;
static CPlayer* g_Player;

void CManager::Init()
{

	CRenderer::Init();
	g_Scene = new CScene;
	g_Scene->Init();
	g_Input = new CInput;
	g_Input->Init();
	g_Player = new CPlayer;
	g_Player->Init();
}

void CManager::Uninit()
{
	g_Player->Uninit();
	delete g_Player;
	g_Input->Uninit();
	delete g_Input;
	g_Scene->Uninit();
	delete g_Scene;

	CRenderer::Uninit();
	
}

void CManager::Update()
{
	g_Scene->Update();
	g_Input->Update();
}

void CManager::Draw()
{

	CRenderer::Begin();
	g_Scene->Draw();

	CRenderer::End();

}
