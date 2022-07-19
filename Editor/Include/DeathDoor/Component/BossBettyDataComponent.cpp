#include "BossBettyDataComponent.h"

CBossBettyDataComponent::CBossBettyDataComponent() :
	m_ThrowFarAttackEnable(false),
	m_FarAttackType(BossBettyFarAttackType::Spin)
{
}

CBossBettyDataComponent::CBossBettyDataComponent(const CBossBettyDataComponent& com)
{
}

CBossBettyDataComponent::~CBossBettyDataComponent()
{
}

void CBossBettyDataComponent::Start()
{
    // �ٰŸ� ���� �Ÿ� �Ǻ� Square Pos ��ġ ����� 
    //  0: ���� �ϴ�, 1 : ���� ���, 2 : ������ ���, 3 : ������ �ϴ�
    const Vector3& ObjectWorldScale = m_Object->GetRootComponent()->GetWorldScale();

    m_PunchLeftSquarePos[0] = Vector3(ObjectWorldScale.x * -1 * 0.5f, 0.f, ObjectWorldScale.z * 0.5f);
    m_PunchLeftSquarePos[1] = Vector3(ObjectWorldScale.x * -1 * 0.5f, 0.f, ObjectWorldScale.z);
    m_PunchLeftSquarePos[2] = Vector3(0.f, 0.f, ObjectWorldScale.z);
    m_PunchLeftSquarePos[3] = Vector3(0.f, 0.f, ObjectWorldScale.z * 0.5f);

    m_PunchRightSquarePos[0] = Vector3(0.f, 0.f, ObjectWorldScale.z * 0.5f);
    m_PunchRightSquarePos[1] = Vector3(0.f, 0.f, ObjectWorldScale.z);
    m_PunchRightSquarePos[2] = Vector3(ObjectWorldScale.x * 0.5f, 0.f, ObjectWorldScale.z);
    m_PunchRightSquarePos[3] = Vector3(ObjectWorldScale.x * 0.5f, 0.f, ObjectWorldScale.z * 0.5f);

    m_SlashLeftSquarePos[0] = Vector3(ObjectWorldScale.x * -1 * 1.5f, 0.f, ObjectWorldScale.z * -1.f);
    m_SlashLeftSquarePos[1] = Vector3(ObjectWorldScale.x * -1 * 1.5f, 0.f, ObjectWorldScale.z);
    m_SlashLeftSquarePos[2] = Vector3(ObjectWorldScale.x * -1 * 0.5f, 0.f, ObjectWorldScale.z);
    m_SlashLeftSquarePos[3] = Vector3(ObjectWorldScale.x * -1 * 0.5f, 0.f, ObjectWorldScale.z * -1.f);

    m_SlashRightSquarePos[0] = Vector3(ObjectWorldScale.x * 0.5f, 0.f, ObjectWorldScale.z * -1.f);
    m_SlashRightSquarePos[1] = Vector3(ObjectWorldScale.x * 0.5f, 0.f, ObjectWorldScale.z);
    m_SlashRightSquarePos[2] = Vector3(ObjectWorldScale.x * 1 * 1.5f, 0.f, ObjectWorldScale.z);
    m_SlashRightSquarePos[3] = Vector3(ObjectWorldScale.x * -1 * 1.5f, 0.f, ObjectWorldScale.z * -1.f);

    m_TwoSideFrontSquarePos[0] = Vector3(ObjectWorldScale.x * -1 * 1.5f, 0.f, ObjectWorldScale.z);
    m_TwoSideFrontSquarePos[1] = Vector3(ObjectWorldScale.x * -1 * 1.5f, 0.f, ObjectWorldScale.z * 1.5f);
    m_TwoSideFrontSquarePos[2] = Vector3(ObjectWorldScale.x * 1 * 1.5f, 0.f, ObjectWorldScale.z * 1.5f);
    m_TwoSideFrontSquarePos[3] = Vector3(ObjectWorldScale.x * 1 * 1.5f, 0.f, ObjectWorldScale.z);
}

void CBossBettyDataComponent::OnBossBettyGenerateTwoSideCloseAttackEffect()
{
	// ���ʿ� 
	// 1) �浹ü Ȱ��ȭ
	// 2) Particle ����
}

void CBossBettyDataComponent::OnBossBettyGenerateRightCloseAttackEffect()
{
}

void CBossBettyDataComponent::OnBossBettyGenerateLeftCloseAttackEffect()
{
}

void CBossBettyDataComponent::OnBossBettyRoarEffect()
{
	// ������, ���� ���·� ������� ������ Particle ������ 
}