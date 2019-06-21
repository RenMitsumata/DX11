#pragma once

#include <list>
#include "GameObject.h"
#include "CPlayer.h"

#include "camera.h"
#include "Field.h"
#include "polygon.h"
#include "CBullet.h"

class CScene
{
protected:
	std::list<CGameObject*> _Objectlist[5];
public:
	template<typename T>
	T* AddGameObject(int layer) {
		T* object = new T();
		object->Init();
		_Objectlist[layer].push_back(object);
		return object;
	}

	CScene() {}
	virtual ~CScene() {}

	virtual void Init(void) {
		
		AddGameObject<CCamera>(0);
		AddGameObject<CField>(1);
		AddGameObject<CPlayer>(2);
		AddGameObject<CPolygon>(4);
		
	}
	virtual void Uninit(void) {
		
		for (int i = 0; i < 5; i++) {
			for (CGameObject* object : _Objectlist[i]) {
				object->Uninit();
				delete object;
			}
			_Objectlist[i].clear();

		}
		
	}
	virtual void Update(void) {
		for (int i = 0; i < 5; i++) {
			for (CGameObject* object : _Objectlist[i]) {
				object->Update();
			}
		}
	}
	virtual void Draw(void) {
		for (int i = 0; i < 5; i++) {
			for (CGameObject* object : _Objectlist[i]) {
				object->Draw();
			}
		}
	}

};

/*
	for文の新しい書き方(C++ 11以降)
	(CGameObject* object : _Objectlist)
	●':'の右側は、コンテナであること(list,vectorなど)

	動き方
	右側のコンテナの先頭から、左側の入れ子に値を入れてくれる
	→終端（NULL）になったらループ脱出

	テンプレートは、あまり乱用しないこと！
	cppに書くとエラーがでる

*/

/*
	レイヤー
	カメラ：０
	背景：１
	３D：２
	エフェクト：３
	２D：４
*/
