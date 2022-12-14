
#include "CameraManager.h"

CCameraManager::CCameraManager()
{
}

CCameraManager::~CCameraManager()
{
}

void CCameraManager::ShakeCamera(float Time, float Amount)
{
	m_CurrentCamera->Shake(Time, Amount);
}

void CCameraManager::Start()
{
	if (m_CurrentCamera->GetName() == "DefaultCamera")
		m_CurrentCamera->Start();

	m_UICamera->Start();
}

void CCameraManager::Init()
{
	m_CurrentCamera = new CCameraComponent;

	m_CurrentCamera->Init();

	m_CurrentCamera->SetName("DefaultCamera");
	m_CurrentCamera->m_Scene = m_Scene;
	m_CurrentCamera->SetCameraType(Camera_Type::Camera3D);


	m_UICamera = new CCameraComponent;

	m_UICamera->Init();

	m_UICamera->SetName("UICamera");
	m_UICamera->m_Scene = m_Scene;
	m_UICamera->SetCameraType(Camera_Type::CameraUI);

}

void CCameraManager::Update(float DeltaTime)
{
	if (m_CurrentCamera->GetName() == "DefaultCamera")
		m_CurrentCamera->Update(DeltaTime);

	m_CurrentCamera->ComputeShadowView();
}

void CCameraManager::PostUpdate(float DeltaTime)
{
	if (m_CurrentCamera->GetName() == "DefaultCamera")
		m_CurrentCamera->PostUpdate(DeltaTime);
}
