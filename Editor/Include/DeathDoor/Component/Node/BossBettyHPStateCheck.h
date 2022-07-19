#pragma once
#include "Component\Node\ConditionNode.h"
class CBossBettyHPStateCheck :
    public CConditionNode
{
protected:
    CBossBettyHPStateCheck();
    CBossBettyHPStateCheck(const CBossBettyHPStateCheck& Node);
    virtual ~CBossBettyHPStateCheck();

public:
    virtual NodeResult OnStart(float DeltaTime) override;
    virtual NodeResult OnUpdate(float DeltaTime) override;
    virtual NodeResult OnEnd(float DeltaTime) override;
};
