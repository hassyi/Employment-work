#pragma once
#include "sceneState.h"
#include "fade.h"

class Scene
{
private:
	Scene() = default;
	static Scene* m_Scene;
	explicit Scene(SceneState* state) : m_State(state){}
	virtual ~Scene() { delete m_State; }
	SceneState* m_State;


public:
	void Init() { m_State->Init(); }
	void Uninit() { m_State->Uninit(); }
	void Update() { m_State->Update(); }
	void Draw() { m_State->Draw(); }

	void InitScene(SceneState* state)
	{
		m_State = state;
		Init();
	}
	void ChangeScene(SceneState* state)
	{
		m_State->Uninit();
		delete m_State;
		m_State = state;
		Init();
	}

	static Scene* GetInstance()
	{
		if (m_Scene == nullptr)
		{
			m_Scene = new Scene;
		}
		return m_Scene;
	}

	template <class T>
	T* GetScene()
	{
		T* scene = static_cast<T*> (m_State);
		if (scene != nullptr) {
			return scene;
		}
		return nullptr;
	}

	SceneState* GetNowScene() { return m_State; }
};