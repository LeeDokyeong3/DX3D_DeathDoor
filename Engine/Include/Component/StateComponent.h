#pragma once

#include "ObjectComponent.h"
#include "BehaviorTree.h"

class CStateComponent :
    public CObjectComponent
{
	friend class CGameObject;

protected:
	CStateComponent();
	CStateComponent(const CStateComponent& com);
	virtual ~CStateComponent();

public:
	virtual bool Init() override;
	virtual void Start();
	virtual void Update(float DeltaTime) override;
	virtual void PostUpdate(float DeltaTime) override;
	virtual void PrevRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual CStateComponent* Clone() override;

protected:
	std::list<class CState*>	m_StateList;
	class CBehaviorTree*		m_BehaviorTree;
	class CAnimationMeshComponent* m_AnimationMeshComp;
	bool							m_TreeUpdate;

public:
	void SetTreeUpdate(bool Update)
	{
		m_TreeUpdate = Update;
	}

	bool IsTreeUpdate()	const
	{
		return m_TreeUpdate;
	}

public:
	void SetAnimationMeshComponent(class CAnimationMeshComponent* Mesh);
	class CAnimationMeshComponent* GetAnimationMeshComp()   const;

public:
	void AddState(class CState* State);
	void AddState(const std::string& Name);

	bool DeleteState(class CState* State);
	bool DeleteState(const std::string& Name);

	class CState* FindState(const std::string& Name);

public:
	template <typename T>
	T* CreateTreeNode(const std::string& Name)
	{
		T* Node = new T;

		Node->SetName(Name);
		Node->SetObject(m_Object);
		Node->SetOwner(m_BehaviorTree);

		if (!m_BehaviorTree->GetRootNode())
			m_BehaviorTree->SetRoot(Node);

		m_BehaviorTree->AddNode(Node);

		return Node;
	}

	CBehaviorTree* GetBehaviorTree()	const
	{
		return m_BehaviorTree;
	}
};

