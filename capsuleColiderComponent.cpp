#include "capsuleColiderComponent.h"
#include "gameObject.h"
#include "transform3DComponent.h"
#include "game.h"
#include "scene.h"
#include <algorithm>
#include "boxColiderComponent.h"

void CapsuleColiderComponent::Init()
{
	m_Model = new ModelRenderer;
	m_Model->Load("asset\\model\\capsule.obj");

	m_Pos = GetGameObject()->GetComponent<Transform>()->GetPos();
	m_Scale = GetGameObject()->GetComponent<Transform>()->GetScale();
	m_Rot = GetGameObject()->GetComponent<Transform>()->GetRot();

	m_StartPos = XMFLOAT3(m_Pos.x, m_Pos.y + m_CenterLength, m_Pos.z);
	m_EndPos = XMFLOAT3(m_Pos.x, m_Pos.y - m_CenterLength, m_Pos.z);

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
		"shader\\wireFrameVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader,
		"shader\\wireFramePS.cso");

	m_ColiderType = CAPSULE_COLIDER;
}

void CapsuleColiderComponent::Uninit()
{
	m_Model->Uninit();
	delete m_Model;
}

void CapsuleColiderComponent::Update()
{
	//m_Pos = GetGameObject()->GetComponent<Transform>()->GetPos();
	m_Rot = GetGameObject()->GetComponent<Transform>()->GetRot();

	MoveCollision();
}

void CapsuleColiderComponent::Draw()
{
	if (!Scene::GetInstance()->GetScene<Game>()->GetIsDrawColider()) return;

	//入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	//シェーダー設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	//ワールドマトリクス設定
	XMMATRIX world, scale, rot, trans;
	scale = XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);
	rot = XMMatrixRotationRollPitchYaw(m_Rot.x, m_Rot.y, m_Rot.z);
	trans = XMMatrixTranslation(m_Pos.x, m_Pos.y, m_Pos.z);	
	world = scale * rot * trans;
	Renderer::SetWorldMatrix(world);

	D3D11_RASTERIZER_DESC rasterDesc{};
	rasterDesc.FillMode = D3D11_FILL_WIREFRAME;
	rasterDesc.CullMode = D3D11_CULL_NONE;
	rasterDesc.DepthBias = 1;
	rasterDesc.SlopeScaledDepthBias = 1.0f;

	ID3D11RasterizerState* wireframeState;
	Renderer::GetDevice()->CreateRasterizerState(&rasterDesc, &wireframeState);

	Renderer::GetDeviceContext()->RSSetState(wireframeState);

	m_Model->Draw();

	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.CullMode = D3D11_CULL_BACK;

	Renderer::GetDevice()->CreateRasterizerState(&rasterDesc, &wireframeState);
	Renderer::GetDeviceContext()->RSSetState(wireframeState);
}


void CapsuleColiderComponent::MoveCollision()
{
	m_Pos = Add(m_Pos, m_AddPos);
	{
		Angle angle;

		XMFLOAT3 vec;
		XMFLOAT3 topVec;
		XMFLOAT3 bottomVec;

		vec = XMFLOAT3(0.0f, 1.0f, 0.0f);
		angle = GetAddAngle(m_Rot, vec);

		topVec.x = angle.AngleX.x + angle.AngleX.y + angle.AngleX.z;
		topVec.y = angle.AngleY.x + angle.AngleY.y + angle.AngleY.z;
		topVec.z = angle.AngleZ.x + angle.AngleZ.y + angle.AngleZ.z;

		topVec = MulFloat(topVec, m_CenterLength);

		m_StartPos.x = topVec.x + m_Pos.x;
		m_StartPos.y = topVec.y + m_Pos.y;
		m_StartPos.z = topVec.z + m_Pos.z;

		vec = XMFLOAT3(0.0f, -1.0f, 0.0f);
		angle = GetAddAngle(m_Rot, vec);

		bottomVec.x = angle.AngleX.x + angle.AngleX.y + angle.AngleX.z;
		bottomVec.y = angle.AngleY.x + angle.AngleY.y + angle.AngleY.z;
		bottomVec.z = angle.AngleZ.x + angle.AngleZ.y + angle.AngleZ.z;

		bottomVec = MulFloat(bottomVec, m_CenterLength);

		m_EndPos.x = bottomVec.x + m_Pos.x;
		m_EndPos.y = bottomVec.y + m_Pos.y;
		m_EndPos.z = bottomVec.z + m_Pos.z;
	}
	m_Capsule = { m_StartPos, m_EndPos, m_Scale.x };

}

