#include "3DParticleObject.h"
#include "Component/ParticleComponent.h"
#include "Component/CameraComponent.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "Input.h"

C3DParticleObject::C3DParticleObject() :
    m_IsCameraRotate(true),
    m_IsCameraZoom(true)
{
    SetTypeID<C3DParticleObject>();
    m_ExcludeSceneSave = true;
}

C3DParticleObject::C3DParticleObject(const C3DParticleObject& obj)
{
}

C3DParticleObject::~C3DParticleObject()
{
}

bool C3DParticleObject::Init()
{
    if (!CGameObject::Init())
        return false;

    m_ParticleComponent = CreateComponent<CParticleComponent>("Particle Sprite");
    m_ParticleComponent->SetLayerName("ParticleEditorLayer");

    m_ParticleArm = CreateComponent<CArm>("Arm");

    m_ParticleCamera = CreateComponent<CCameraComponent>("Particle Camera");

    // Component �Ʒ� Arm
    m_ParticleComponent->AddChild(m_ParticleArm);

    // Arm �Ʒ� Camera
    m_ParticleArm->AddChild(m_ParticleCamera);

    // Camera Setting
    CSceneManager::GetInst()->GetScene()->GetCameraManager()->SetParticleEditorCamera(m_ParticleCamera);

    m_ParticleCamera->SetInheritRotX(true);
    m_ParticleCamera->SetInheritRotY(true);
    m_ParticleCamera->SetInheritRotZ(true);

    m_ParticleCamera->SetCameraType(Camera_Type::Camera3D);
    m_ParticleCamera->SetViewAngle(27.f);

    m_ParticleArm->SetOffset(0.f, 2.f, 0.f);
    m_ParticleArm->SetRelativeRotation(45.f, 0.f, 0.f);
    m_ParticleArm->SetTargetDistance(90.f);

	return true;
}

void C3DParticleObject::Update(float DeltaTime)
{
    CGameObject::Update(DeltaTime);

    if (CInput::GetInst()->GetWheelDir() && m_IsCameraZoom)
    {
        float Length = m_ParticleArm->GetTargetDistance() +
            CInput::GetInst()->GetWheelDir() * m_CameraZoomSpeed;

        m_ParticleArm->SetTargetDistance(Length);
    }

    if (m_IsCameraRotate)
    {
        m_ParticleArm->AddRelativeRotationY(m_CameraRotateSpeed * DeltaTime);
    }
}
