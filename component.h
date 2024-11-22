#pragma once
#include "main.h"
#include <string>
#include <iostream>
#include <list>

class Component
{
private:

	class GameObject* m_GameObject = nullptr;
protected:
	int m_ComponentNum = 0;

public:
	Component() = default;		//deleteを入れることで引数なしのコンストラクタを消せる
	//Component(GameObject* object) { m_GameObject = object; }
	virtual ~Component(){}

	virtual void Init() {};		//仮想関数
	virtual void Uninit() {};
	virtual void Update() {};
	virtual void Draw() {};

	void SetComponentNum(int num) { m_ComponentNum = num; }
	int GetComponentNum() { return m_ComponentNum; }

};