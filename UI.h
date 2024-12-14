#pragma once
#include "main.h"
#include "renderer.h"
#include <list>
#include "uiComponent.h"
#include "sceneState.h"

class UIComponent;

class UI
{
protected:
	std::list<UIComponent* > m_ComponentList;
	SceneState* m_Scene;
	
	bool m_ShouldDelete = false;

	int m_TextureNum = 0;

public:
	UI() {}
	~UI(){}

	virtual void Init() = 0;
	virtual void Uninit() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

	void SetScene(SceneState* state) { m_Scene = state; }
	SceneState* GetScene() { return m_Scene; }

	void SetDelete(bool _delete) { m_ShouldDelete = _delete; }
	bool Delete() 
	{
		if (m_ShouldDelete) 
		{
			Uninit();
			delete this;
			return true;
		}
		else {
			return false;
		}
	}

	template <class T>
	T* AddComponent()
	{
		T* component = new T;
		component->AcceptUITexture(this);
		m_ComponentList.push_back(component);
		
		return component;
	}

	template <class T>
	T* GetComponent()
	{
		for (auto component : m_ComponentList)
		{
			T* ret = dynamic_cast<T*>(component);
			if (ret != nullptr) {
				return ret;
			}
		}
		return nullptr;
	}

	void SetTextureNum(int num) { m_TextureNum = num; }
	int GetTextureNum() { return m_TextureNum; }
};