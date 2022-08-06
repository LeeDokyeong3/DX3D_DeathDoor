#pragma once

#include "MonsterDataComponent.h"

class CPlagueKnightDataComponent :
    public CMonsterDataComponent
{
    friend class CGameObject;

private:
    CPlagueKnightDataComponent();
    CPlagueKnightDataComponent(const CPlagueKnightDataComponent& com);
    virtual ~CPlagueKnightDataComponent();

public:
	virtual void Start() override;
    virtual void Update(float DeltaTime) override;

public:
    void OnShootReadyAnimEnd();
    void OnChargeAnimEnd();
    void OnShoot();
    void OnShootAnimEnd();
    void OnBulletGround(const Vector3& Pos);
    void OnCollideExplode(const CollisionResult& Result);

private:
    class CSceneComponent* m_FirePos;
    class CColliderSphere* m_ExplodeCollider;
    bool m_ColliderOn;
};

