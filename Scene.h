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
	for���̐V����������(C++ 11�ȍ~)
	(CGameObject* object : _Objectlist)
	��':'�̉E���́A�R���e�i�ł��邱��(list,vector�Ȃ�)

	������
	�E���̃R���e�i�̐擪����A�����̓���q�ɒl�����Ă����
	���I�[�iNULL�j�ɂȂ����烋�[�v�E�o

	�e���v���[�g�́A���܂藐�p���Ȃ����ƁI
	cpp�ɏ����ƃG���[���ł�

*/

/*
	���C���[
	�J�����F�O
	�w�i�F�P
	�RD�F�Q
	�G�t�F�N�g�F�R
	�QD�F�S
*/
