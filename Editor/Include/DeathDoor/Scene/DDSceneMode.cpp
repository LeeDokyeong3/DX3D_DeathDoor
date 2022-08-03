#include "DDSceneMode.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "Component/Component.h"
#include "Component/ColliderBox3D.h"
#include "../Thread/DDSceneLoadingThread.h"
#include "Render/RenderManager.h"
#include "../Component/GameStateComponent.h"

CDDSceneMode::CDDSceneMode()	:
	m_ExitPointCollider(nullptr),
	m_NextSceneLoadComplete(false),
	m_NextSceneLoadingThread(nullptr),
	m_InitialScene(false)
{
	SetTypeID<CDDSceneMode>();
}

CDDSceneMode::~CDDSceneMode()
{
	SAFE_DELETE(m_NextSceneLoadingThread);
}

bool CDDSceneMode::Init()
{
	CSceneMode::Init();

	return true;
}

void CDDSceneMode::Start()
{
	CSceneMode::Start();

	m_ExitPointObj = m_Scene->FindObject(m_ExitPointObjName);

	if (m_ExitPointObj)
	{
		m_ExitPointCollider = m_ExitPointObj->FindComponentFromType<CColliderBox3D>();
	}

	// Initial Scene�� �ƴ� ���, ���� ������ NoDestroy ���·� �÷��̾ �Ѿ���� ������ ã�Ƽ� �־��ش�.
	if (!m_InitialScene)
	{
		m_PlayerObject = m_Scene->FindObjectFromType(Object_Type::Player);
	}

	if (m_PlayerObject)
	{
		if (m_InitialScene)
		{
			m_PlayerObject->SetNoDestroyOnSceneChange(true);
		}

		m_PlayerObject->SetWorldPos(m_EntryPoint);
	}

	// �� ��ȯ �ݶ��̴� �ݹ�
	if (m_ExitPointCollider)
	{
		m_ExitPointCollider->AddCollisionCallback(Collision_State::Begin, this, &CDDSceneMode::OnCollideExitCollider);
	}

	// �ٷ� ���� ���� �̸� �ε��صα� ���� ������
	if (!m_NextSceneLoadingThread)
	{
		m_NextSceneLoadingThread = new CDDSceneLoadingThread;

		if (!m_NextSceneFileName.empty())
		{
			m_NextSceneLoadingThread->Load(m_NextSceneFileName, this);
		}
	}

	// ���� ī��Ʈ üũ
	m_MonsterCount = m_Scene->GetObjectCountByType(Object_Type::Monster);

	// Fade In 
	CRenderManager::GetInst()->SetFadeStartCallBack(this, &CDDSceneMode::OnFadeInStart);
	CRenderManager::GetInst()->StartFadeEffect(FadeEffecType::FADE_IN);
}

bool CDDSceneMode::Save(FILE* File)
{
	CRef::Save(File);

	fwrite(&m_InitialScene, sizeof(bool), 1, File);
	fwrite(&m_EntryPoint, sizeof(Vector3), 1, File);
	int Length = m_NextSceneFileName.length();
	fwrite(&Length, sizeof(int), 1, File);
	fwrite(m_NextSceneFileName.c_str(), sizeof(char), Length, File);

	Length = m_ExitPointObjName.length();
	fwrite(&Length, sizeof(int), 1, File);
	fwrite(m_ExitPointObjName.c_str(), sizeof(char), Length, File);

	return true;
}

bool CDDSceneMode::Load(FILE* File)
{
	CRef::Load(File);

	fread(&m_InitialScene, sizeof(bool), 1, File);
	fread(&m_EntryPoint, sizeof(Vector3), 1, File);

	int Length = 0;
	char NextSceneFileName[MAX_PATH] = {};
	fread(&Length, sizeof(int), 1, File);
	fread(NextSceneFileName, sizeof(char), Length, File);

	char ExitObjName[MAX_PATH] = {};
	fread(&Length, sizeof(int), 1, File);
	fread(ExitObjName, sizeof(char), Length, File);

	m_NextSceneFileName = NextSceneFileName;
	m_ExitPointObjName = ExitObjName;

	return true;
}

void CDDSceneMode::OnClearDungeon()
{
	m_ExitPointObj->Enable(true);
	m_ExitPointCollider->Enable(true);
}

bool CDDSceneMode::SetExitPointObj(CGameObject* Obj)
{
	CColliderBox3D* Collider = Obj->FindComponentFromType<CColliderBox3D>();

	if (!Collider)
	{
		return false;
	}

	m_ExitPointObjName = Obj->GetName();
	m_ExitPointObj = Obj;
	m_ExitPointCollider = Collider;
}

void CDDSceneMode::OnDieMonster()
{
	--m_MonsterCount;

	if (m_MonsterCount == 0)
	{
		OnClearDungeon();
	}
}

void CDDSceneMode::OnFadeOutStart()
{
	// Scene Change
	CSceneManager::GetInst()->ChangeNextScene();

	// �÷��̾� ���� �Ұ��� ���·�
	if (m_PlayerObject)
	{
		CGameStateComponent* PlayerState = m_PlayerObject->FindObjectComponentFromType<CGameStateComponent>();
		PlayerState->SetTreeUpdate(false);
	}
}

void CDDSceneMode::OnFadeInStart()
{
	// �÷��̾� ���� ���� ���·�
	if (m_PlayerObject)
	{
		CGameStateComponent* PlayerState = m_PlayerObject->FindObjectComponentFromType<CGameStateComponent>();
		PlayerState->SetTreeUpdate(true);
	}
}

void CDDSceneMode::OnCollideExitCollider(const CollisionResult& Result)
{
	// �ε� �Ǳ� �������� ���
	while (!m_NextSceneLoadComplete)
	{
		continue;
	}

	// Fade Out
	CRenderManager::GetInst()->SetFadeStartCallBack(this, &CDDSceneMode::OnFadeOutStart);
	CRenderManager::GetInst()->StartFadeEffect(FadeEffecType::FADE_OUT);
}