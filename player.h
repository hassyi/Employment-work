#pragma once
#include "gameObject.h"
#include "component.h"
#include "animationModel.h"
#include <string>

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
	float m_GroundHeight = 0.0f;
	bool m_JampFlag = false;
	float m_move = 0.0f;
	

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

	float m_Gravity = 0.0f;
	bool m_IsGravity = true;


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

	XMFLOAT3 GetDir() const { return m_dir; }
};
