#include "main.h"
#include "renderer.h"
#include "input.h"
#include "Scene.h"
#include "manager.h"



static CInput* g_Input;
static CScene* g_Scene;

void CManager::Init()
{

	CRenderer::Init();
	g_Scene = new CScene;
	g_Scene->Init();
	g_Input = new CInput;
	g_Input->Init();
}

void CManager::Uninit()
{
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

CScene * CManager::GetScene()
{
	return g_Scene;
}
