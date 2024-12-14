#pragma once
#include "gameObject.h"
#include "component.h"
#include "animationModel.h"
#include "modelRenderer.h"
#include <string>

class PlayerShadow : public GameObject {
private:
	//AnimationModel* m_Component{};

	ModelRenderer* m_Component{};

	float m_Rot = 0.0f;
	XMFLOAT3 m_dir{};

	int m_AnimationFrame = 0;
	int m_IdleFrame = 0;
	int m_RunFrame = 0;
	float m_AnimetionBlendRatio = 0.0f;

	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader* m_PixelShader{};
	ID3D11InputLayout* m_VertexLayout{};

	std::string		m_AnimationName1{};
	std::string		m_AnimationName2{};
	std::string		m_AnimationName{};


public:
	void Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;

};
