#pragma once

#include "gameObject.h"
#include "component.h"
#include "transform3DComponent.h"


class Predation : public GameObject
{
private:
	Transform3DComponent* m_Kiba_ue = nullptr;
	Transform3DComponent* m_Kiba_sita = nullptr;


	Component* m_Component{};

	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader* m_PixelShader{};
	ID3D11InputLayout* m_VertexLayout{};

	float m_Kiba_ue_RotTarget = -XM_PI / 4;
	float m_Kiba_sita_RotTarget = XM_PI / 4;

	int m_Frame = 0;
	bool m_Buff = false;
	bool m_Use = false;
	bool m_IsHit = false;

	float m_Time = 0.0f;

public:
	void Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;
	void PredationCollision();
	void SetBuff(bool buff) { m_Buff = buff; }
	bool GetBuff() const{ return m_Buff; }
	void SetUse(bool use) { m_Use = use; }

	void SetKibaPos(XMFLOAT3 pos)
	{
		m_Kiba_ue->SetPos(pos);
		m_Kiba_sita->SetPos(pos);
	}
	void SetKibaRot(XMFLOAT3 rot)
	{
		m_Kiba_ue->SetRot(XMFLOAT3(rot.x, rot.y - XM_PI / 2, rot.z));
		m_Kiba_sita->SetRot(XMFLOAT3(rot.x, rot.y - XM_PI / 2, rot.z));
	}

	void RotationAnim(float t);
};
