#pragma once
#include "gameObject.h"
#include "component.h"
#include "animationModel.h"
#include <string>

enum WEAPON_TYPE
{
	SWORD,
	GUN
};

class Predation;
class BuffParticle;

class Player : public GameObject {
private:
	AnimationModel* m_Component{};
	Component* m_ChildModel{};

	Predation* m_Predation{};
	BuffParticle* m_BuffParticle{};

	float m_Rot = 0.0f;
	XMFLOAT3 m_dir{};
	bool m_JampFlag = false;
	float m_move = 0.0f;
	float m_BulletPoint = 0.0f;
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

	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;
	ID3D11InputLayout* m_VertexLayout;

	std::string		m_AnimationName1{};
	std::string		m_AnimationName2{};
	std::string		m_AnimationName{};


	class Audio* m_SE{};

	WEAPON_TYPE m_Weapon = SWORD;


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

	void SetIsBuff(bool isbuff) { m_IsBuff = isbuff; }

	bool GetIsBuff() { return m_IsBuff; }
	XMFLOAT3 GetDir() const { return m_dir; }
	float GetBulletPoint() { return m_BulletPoint; }
	WEAPON_TYPE GetWeaponType() { return m_Weapon; }

	void DrawImGui()override;
};