float CapsuleColiderComponent::SegmentDistance(const XMFLOAT3 vec1start, const XMFLOAT3 vec1end, const XMFLOAT3 vec2start, const XMFLOAT3 vec2end, float& t1, float& t2)
{
	XMFLOAT3 dir1 = XMFLOAT3(vec1end.x - vec1start.x, vec1end.y - vec1start.y, vec1end.z - vec1start.z);
	XMFLOAT3 dir2 = XMFLOAT3(vec2end.x - vec2start.x, vec2end.y - vec2start.y, vec2end.z - vec2start.z);
	XMFLOAT3 r = XMFLOAT3(vec1start.x - vec2start.x, vec1start.y - vec2start.y, vec1start.z - vec2start.z);

	float dot11 = Dot(dir1, dir1);	//線分1の長さの2乗
	float dot22 = Dot(dir2, dir2);	//線分2の長さの2乗
	float dot2r = Dot(dir2, r);

	float s = 0.0f, t = 0.0f;

	if (dot11 > 1e-6f && dot22 > 1e-6f)	//値が小さすぎない場合
	{
		float dot12 = Dot(dir1, dir2);
		float dot1r = Dot(dir1, r);
		float denom = dot11 * dot22 - dot12 * dot12;

		if (std::abs(denom) > 1e-6f)	//線分同士が平行じゃない場合
		{
			s = (dot12 * dot2r - dot1r * dot22) / denom;
			t = (dot11 * dot2r - dot12 * dot1r) / denom;
		}

		s = std::clamp(s, 0.0f, 1.0f);
		t = std::clamp(t, 0.0f, 1.0f);
	}

	t1 = s;
	t2 = t;

	XMFLOAT3 closestPosint1 = XMFLOAT3(dir1.x * s, dir1.y * s, dir1.z * s);
	closestPosint1 = XMFLOAT3(vec1start.x + closestPosint1.x, vec1start.y + closestPosint1.y, vec1start.z + closestPosint1.z);

	XMFLOAT3 closestPosint2 = XMFLOAT3(dir2.x * t, dir2.y * t, dir2.z * t);
	closestPosint2 = XMFLOAT3(vec2start.x + closestPosint2.x, vec2start.y + closestPosint2.y, vec2start.z + closestPosint2.z);

	XMFLOAT3 direction = XMFLOAT3(closestPosint1.x - closestPosint2.x, closestPosint1.y - closestPosint2.y, closestPosint1.z - closestPosint2.z);

	float length = Length(direction);

	return LengthSquared(direction);

}

bool CapsuleColiderComponent::CapsuleCollision(const CapsuleColiderComponent* colider1, const CapsuleColiderComponent* colider2)
{
	Capsule capsule1 = colider1->GetCapsule();
	Capsule capsule2 = colider2->GetCapsule();

	float t1, t2;
	float distanceSquared = SegmentDistance(capsule1.startPos, capsule1.endPos, capsule2.startPos, capsule2.endPos, t1, t2);

	float radiusSum = capsule1.radius + capsule2.radius;

	if (distanceSquared <= radiusSum * radiusSum)
	{
		if (colider1->m_GameObject->GetObjectType() == OBJ_TYPE::ENEMY && colider2->m_GameObject->GetObjectType() == OBJ_TYPE::PREDATION) {
			return true;
		}
		if (colider1->m_GameObject->GetObjectType() == OBJ_TYPE::SLASH || colider2->m_GameObject->GetObjectType() == OBJ_TYPE::SLASH) {
			return true;
		}

		XMFLOAT3 capsule1Len = XMFLOAT3(capsule1.endPos.x - capsule1.startPos.x, capsule1.endPos.y - capsule1.startPos.y, capsule1.endPos.z - capsule1.startPos.z);
		XMFLOAT3 capsule2Len = XMFLOAT3(capsule2.endPos.x - capsule2.startPos.x, capsule2.endPos.y - capsule2.startPos.y, capsule2.endPos.z - capsule2.startPos.z);

		capsule1Len = XMFLOAT3(capsule1Len.x * t1, capsule1Len.y * t1, capsule1Len.z * t1);
		capsule2Len = XMFLOAT3(capsule2Len.x * t2, capsule2Len.y * t2, capsule2Len.z * t2);

		//衝突時の最近点と法線計算
		XMFLOAT3 point1 = XMFLOAT3(capsule1.startPos.x + capsule1Len.x, capsule1.startPos.y + capsule1Len.y, capsule1.startPos.z + capsule1Len.z);
		XMFLOAT3 point2 = XMFLOAT3(capsule2.startPos.x + capsule2Len.x, capsule2.startPos.y + capsule2Len.y, capsule2.startPos.z + capsule2Len.z);
		
		XMFLOAT3 pointLen = XMFLOAT3(point1.x - point2.x, point1.y - point2.y, point1.z - point2.z);

		XMFLOAT3 normal = Normalize(pointLen);

		//埋まっている深さを計算
		float depth = radiusSum - sqrt(distanceSquared);

		XMFLOAT3 object1Pos = colider1->GetPos();
		XMFLOAT3 object2Pos = colider2->GetPos();

		XMFLOAT3 direction = XMFLOAT3(object1Pos.x - object2Pos.x, object1Pos.y - object2Pos.y, object1Pos.z - object2Pos.z);

		direction = Normalize(direction);

		//修正ベクトル
		XMFLOAT3 correction = MulFloat(direction, depth);

		//位置の修正
		m_Pos = Add(m_Pos, correction);
		XMFLOAT3 pos = m_GameObject->GetComponent<Transform>()->GetPos();
		m_GameObject->GetComponent<Transform>()->SetPos(Add(pos, correction));

		return true;
		
	}

	return false;
}

