#pragma once

#include "Component\Node\ConditionNode.h"

class CMeleeRangeCheckNode :
    public CConditionNode
{
    friend class CGameBehaviorTree;

protected:
    CMeleeRangeCheckNode();
    CMeleeRangeCheckNode(const CMeleeRangeCheckNode& Node);
    virtual ~CMeleeRangeCheckNode();

public:
    virtual NodeResult OnStart(float DeltaTime) override;
    virtual NodeResult OnUpdate(float DeltaTime) override;
    virtual NodeResult OnEnd(float DeltaTime) override;
};

