#pragma once
#include <list>
#include <vector>
#include "gameObject.h"
#include "polygon2D.h"
#include "camera.h"
#include "field.h"
#include "player.h"
#include "enemy.h"
#include "bullet.h"
#include "sky.h"
#include "explosion.h"
#include "cylinder.h"
#include "box.h"

#define LAYER_MAX (3)

class Scene
{
protected:
	std::list<GameObject*> m_GameObject[LAYER_MAX];


public:
	virtual ~Scene() {}
	virtual void Init() {}

	virtual void Uninit() {
		for (int i = 0; i < LAYER_MAX; i++) {
			for (GameObject* object : m_GameObject[i]) {
				object->Uninit();
				delete object;
			}
			m_GameObject[i].clear();
		}
	}
	virtual void Update() {
		for (int i = 0; i < LAYER_MAX; i++) {
			for (GameObject* object : m_GameObject[i]) {
				object->Update();
			}
			m_GameObject[i].remove_if([](GameObject* object) {return object->Destroy(); });	//ラムダ式
		}
	}
	virtual void Draw() {

		for (int i = 0; i < LAYER_MAX; i++) {
			//m_GameObject[i].sort();
			for (GameObject* object : m_GameObject[i]) {
				object->Draw();
			}
		}
	}

	//listの追加
	template <typename T>	//テンプレート関数
	T* AddGameObject(int Layer) {
		T* gameObject = new T();
		gameObject->Init();
		m_GameObject[Layer].push_back(gameObject);

		return gameObject;
	}

	//指定したGameObjectの取得
	template <typename T>
	T* GetGameObject() {
		for (int i = 0; i < 3; i++) {
			for (GameObject* object : m_GameObject[i]) {
				T* ret = dynamic_cast<T*>(object);
				if (ret != nullptr) {
					return ret;
				}
			}
		}
		return nullptr;
	}

	//指定したGameObjectの取得
	template <typename T>
	std::vector<T*> GetGameObjects() {
		std::vector<T*> objectList;

		for (int i = 0; i < 3; i++) {
			for (GameObject* object : m_GameObject[i]) {
				T* ret = dynamic_cast<T*>(object);
				if (ret != nullptr) {
					objectList.push_back(ret);
				}
			}
		}
		return objectList;
	}

	template <typename T>
	std::list<T*> GetGameObjectList() {
		std::list<T*> objectList;

		for (int i = 0; i < 3; i++) {
			for (GameObject* object : m_GameObject[i]) {
				T* ret = dynamic_cast<T*>(object);
				if (ret != nullptr) {
					objectList.push_back(ret);
				}
			}
		}
		return objectList;
	}


	std::list<GameObject*> GetGameObjectList(int object) {
		return m_GameObject[object];
	}
};