#pragma once
#include "gameObject.h"

struct VERTEX_3D_PARTICLE
{
	XMFLOAT3 Position;
	XMFLOAT3 TexCoord;
};

class SandParticle : public GameObject
{
private:
	ID3D11ComputeShader* m_ComputeShader;

	ID3D11Buffer* m_VertexBuffer{};
	ID3D11ShaderResourceView* m_Texture{};

	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader* m_PixelShader{};
	ID3D11InputLayout* m_VertexLayout{};

	ID3D11Buffer* m_ParticleBuffer{};
	ID3D11Buffer* m_ResultBuffer{};
	ID3D11Buffer* m_PositionBuffer{};

	ID3D11ShaderResourceView* m_ParticleSRV;
	ID3D11ShaderResourceView* m_PositionSRV;

	ID3D11UnorderedAccessView* m_ResultUAV;

	int m_Count{};

	struct PARTICLE 
	{
		bool		Enable;
		int			Life;
		XMFLOAT3	Pos;
		XMFLOAT3	Vel;
		XMFLOAT3	Accel;
		XMFLOAT3	Resultant;
		XMFLOAT3	Gravity;
	};

	struct WINDFORCE
	{
		int			Status;
		XMFLOAT3	Force;
	};

	static const int PARTICLE_MAX = 1000;
	PARTICLE m_Particle[PARTICLE_MAX]{};			//ここに{}を入れるとstructの中身が0とかfalseに初期化される

	float m_Size = 1.0f;

public:
	void Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;

};