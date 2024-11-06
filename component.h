#pragma once

class Component
{
private:

	class GameObject* m_GameObject = nullptr;

public:
	Component() = delete;		//deleteを入れることで引数なしのコンストラクタを消せる
	Component(GameObject* object) { m_GameObject = object; }
	virtual ~Component(){}

	virtual void Init() {};		//仮想関数
	virtual void Uninit() {};
	virtual void Update() {};
	virtual void Draw() {};

};