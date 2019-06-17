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
	for���̐V����������(C++ 11�ȍ~)
	(CGameObject* object : _Objectlist)
	��':'�̉E���́A�R���e�i�ł��邱��(list,vector�Ȃ�)

	������
	�E���̃R���e�i�̐擪����A�����̓���q�ɒl�����Ă����
	���I�[�iNULL�j�ɂȂ����烋�[�v�E�o

	�e���v���[�g�́A���܂藐�p���Ȃ����ƁI
	cpp�ɏ����ƃG���[���ł�

*/

