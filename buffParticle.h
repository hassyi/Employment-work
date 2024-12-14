#pragma once
#include "gameObject.h"

class BuffParticle : public GameObject
{
private:
	int m_Count{};

	struct PARTICLE 
	{
		bool		Enable;
		int			Life;
		XMFLOAT3	Pos;
		XMFLOAT3	Vel;
	};

	static const int PARTICLE_MAX = 1000;
	PARTICLE m_Particle[PARTICLE_MAX]{};			//ここに{}を入れるとstructの中身が0とかfalseに初期化される

	bool m_isPlayerBuff = false;

public:
	void Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;

	void SetPlayerBuff(bool isbuff) { m_isPlayerBuff = isbuff; }
};