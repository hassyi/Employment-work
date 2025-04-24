#include "transform.h"

void Transform::SetRotateAround(XMFLOAT3 target)
{
	if (!m_RotAroundDoOnce)
	{
		float vx = m_Pos.x - target.x;
		float vy = m_Pos.y - target.y;
		float vz = m_Pos.z - target.z;

		float vx2 = vx * vx;
		float vy2 = vy * vy;
		float vz2 = vz * vz;

		//球座標
		m_Length = sqrtf(vx2 + vy2 + vz);
		m_Angle = atanf(vz / vx);
		m_Fai = atanf(sqrtf(vx2 + vz2) / vy);
		m_RotAroundDoOnce = true;
	}
}

void Transform::RotateAround(float rotx, float roty)
{
	m_Angle += rotx;
	m_Fai += roty;
	m_Rot.y -= rotx;
}

XMFLOAT3 Transform::GetRotPos(XMFLOAT3 target)
{
	XMFLOAT3 rotpos = m_Pos;

	if (m_RotAroundDoOnce) 
	{
		rotpos.x = (m_Length * sinf(m_Fai) * cosf(m_Angle)) + target.x;
		rotpos.y = (m_Length * cosf(m_Fai)) + target.y;
		rotpos.z = (m_Length * sinf(m_Fai) * sinf(m_Angle)) + target.z;
	}

	return rotpos;
}
