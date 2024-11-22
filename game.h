#pragma once
#include "sceneState.h"
#include "gameObject.h"
#include "UI.h"



#define LAYER_MAX (3)

class Game : public SceneState
{
private:
	class Audio* m_BGM{};
	std::list<GameObject*> m_GameObject[LAYER_MAX];
	std::list<UI*> m_Texture;


public:
	void Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;


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

	template <class T>
	T* AddUITexture() {
		T* ui = new T();
		ui->Init();
		m_Texture.push_back(ui);

		return ui;
	}

	template <class T>
	T* GetUITexture() {
		for (UI* ui : m_Texture) {
			T* ret = dynamic_cast<T*>(UI);
			if (ret != nullptr) {
				return ret;
			}
		}
		return nullptr;
	}


};