#pragma once
#include "gameObject.h"

class PartrcleEmitter : public GameObject
{
private:
	ID3D11Buffer* m_VertexBuffer{};
	ID3D11ShaderResourceView* m_Texture{};

	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader* m_PixelShader{};
	ID3D11InputLayout* m_VertexLayout{};

	int m_Count{};

	struct PARTICLE 
	{
		bool		Enable;
		int			Life;
		XMFLOAT3	Pos;
		XMFLOAT3	Vel;
	};

	static const int PARTICLE_MAX = 1000;
	PARTICLE m_Particle[PARTICLE_MAX]{};			//‚±‚±‚É{}‚ğ“ü‚ê‚é‚Æstruct‚Ì’†g‚ª0‚Æ‚©false‚É‰Šú‰»‚³‚ê‚é

public:
	void Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;

};