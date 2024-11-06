#pragma once

#include "gameObject.h"
#include "component.h"

class Predation : public GameObject
{
private:
	Component* m_Component{};

	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;
	ID3D11InputLayout* m_VertexLayout;

	int m_frame = 0;
	bool m_buff = false;
	bool m_use = false;

public:
	void Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;
	void PredationCollision();
	void SetBuff(bool buff) { m_buff = buff; }
	bool GetBuff() const{ return m_buff; }
	void SetUse(bool use) { m_use = use; }
};
