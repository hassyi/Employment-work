#pragma once
#include "sceneState.h"
#include "gameObject.h"
#include "UI.h"
#include "imguiManager.h"



#define LAYER_MAX (3)

class Game : public SceneState
{
private:
	class Audio* m_BGM{};
	std::list<GameObject*> m_GameObject[LAYER_MAX];
	std::list<UI*> m_Texture;

	ImguiManager* m_ImGui = nullptr;

	bool m_IsDrawColider  = false;
	bool m_IsDrawNodeEditor = false;
	bool m_IsDrawImGui = false;

	bool m_shoudNextScene = false;
	bool m_IsClearResult = false;
	bool m_IsFailureResult = false;

	int m_TransitionCount = 0;

public:
	void Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;

	void SetIsDrawColider(bool isDraw) { m_IsDrawColider = isDraw; }
	bool GetIsDrawColider() { return m_IsDrawColider; }
	bool GetIsDrawImGui() { return m_IsDrawImGui; }

	//listの追加
	template <typename T>	//テンプレート関数
	T* AddGameObject(int Layer) {
		T* gameObject = new T;
		m_GameObject[Layer].push_back(gameObject);
		gameObject->SetScene(this);
		gameObject->Init();

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


	std::list<GameObject*> GetGameObjectList() {
		return m_GameObject[1];
	}

	template <class T>
	T* AddUITexture() {
		T* ui = new T();
		ui->Init();
		m_Texture.push_back(ui);

		return ui;
	}

	template <class T>
	T* GetUITexture(int num) {
		for (UI* ui : m_Texture) {
			T* ret = dynamic_cast<T*>(ui);
			if (ret != nullptr) {
				if (ret->GetTextureNum() == num)
					return ret;
			}
		}
		return nullptr;
	}

	void DrawImGui();

};