//OBBの当たり判定
bool CapsuleColiderComponent::BoxCollision(Capsule& capsule,  GameObject* object)
{

	const float min = 0.00001f;	//無視する値
	const int maxCount = 5;	//最大処理回数

	Box box = object->GetComponent<BoxColiderComponent>()->GetOBB();
	Angle angle;

	int count = 0;
	while (count < maxCount)
	{
		// カプセルのローカル座標変換
		XMFLOAT3 localStart = LocalSpace(capsule.startPos, box);
		XMFLOAT3 localEnd = LocalSpace(capsule.endPos, box);

		// ボックスの範囲内にクランプ
		XMFLOAT3 clampedStart = clampToBox(localStart, box.size);
		XMFLOAT3 clampedEnd = clampToBox(localEnd, box.size);

		//衝突判定(始点または終点が埋まっている場合)
		float distStartSquared = Dot(Sub(localStart, clampedStart), Sub(localStart, clampedStart));
		float distEndSquared = Dot(Sub(localEnd, clampedEnd), Sub(localEnd, clampedEnd));

		//衝突判定
		if (distStartSquared > capsule.radius * capsule.radius && distEndSquared > capsule.radius * capsule.radius) {
			return count > 0;	//一度でも当たったらtrueを返す
		}

		//一番深いポジション
		XMFLOAT3 DeepPos{};

		//埋まった深さの線分の始点と終点
		XMFLOAT3 penetrationStart{};
		XMFLOAT3 penetrationEnd{};

		if (distStartSquared > distEndSquared)
		{
			DeepPos = clampedEnd;			//最も深く埋まったボックス側の点
			penetrationStart = localEnd;	//カプセル側の点(終点)
			penetrationEnd = clampedEnd;	//ボックス側の点
		}
		else
		{
			DeepPos = clampedStart;			//最も深く埋まったボックス側の点
			penetrationStart = localStart;	//カプセル側の点(始点)
			penetrationEnd = clampedStart;	//ボックス側の点
		}

		XMFLOAT3 penetrationVector = Sub(penetrationStart, penetrationEnd);

		//埋まってる深さの計算
		float penetrationDepth = capsule.radius - Length(penetrationVector);
		if (penetrationDepth <= min) {
			break;		//埋まっていない場合終了
		}

		//カプセルの法線
		XMFLOAT3 localNormal = Normalize(penetrationVector);
		//当たったボックスの法線
		XMFLOAT3 worldNormal = Sub(WorldSpace(localNormal, box), box.center);

		XMFLOAT3 correction = MulFloat(Normalize(worldNormal), penetrationDepth);

		DeepPos = WorldSpace(DeepPos, box);
		penetrationStart = WorldSpace(penetrationStart, box);
		penetrationEnd = WorldSpace(penetrationEnd, box);

		XMFLOAT3 t = XMFLOAT3(0.0f, 1.0f, 0.0f);

		float dotProdact = Dot(t, worldNormal);
		float len = Length(t) * Length(worldNormal);

		float cosTheta = dotProdact / len;

		cosTheta = std::fmax(-1.0f, std::fmin(1.0f, cosTheta));

		//二つのベクトルのなす角
		float radian = std::acos(cosTheta);

		XMVECTOR deepVector = { DeepPos.x, DeepPos.y, DeepPos.z,0.0f };
		XMVECTOR clampedEndVector = { clampedEnd.x, clampedEnd.y ,clampedEnd.z ,0.0f };

		XMFLOAT3 boxScale = object->GetComponent<BoxColiderComponent>()->GetScale();

		//法線と真上のベクトルのなす角が４５度以下なら
		if (radian <= 50.0f * (XM_PI / 180.0f) && penetrationStart.y <= m_EndPos.y + m_Scale.x)
		{
			float a = 90.0f;
			float b = 90.0f - (radian / (XM_PI / 180.0f));
			float c = 180.0f - (a + b);

			//半径
			float radius = (penetrationDepth / sinf(a)) / 2;

			float oppositeSideA = (2 * radius) * sinf(a);//aの角の対辺
			float oppositeSideB = (2 * radius) * sinf(b);//bの角の対辺
			float oppositeSideC = (2 * radius) * sinf(c);//今わかっている長さ

			float baiA = sqrt((oppositeSideB * oppositeSideB) + (oppositeSideC * oppositeSideC));

			float ratioA = oppositeSideA / oppositeSideC;

			float reA = ratioA * penetrationDepth;

			//進んでいる方向の反対のベクトル
			XMFLOAT3 reverseVel = XMFLOAT3(0.0f, worldNormal.y, 0.0f);
			XMFLOAT3 reverseVec = Normalize(reverseVel);

			reverseVec = XMFLOAT3(reverseVec.x * baiA, reverseVec.y * baiA, reverseVec.z * baiA);

			capsule.startPos = Add(capsule.startPos, reverseVec);
			capsule.endPos = Add(capsule.endPos, reverseVec);
			m_Pos = Add(m_Pos, reverseVec);
			XMFLOAT3 objectPos = m_GameObject->GetComponent<Transform>()->GetPos();
			m_GameObject->GetComponent<Transform>()->SetPos(Add(objectPos, reverseVec));

			m_GameObject->SetGuroundHeiht(capsule.endPos.y);
			m_GameObject->SetIsGravity(false);
			m_GameObject->SetGravityScale(0.0f);
		}
		else
		{
			float a = 90.0f;
			float b = 90.0f - (radian / (XM_PI / 180.0f));
			float c = 180.0f - (a + b);

			float radius = (penetrationDepth / sinf(a)) / 2;

			float oppositeSideA = (2 * radius) * sinf(a);
			float oppositeSideB = (2 * radius) * sinf(b);
			float oppositeSideC = (2 * radius) * sinf(c);

			float baiA = sqrtf(oppositeSideB * oppositeSideB + oppositeSideC * oppositeSideC);

			XMFLOAT3 reverseVel = XMFLOAT3(worldNormal.x, 0.0f, worldNormal.z);
			XMFLOAT3 reverseVec = Normalize(reverseVel);

			reverseVec = XMFLOAT3(reverseVec.x * baiA, reverseVec.y * baiA, reverseVec.z * baiA);

			capsule.startPos = Add(capsule.startPos, reverseVec);
			capsule.endPos = Add(capsule.endPos, reverseVec);
			m_Pos = Add(m_Pos, reverseVec);
			XMFLOAT3 objectPos = m_GameObject->GetComponent<Transform>()->GetPos();
			m_GameObject->GetComponent<Transform>()->SetPos(Add(objectPos, reverseVec));
		}
		count++;

	}
}

