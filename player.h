#pragma once
#include "gameObject.h"
#include "animationModel.h"
#include <string>

enum WEAPON_TYPE
{
	SWORD,
	GUN
};

enum PLAYER_STATE
{
	IDLE,
	RUN,
	ATTACK1,
	ATTACK2,
	JUMP,
	STEP,

};

class Player : public GameObject {
private:
	AnimationModel* m_Component{};

	class Predation* m_Predation{};
	class BuffParticle* m_BuffParticle{};

	float m_Rot = 0.0f;
	XMFLOAT3 m_dir{};
	bool m_JampFlag = false;
	float m_move = 0.0f;
	float m_BulletPoint = 0.0f;
	const float BULLET_POINT_MAX = 100.0f;

	bool m_IsBuff = false;
	

	//攻撃のフレーム
	int m_AttackFrame = 0;
	bool m_isAttack = false;
	//次の攻撃の受付時間のフレーム
	int m_AttackReseveFrame = 0;
	bool m_isNext = false;
	bool m_isNextOnAttack = false;

	//次の攻撃のフレーム
	int m_SecondAttackFrame = 0;
	bool m_isSecondAttack = false;

	bool m_isAttackCancel = false;

	//捕食攻撃のフレーム
	int m_PredationFrame = 0;
	bool m_isPredation = false;

	//ステップ
	int m_StepFrame = 0;
	float m_StepSpeed = 3.0f;
	bool m_isStep = false;

	//武器切り替え
	bool m_Sword = true;

	int m_AnimationFrame = 0;
	float m_AnimetionBlendRatio = 0.0f;

	int m_DyingFrame = 0;		//死ぬときのアニメーションのフレーム
	bool m_IsDethAnim = false;
	int m_DieFrame = 0;			//死んでからのフレーム
	bool m_IsDie = false;

	bool m_shoudNextScene = false;

	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;
	ID3D11InputLayout* m_VertexLayout;

	std::string		m_AnimationName1{};
	std::string		m_AnimationName2{};
	std::string		m_AnimationName{};


	class Audio* m_SE{};

	WEAPON_TYPE m_Weapon = SWORD;
	PLAYER_STATE m_State = IDLE;

public:
	void Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;

	void PlayerControl();
	void PlayerCollision();
	void PredationAttack();
	void PlayerBuff();
	void PlayerAttack();
	void DeathAnim();

	void ResetFlag()
	{
		m_JampFlag = false;
		m_IsBuff = false;
		m_isAttack = false;
		m_isNext = false;
		m_isNextOnAttack = false;
		m_isSecondAttack = false;
		m_isAttackCancel = false;
		m_isPredation = false;
		m_isStep = false;
	}


	void SetIsBuff(bool isbuff) { m_IsBuff = isbuff; }
	void SetBulletPoint(float point) { m_BulletPoint = point; }

	bool GetIsBuff() { return m_IsBuff; }
	XMFLOAT3 GetDir() const { return m_dir; }
	float GetBulletPoint() { return m_BulletPoint; }
	WEAPON_TYPE GetWeaponType() { return m_Weapon; }
	bool GetIsDie() { return m_IsDie; }
	bool GetPlayerDie() { return m_shoudNextScene; }

	bool GetIsAttack() { return m_isAttack; }
	bool GetIsSecondAttack() { return m_isSecondAttack; }

	void DrawImGui()override;
};
