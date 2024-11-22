#pragma once
#include "sceneState.h"

class Scene
{
private:
	explicit Scene(SceneState* state) : m_State(state){}
	Scene() = default;
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
		delete m_State;
		m_State = state;
		Init();
	}

	static Scene* GetInstance()
	{
		static Scene* scene = new Scene;
		return scene;
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


};