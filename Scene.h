#pragma once

#include <list>


class CScene
{
protected:
	std::list<CGameObject*> _Objectlist;

	template<typename T>
	T* AddGameObject() {
		T* object = new T();
		object->Init();
		_Objectlist.push_back(object);
		return object;
	}
public:
	CScene() {}
	virtual ~CScene() {}

	virtual void Init(void) {
		
		AddGameObject<CCamera>();
		AddGameObject<CField>();
		AddGameObject<CModel>();
		AddGameObject<CPolygon>();		
	}
	virtual void Uninit(void) {
		for (CGameObject* object:_Objectlist) {
			object->Uninit();
			delete object;
		}
		_Objectlist.clear();
	}
	virtual void Update(void) {
		for (CGameObject* object : _Objectlist) {
			object->Update();
		}
	}
	virtual void Draw(void) {
		for (CGameObject* object : _Objectlist) {
			object->Draw();
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

