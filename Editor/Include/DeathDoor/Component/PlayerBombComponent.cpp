
#include "PlayerBombComponent.h"
#include "ObjectPool.h"
#include "Scene/SceneManager.h"
#include "EngineUtil.h"
#include "Component/StaticMeshComponent.h"
#include "ProjectileComponent.h"

CPlayerBombComponent::CPlayerBombComponent()	:
	m_Bomb(nullptr),
	m_CollisionLifeTime(0.5f),
	m_AccCollisionLifeTime(0.f),
	m_Collision(false),
	m_LiftSpeed(0.f),
	m_ShootFirstTime(true),
	m_CancleAction(false),
	m_IsClearBomb(false)
{
	SetTypeID<CPlayerBombComponent>();

	m_ComponentType = Component_Type::ObjectComponent;
	m_ShootSpeed = 70.f;
}

CPlayerBombComponent::CPlayerBombComponent(const CPlayerBombComponent& com)	:
	CObjectComponent(com)
{
}

CPlayerBombComponent::~CPlayerBombComponent()
{
}

void CPlayerBombComponent::SetBomb(CGameObject* Bomb)
{
	m_Bomb = Bomb;
}

const Vector3& CPlayerBombComponent::GetBombPos() const
{
	return m_Bomb->GetWorldPos();
}

CGameObject* CPlayerBombComponent::GetBomb() const
{
	return m_Bomb;
}

void CPlayerBombComponent::OnBombProjectileDestroy(const Vector3& Pos)
{
	m_Bomb = nullptr;

	while (!m_LiftBombPathQueue.empty())
		m_LiftBombPathQueue.pop();
}

void CPlayerBombComponent::ClearLiftPathQueue()
{
	while (!m_LiftBombPathQueue.empty())
		m_LiftBombPathQueue.pop();
}

void CPlayerBombComponent::Start()
{

}

bool CPlayerBombComponent::Init()
{

	return true;
}

void CPlayerBombComponent::Update(float DeltaTime)
{
	if (!m_Bomb)
		return;

	if (m_Collision)
	{
		m_AccCollisionLifeTime += DeltaTime;
		m_Bomb->SetWorldScale(Vector3(0.2f, 0.2f, 0.2f));

		if (m_AccCollisionLifeTime >= m_CollisionLifeTime)
		{
			ResetInfo();
		}
	}

	else if (!m_LiftBombPathQueue.empty())
	{
		Vector3 CurrentPos = m_Bomb->GetWorldPos();
		Vector3 NextPos = m_LiftBombPathQueue.front();

		if (CurrentPos.Distance(NextPos) < 0.5f)
		{
			m_LiftBombPathQueue.pop();
		}

		Vector3 MoveDir = NextPos - CurrentPos;
		MoveDir.Normalize();

		m_Bomb->AddWorldPos(MoveDir * m_LiftSpeed);

		CStaticMeshComponent* Comp = (CStaticMeshComponent*)m_Bomb->GetRootComponent();

		if (Comp)
		{
			Vector4 Emmisive = Comp->GetMaterial(0)->GetEmissiveColor();
			Comp->GetMaterial(0)->SetEmissiveColor(Emmisive.x, Emmisive.y, Emmisive.z + DeltaTime * 10.f, 1.f);
		}
	}
}

void CPlayerBombComponent::PostUpdate(float DeltaTime)
{

}

void CPlayerBombComponent::PrevRender()
{

}

void CPlayerBombComponent::Render()
{

}

void CPlayerBombComponent::PostRender()
{

}

void CPlayerBombComponent::ResetInfo()
{
	m_AccCollisionLifeTime = 0.f;
	m_Collision = false;

	CProjectileComponent* Proj = m_Bomb->FindObjectComponentFromType<CProjectileComponent>();

	// NoUpdate�� true�� ���ϸ� Reset���� �ƹ��� Bomb Object�� �Ⱥ��̴� ���� ��ġ�ص� �ٽ� Projectile���� Move�� ���̴°��� �Űܳ��� ������ NoUpdate�� True�� ���ְ�
	// LiftBomb �Լ����� �ٽ� NoUpdate�� false�� �ǵ�����
	Proj->SetNoUpdate(true);

	m_Bomb->Destroy();
	m_Bomb = nullptr;

	ClearLiftPathQueue();
}

