#pragma once
#include "gameObject.h"
#include "component.h"
#include "animationModel.h"
#include "modelRenderer.h"
#include <string>

class SphereShadow : public GameObjectComponet {
private:
	ModelRenderer* m_Component{};

	XMFLOAT3 m_Pos = { 0.0f,0.0f,0.0f };
	XMFLOAT3 m_Scale = { 1.0f,1.0f,1.0f };
	XMFLOAT3 m_Rot = { 0.0f,0.0f,0.0f };

	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader* m_PixelShader{};
	ID3D11InputLayout* m_VertexLayout{};


public:
	void Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;

	void SetPos(XMFLOAT3 pos) { m_Pos = pos; }
	void SetPosX(float posx) { m_Pos.x = posx; }
	void SetPosY(float posy) { m_Pos.y = posy; }
	void SetPosZ(float posz) { m_Pos.z = posz; }
	void SetScale(XMFLOAT3 scale) { m_Scale = scale; }
	void SetScaleX(float scalex) { m_Scale.x = scalex; }
	void SetScaleY(float scaley) { m_Scale.y = scaley; }
	void SetScaleZ(float scalez) { m_Scale.z = scalez; }
	void SetRot(XMFLOAT3 rot) { m_Rot = rot; }
	void SetRotX(float rotx) { m_Rot.x = rotx; }
	void SetRotY(float roty) { m_Rot.y = roty; }
	void SetRotZ(float rotz) { m_Rot.z = rotz; }

	XMFLOAT3 GetPos() const { return m_Pos; }
	XMFLOAT3 GetScale() const { return m_Scale; }
	XMFLOAT3 GetRot() const { return m_Rot; }
};
