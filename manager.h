#pragma once

class CScene;

class CManager
{


public:
	static void Init();
	static void Uninit();
	static void Update();
	static void Draw();
	static CScene* GetScene();

};