CPlayerBombComponent* CPlayerBombComponent::Clone()
{
	return new CPlayerBombComponent(*this);
}

void CPlayerBombComponent::LiftBomb()
{
	if (m_Bomb)
	{
		// ����� ������ ��ź�� �浹�ż� ����Ʈ�� ����ǰ� �ִ� �߿� �� ��ź�� ��� �ϴ� ���̹Ƿ� return
		m_Bomb = nullptr;
		return;
	}

	CScene* CurrentScene = CSceneManager::GetInst()->GetScene();

	m_Bomb = CObjectPool::GetInst()->GetProjectile("PlayerBomb", CurrentScene);

	if (!m_Bomb)
		return;

	Vector3 ZLookDir = m_Object->GetWorldAxis(AXIS::AXIS_Z) * -1.f;
	Vector3 YLookDir = m_Object->GetWorldAxis(AXIS::AXIS_Y);
	Vector3 XLookDir = m_Object->GetWorldAxis(AXIS::AXIS_X);

	Vector3 Tmp = m_Object->GetWorldPos() + ZLookDir / 2.f;
	const Vector3& InitBombPos = Vector3(Tmp.x + XLookDir.x / 3.f, Tmp.y, Tmp.z);

	m_Bomb->SetWorldScale(0.02f, 0.02f, 0.02f);
	m_Bomb->SetWorldPos(InitBombPos);
	m_Bomb->FindComponentFromType<CStaticMeshComponent>()->GetMaterial(0)->SetEmissiveColor(0.f, 0.f, 0.f, 0.f);

	// Bazier �� ���� �̵��� �� �ְ� �����Ѵ�.
	const Vector3& D2 = m_Bomb->GetWorldPos() + ZLookDir * 1.1f + YLookDir + 0.5f;
	const Vector3& D3 = m_Bomb->GetWorldPos() + ZLookDir * 0.7f + YLookDir * 1.7f;
	const Vector3& D4 = m_Bomb->GetWorldPos() + YLookDir * 3.6f;

	CEngineUtil::CalculateBazierTargetPoses(InitBombPos, D2, D3, D4, m_LiftBombPathQueue, 100);

	CProjectileComponent* Proj = m_Bomb->FindObjectComponentFromType<CProjectileComponent>();
	Proj->SetNoUpdate(false);
}

void CPlayerBombComponent::ShootBomb(const Vector3& ShootDir)
{
	if (!m_Bomb)
	{
		m_AccCollisionLifeTime = 0.f;
		m_Collision = false;

		return;
	}

	if (!m_LiftBombPathQueue.empty())
	{
		m_Bomb->Reset();
		ResetInfo();

		return;
	}


	CProjectileComponent* Proj = m_Bomb->FindObjectComponentFromType<CProjectileComponent>();

	Vector3 BombPos = m_Bomb->GetWorldPos();

	Proj->ShootByLifeTimeCollision<CPlayerBombComponent>(this, &CPlayerBombComponent::OnCollision, Collision_State::Begin, 
		BombPos, ShootDir, m_ShootSpeed, 2.f);

	Proj->SetDestroy(true);
	Proj->SetEndCallBack<CPlayerBombComponent>(this, &CPlayerBombComponent::OnBombProjectileDestroy);

	if (m_ShootFirstTime)
		m_ShootFirstTime = false;
}

void CPlayerBombComponent::HideBomb()
{
}

void CPlayerBombComponent::OnCollision(const CollisionResult& Result)
{
	m_Collision = true;

	Vector3 DestPos = Result.Dest->GetGameObject()->GetWorldPos();

	m_Bomb->FindObjectComponentFromType<CProjectileComponent>()->SetNoUpdate(true);

	m_Bomb->FindComponentFromType<CStaticMeshComponent>()->GetMaterial(0)->SetEmissiveColor(1.f, 1.f, 1.f, 1.f);

	m_Bomb->SetWorldPos(DestPos);

	m_Object->GetScene()->GetCameraManager()->ShakeCamera(0.4f, 0.7f);


}

bool CPlayerBombComponent::Save(FILE* File)
{
	return CObjectComponent::Save(File);
}

bool CPlayerBombComponent::Load(FILE* File)
{
	return CObjectComponent::Load(File);
}

bool CPlayerBombComponent::SaveOnly(FILE* File)
{
	return false;
}

bool CPlayerBombComponent::LoadOnly(FILE* File)
{
	return false;
}