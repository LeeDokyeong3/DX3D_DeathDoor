
#include "RotateAttackDirectionNode.h"
#include "Component/AnimationMeshComponent.h"
#include "Animation/AnimationSequenceInstance.h"
#include "Component/BehaviorTree.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"


CRotateAttackDirectionNode::CRotateAttackDirectionNode() :
	m_Over180(false)
{
	SetTypeID(typeid(CRotateAttackDirectionNode).hash_code());
}

CRotateAttackDirectionNode::CRotateAttackDirectionNode(const CRotateAttackDirectionNode& Node) :
	CActionNode(Node)
{
}

CRotateAttackDirectionNode::~CRotateAttackDirectionNode()
{
}

NodeResult CRotateAttackDirectionNode::OnStart(float DeltaTime)
{
	m_Object->SetNoInterrupt(false);
	m_CallStart = true;

	// Nav Agent�� PathList�� ����
	CNavAgent* Agent = m_Object->GetNavAgent();

	if (Agent)
		Agent->ClearPathList();

	CNavigation3DManager* Manager = m_Object->GetScene()->GetNavigation3DManager();

	Vector3 TargetPos;
	Vector3 ZAxis = m_Object->GetWorldAxis(AXIS::AXIS_Z);

	// ������ ������ ������ ��ǥ��(+z ������ ����� ��� ����)�����̹Ƿ� z��ȣ�� �ݴ�� ���ش�
	m_CurrentForwardVector = Vector3(-ZAxis.x, 0.f, -ZAxis.z);

	if (Manager->CheckPickingPoint(m_PickingPoint))
	{
		Vector3 CurrentPos = m_Object->GetWorldPos();
		m_DestForwardVector = Vector3(m_PickingPoint.x, 0.f, m_PickingPoint.z) - Vector3(CurrentPos.x, 0.f, CurrentPos.z);
		m_DestForwardVector.Normalize();

		Vector3 CrossVec = Vector3(m_CurrentForwardVector.x, m_CurrentForwardVector.y, -m_CurrentForwardVector.z).Cross(Vector3(m_DestForwardVector.x, m_DestForwardVector.y, -m_DestForwardVector.z));

		// �ݽð� �������� 180���� �Ѵ´�
		if (CrossVec.y < 0)
			m_Over180 = true;
		else
			m_Over180 = false;
	}

	else
	{
		m_DestForwardVector = m_CurrentForwardVector;
	}

	Vector3 CurrentFowardYZero = Vector3(m_CurrentForwardVector.x, 0.f, m_CurrentForwardVector.z);
	Vector3 DestForwardYZero = Vector3(m_DestForwardVector.x, 0.f, m_DestForwardVector.z);

	float Degree = RadianToDegree(acosf(CurrentFowardYZero.Dot(DestForwardYZero)));

	m_DestForwardVector.Normalize();

	// CurretForwardVector���� DestForwardVector�� �ݽð� �������� 180���� �Ѵ´ٸ�
	if (m_Over180)
	{
		m_Object->AddWorldRotationY(Degree);
		m_IsEnd = true;
		return NodeResult::Node_True;

	}

	else
	{
		m_Object->AddWorldRotationY(-Degree);
		m_IsEnd = true;
		return NodeResult::Node_True;
	}

}

NodeResult CRotateAttackDirectionNode::OnUpdate(float DeltaTime)
{
	return NodeResult::Node_True;
}

NodeResult CRotateAttackDirectionNode::OnEnd(float DeltaTime)
{
	m_Owner->SetCurrentNode(nullptr);
	m_IsEnd = false;

	return NodeResult::Node_True;
}

NodeResult CRotateAttackDirectionNode::Invoke(float DeltaTime)
{
	return CActionNode::Invoke(DeltaTime);
}