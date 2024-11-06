#pragma once

class Component
{
private:

	class GameObject* m_GameObject = nullptr;

public:
	Component() = delete;		//delete�����邱�Ƃň����Ȃ��̃R���X�g���N�^��������
	Component(GameObject* object) { m_GameObject = object; }
	virtual ~Component(){}

	virtual void Init() {};		//���z�֐�
	virtual void Uninit() {};
	virtual void Update() {};
	virtual void Draw() {};

};