bool CapsuleColiderComponent::SphereCollision(Capsule& capsule, GameObject* object)
{
	return false;
}

std::tuple<bool, GameObject*, std::list<GameObject*>> CapsuleColiderComponent::GetCollision()
{
	int objSize = 0;
	std::list<GameObject*> objectList;
	std::tuple<bool, GameObject*, std::list<GameObject*>> OnCollisionObject;

	XMFLOAT3 position;
	XMFLOAT3 scale;

	for (auto object : Scene::GetInstance()->GetScene<Game>()->GetGameObjectList())
	{
		if (object->GetComponent<Colider>() == nullptr) { continue; }
		if (GetGameObject() == object) { continue; }

		position = object->GetComponent<Colider>()->GetPos();
		scale = object->GetComponent<Colider>()->GetScale();

		//カプセルコライダーの場合
		if (object->GetColider()->GetColiderType() == CAPSULE_COLIDER)
		{
			CapsuleColiderComponent* otherCapsule = object->GetComponent<CapsuleColiderComponent>();

			if (CapsuleCollision(this, otherCapsule))
			{
				objectList.push_back(object);
				objSize = objectList.size();
			}
		}

		//ボックスコライダーの場合
		if (object->GetColider()->GetColiderType() == BOX_COLIDER)
		{
			if (BoxCollision(m_Capsule, object))
			{
				objectList.push_back(object);
				objSize = objectList.size();
			}
			
		}
	}

	if (objSize != 0)
	{
		auto itr = objectList.begin();
		GameObject* gameObject = (*itr);

		OnCollisionObject = std::make_tuple(true, gameObject, objectList);
		return OnCollisionObject;
	}
	else if (objSize == 0)
	{
		OnCollisionObject = std::make_tuple(false, nullptr, objectList);
		return OnCollisionObject;
	}
}

