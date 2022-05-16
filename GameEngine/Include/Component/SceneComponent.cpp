
#include "SceneComponent.h"
#include "../Render/RenderManager.h"
#include "../GameObject/GameObject.h"
#include "../Resource/Shader/Standard2DConstantBuffer.h"
#include "../Scene/SceneManager.h"

CSceneComponent::CSceneComponent()
{
	SetTypeID<CSceneComponent>();
	m_ComponentType = Component_Type::SceneComponent;
	m_Render = false;

	m_Transform = new CTransform;

	m_Transform->m_Owner = this;
	m_Transform->Init();

	m_Parent = nullptr;

	m_LayerName = "Default";
}

CSceneComponent::CSceneComponent(const CSceneComponent& com)	:
	CComponent(com)
{
	*this = com;
	m_Transform = com.m_Transform->Clone();

	m_Transform->m_Parent = nullptr;
	m_Transform->m_vecChild.clear();

	m_Transform->m_Owner = this;

	m_Parent = nullptr;

	m_vecChild.clear();

	size_t	Size = com.m_vecChild.size();

	for (size_t i = 0; i < Size; ++i)
	{
		CSceneComponent* CloneComponent = com.m_vecChild[i]->Clone();

		CloneComponent->m_Parent = this;

		m_vecChild.push_back(CloneComponent);

		CloneComponent->m_Transform->m_Parent = m_Transform;

		m_Transform->m_vecChild.push_back(CloneComponent->m_Transform);
	}
}

CSceneComponent::~CSceneComponent()
{
	SAFE_DELETE(m_Transform);
}

void CSceneComponent::SetSceneComponent(CGameObject* Object)
{
	Object->AddSceneComponent(this);

	size_t	Size = m_vecChild.size();

	for (size_t i = 0; i < Size; ++i)
	{
		m_vecChild[i]->SetSceneComponent(Object);
	}
}

void CSceneComponent::GetAllSceneComponentsName(std::vector<FindComponentName>& vecNames)
{
	FindComponentName	Name;

	Name.Name = m_Name;

	if (m_Parent)
		Name.ParentName = m_Parent->GetName();

	vecNames.push_back(Name);

	size_t	Size = m_vecChild.size();

	for (size_t i = 0; i < Size; ++i)
	{
		m_vecChild[i]->GetAllSceneComponentsName(vecNames);
	}
}

void CSceneComponent::Destroy()
{
	CComponent::Destroy();

	size_t	Size = m_vecChild.size();

	for (size_t i = 0; i < Size; ++i)
	{
		m_vecChild[i]->Destroy();
	}
}

void CSceneComponent::DestroySingle()
{
	CComponent::Destroy();
}

void CSceneComponent::SetScene(CScene* Scene)
{
	CComponent::SetScene(Scene);

	m_Transform->m_Scene = Scene;

	size_t	Size = m_vecChild.size();

	for (size_t i = 0; i < Size; ++i)
	{
		m_vecChild[i]->SetScene(Scene);
	}
}

void CSceneComponent::SetGameObject(CGameObject* Object)
{
	CComponent::SetGameObject(Object);

	m_Transform->m_Object = Object;

	size_t	Size = m_vecChild.size();

	for (size_t i = 0; i < Size; ++i)
	{
		m_vecChild[i]->SetGameObject(Object);
	}
}

void CSceneComponent::AddChild(CSceneComponent* Child)
{
	Child->m_Parent = this;

	m_vecChild.push_back(Child);

	Child->m_Transform->m_Parent = m_Transform;

	m_Transform->m_vecChild.push_back(Child->m_Transform);
}

bool CSceneComponent::DeleteChild(CSceneComponent* Child)
{
	size_t	Size = m_vecChild.size();

	for (size_t i = 0; i < Size; ++i)
	{
		if (m_vecChild[i] == Child)
		{
			auto	iter = m_vecChild.begin() + i;

			m_vecChild.erase(iter);

			auto	iterTr = m_Transform->m_vecChild.begin() + i;

			m_Transform->m_vecChild.erase(iterTr);
			return true;
		}

		if (m_vecChild[i]->DeleteChild(Child))
			return true;
	}

	return false;
}

bool CSceneComponent::DeleteChild(const std::string& Name)
{
	size_t	Size = m_vecChild.size();

	for (size_t i = 0; i < Size; ++i)
	{
		if (m_vecChild[i]->GetName() == Name)
		{
			auto	iter = m_vecChild.begin() + i;

			m_vecChild.erase(iter);

			auto	iterTr = m_Transform->m_vecChild.begin() + i;

			m_Transform->m_vecChild.erase(iterTr);
			return true;
		}

		if (m_vecChild[i]->DeleteChild(Name))
			return true;
	}

	return false;
}

