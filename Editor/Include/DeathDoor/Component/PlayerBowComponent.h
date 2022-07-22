#pragma once

#include "Component/StaticMeshComponent.h"

class CPlayerBowComponent :
    public CStaticMeshComponent
{
	friend class CGameObject;

protected:
	CPlayerBowComponent();
	CPlayerBowComponent(const CPlayerBowComponent& com);
	virtual ~CPlayerBowComponent();

private:
	class CPlayerDataComponent* m_PlayerData;

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void PrevRender();
	virtual void Render();
	virtual void PostRender();
	virtual CPlayerBowComponent* Clone();

public:
	void ShowBow(const Vector3& ShootDir);
	void ShootArrow(const Vector3& ShootDir);

public:
	virtual bool Save(FILE* File);
	virtual bool Load(FILE* File);
	virtual bool SaveOnly(FILE* File) override;
	virtual bool LoadOnly(FILE* File) override;
};
