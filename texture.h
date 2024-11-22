#pragma once
#include "uiComponent.h"

class Texture : public UIComponent
{
protected:
	VERTEX_3D m_Vertex[4] = {};

	XMFLOAT3 m_Pos = { 0.0f,0.0f,0.0f };
	XMFLOAT3 m_Scale = { 1.0f,1.0f,1.0f };
	XMFLOAT3 m_Rot = { 0.0f,0.0f,0.0f };

	ID3D11Buffer* m_VertexBuffer{};
	ID3D11ShaderResourceView* m_Texture{};

	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader* m_PixelShader{};
	ID3D11InputLayout* m_VertexLayout{};
	
	XMFLOAT2 m_VertexPos = {};
	XMFLOAT2 m_VertexSize = {};

	float m_AnimX = 0.0f, m_AnimY = 0.0f;
	float m_AnimSizeX = 0.0f, m_AnimSizeY = 0.0f;
	float m_Disit = 0.0f;

	const wchar_t* m_TextureData = L"none";

	int m_Count = 0;
	bool m_IsDisitDisplay = false;

public:
	Texture() = default;
	Texture(UI* ui, const wchar_t* data) : UIComponent(ui), m_TextureData(data){}

	virtual void Init()override {};
	virtual void Uninit()override {};
	virtual void Update()override {};
	virtual void Draw()override {};

	void SetPos(XMFLOAT3 pos) { m_Pos = pos; }
	void SetScale(XMFLOAT3 scale) { m_Scale = scale; }
	void SetRot(XMFLOAT3 rot) { m_Rot = rot; }

	XMFLOAT3 GetPos() { return m_Pos; }
	XMFLOAT3 GetScale() { return m_Scale; }
	XMFLOAT3 GetRot() { return m_Rot; }

	void SetVertex(VERTEX_3D vertex[4])
	{
		m_Vertex[0] = vertex[0];
		m_Vertex[1] = vertex[1];
		m_Vertex[2] = vertex[2];
		m_Vertex[3] = vertex[3];
	}

	void SetTransTexNum(float posx, float posy, float sizex, float sizey, const wchar_t* data, int num)
	{
		m_VertexPos.x = posx;
		m_VertexPos.y = posy;
		m_VertexSize.x = sizex;
		m_VertexSize.y = sizey;
		m_TextureData = data;
		m_ComponentNum = num;
	}

	void SetPosSize(float posx, float posy, float sizex, float sizey)
	{
		m_VertexPos.x = posx;
		m_VertexPos.y = posy;
		m_VertexSize.x = sizex;
		m_VertexSize.y = sizey;
	}

	void SetAnimation(float animx, float animy, float sizex, float sizey)
	{
		m_AnimX = animx;
		m_AnimY = animy;
		m_AnimSizeX = sizex;
		m_AnimSizeY = sizey;
	}

	void SetDisitDisplay(bool isDisplay) { m_IsDisitDisplay = isDisplay; }
	void SetDisit(float disit) { m_Disit = disit; }

};