bool CSceneComponent::ReplaceComponent(CSceneComponent* DestNode)
{
	// 1)  Parent Component 정보 교체
	CSceneComponent* srcParent = m_Parent;
	CSceneComponent* destParent = DestNode->m_Parent;
	CTransform* srcTransformParent = m_Transform->m_Parent;
	CTransform* destTransformParent = DestNode->m_Transform->m_Parent;

	m_Parent = destParent;
	DestNode->m_Parent = srcParent;

	m_Transform->m_Parent = destTransformParent;
	DestNode->m_Transform->m_Parent = srcTransformParent;

	if(!srcParent)
	{
		// Parent 가 없다면 지금 노드가 Root Node -> Root Component 로 세팅
		m_Object->SetRootComponent(DestNode);
	}

	// 2) 자식 Component, Transform 정보 교체 
	std::vector<CSceneComponent*> vecSrcChilds = {};
	std::vector<CSceneComponent*> vecDestChilds = {};
	std::vector<CTransform*> vecSrcTransformChilds = {};
	std::vector<CTransform*> vecDestTransformChilds = {};

	// Src Child 들 모아두기
	size_t ChildSize = m_vecChild.size();

	for (size_t i = 0; i < ChildSize; ++i)
	{
		vecSrcChilds.push_back(m_vecChild[i]);
		vecSrcTransformChilds.push_back(m_vecChild[i]->m_Transform);
	}

	// Dest Child 들 모아두기
	ChildSize = DestNode->m_vecChild.size();

	for (size_t i = 0; i < ChildSize; ++i)
	{
		vecDestChilds.push_back(DestNode->m_vecChild[i]);
		vecDestTransformChilds.push_back(DestNode->m_vecChild[i]->m_Transform);
	}

	// DestNode 의 Child 를 현재 Child 로 넣기
	m_vecChild.clear();
	m_Transform->m_vecChild.clear();

	ChildSize = vecDestChilds.size();

	for (size_t i = 0; i < ChildSize; ++i)
	{
		vecDestChilds[i]->m_Parent = this;
		m_vecChild.push_back(vecDestChilds[i]);

		vecDestChilds[i]->m_Transform->m_Parent = m_Transform;
		m_Transform->m_vecChild.push_back(vecDestChilds[i]->m_Transform);
	}

	// SrcNode 의 Child 를 DestNode 의 Child 로 넣기
	DestNode->m_vecChild.clear();
	DestNode->m_Transform->m_vecChild.clear();

	ChildSize = vecSrcChilds.size();

	for (size_t i = 0; i < ChildSize; ++i)
	{
		vecSrcChilds[i]->m_Parent = DestNode;
		DestNode->m_vecChild.push_back(vecSrcChilds[i]);

		vecSrcChilds[i]->m_Transform->m_Parent = DestNode->m_Transform;
		DestNode->m_Transform->m_vecChild.push_back(vecSrcChilds[i]->m_Transform);
	}

	return false;
}

bool CSceneComponent::DeleteComponent()
{
	if (!m_Parent)
	{
		// 루트노드를 지우는 경우, 그냥 모두 Destroy
		Destroy();
		m_Object->SetRootComponent(nullptr);
		return true;
	}

	// 지우게 될 경우 -> 가장 첫번째 자식을 해당 위치로 이동시킨다.
	// 자식이 없을 경우 -> 부모에서 현재 SceneComponent 정보만 삭제하면 된다.
	if (m_vecChild.empty())
	{
		m_Parent->DeleteChild(this);

		DestroySingle();

		return true;
	}

	// 부모 자식 관계 재정의
	CSceneComponent* FirstChild = m_vecChild[0];

	FirstChild->m_Parent = m_Parent;
	FirstChild->m_Transform->m_Parent = m_Parent->m_Transform;

	m_Parent->AddChild(FirstChild);
	m_Parent->m_Transform->m_vecChild.push_back(FirstChild->m_Transform);

	// 기존 자식들을 FirsChild 의 자식들로 추가하기
	size_t ChildrenSize = m_vecChild.size();

	for (size_t i = 0; i < ChildrenSize; ++i)
	{
		if (m_vecChild[i] == FirstChild)
			continue;

		m_vecChild[i]->m_Parent = FirstChild;
		FirstChild->m_vecChild.push_back(m_vecChild[i]);

		m_vecChild[i]->m_Transform->m_Parent = FirstChild->m_Transform;
		FirstChild->m_Transform->m_vecChild.push_back(m_vecChild[i]->m_Transform);
	}

	DestroySingle();

	return true;
}

size_t CSceneComponent::GetChildCounet() const
{
	return m_vecChild.size();
}

CSceneComponent* CSceneComponent::FindComponent(const std::string& Name)
{
	if (m_Name == Name)
		return this;

	size_t	Size = m_vecChild.size();

	for (size_t i = 0; i < Size; ++i)
	{
		CSceneComponent* Find = m_vecChild[i]->FindComponent(Name);

		if (Find)
			return Find;
	}

	return nullptr;
}

void CSceneComponent::ClearParent()
{
	if (m_Parent)
	{
		m_Parent = nullptr;
	}

	/*
	Child->m_Parent = this;

	m_vecChild.push_back(Child);

	Child->m_Transform->m_Parent = m_Transform;

	m_Transform->m_vecChild.push_back(Child->m_Transform);
	*/
}

void CSceneComponent::Start()
{
	CComponent::Start();

	size_t	Size = m_vecChild.size();

	for (size_t i = 0; i < Size; ++i)
	{
		m_vecChild[i]->Start();
	}
}

bool CSceneComponent::Init()
{
	return true;
}

void CSceneComponent::Update(float DeltaTime)
{
	m_Transform->Update(DeltaTime);

	size_t	Size = m_vecChild.size();

	for (size_t i = 0; i < Size; ++i)
	{
		m_vecChild[i]->Update(DeltaTime);
	}
}

void CSceneComponent::PostUpdate(float DeltaTime)
{
	m_Transform->PostUpdate(DeltaTime);

	size_t	Size = m_vecChild.size();

	for (size_t i = 0; i < Size; ++i)
	{
		m_vecChild[i]->PostUpdate(DeltaTime);
	}
}

void CSceneComponent::CheckCollision()
{
	size_t	Size = m_vecChild.size();

	for (size_t i = 0; i < Size; ++i)
	{
		m_vecChild[i]->CheckCollision();
	}
}

void CSceneComponent::PrevRender()
{
	if (m_Render)
		CRenderManager::GetInst()->AddRenderList(this);

	size_t	Size = m_vecChild.size();

	for (size_t i = 0; i < Size; ++i)
	{
		m_vecChild[i]->PrevRender();
	}
}

void CSceneComponent::Render()
{
	m_Transform->SetTransform();

	CRenderManager::GetInst()->GetStandard2DCBuffer()->SetAnimation2DEnable(false);
	CRenderManager::GetInst()->GetStandard2DCBuffer()->UpdateCBuffer();
}

void CSceneComponent::PostRender()
{
}

CSceneComponent* CSceneComponent::Clone()
{
	return new CSceneComponent(*this);
}

void CSceneComponent::Save(FILE* File)
{
	CComponent::Save(File);

	fwrite(&m_Render, sizeof(bool), 1, File);

	int	Length = (int)m_LayerName.length();
	fwrite(&Length, sizeof(int), 1, File);
	fwrite(m_LayerName.c_str(), sizeof(char), Length, File);

	m_Transform->Save(File);

	int	ChildCount = (int)m_vecChild.size();

	fwrite(&ChildCount, sizeof(int), 1, File);

	for (int i = 0; i < ChildCount; ++i)
	{
		size_t	TypeID = m_vecChild[i]->GetTypeID();
		fwrite(&TypeID, sizeof(size_t), 1, File);

		m_vecChild[i]->Save(File);
	}
}

void CSceneComponent::Load(FILE* File)
{
	CComponent::Load(File);

	fread(&m_Render, sizeof(bool), 1, File);

	int	Length = 0;
	char	LayerName[256] = {};
	fread(&Length, sizeof(int), 1, File);
	fread(LayerName, sizeof(char), Length, File);

	m_LayerName = LayerName;

	m_Transform->Load(File);

	int	ChildCount = 0;

	fread(&ChildCount, sizeof(int), 1, File);

	for (int i = 0; i < ChildCount; ++i)
	{
		size_t	TypeID = 0;
		fread(&TypeID, sizeof(size_t), 1, File);

		CComponent* Component = CSceneManager::GetInst()->CallCreateComponent(m_Object, TypeID);

		Component->Load(File);

		m_vecChild.push_back((CSceneComponent*)Component);
	}
}
