#include "SceneModeWindow.h"
#include "IMGUIText.h"
#include "IMGUIComboBox.h"
#include "../DeathDoor/DDUtil.h"
#include "Scene/SceneManager.h"
#include "../DeathDoor/Scene/DDSceneMode.h"
#include "../DeathDoor/Scene/DDBossSceneMode.h"

#include "../Widget/DDSceneModeWidget.h"

CSceneModeWindow::CSceneModeWindow()
{
}

CSceneModeWindow::~CSceneModeWindow()
{
}

bool CSceneModeWindow::Init()
{
	CIMGUIText* Text = AddWidget<CIMGUIText>("Text");
	Text->SetText("SceneMode Window");

	m_DDSceneTypeCombo = AddWidget<CIMGUIComboBox>("DeathDoor SceneMode Type", 200.f);

	for (int i = 0; i < (int)DDSceneModeType::Max; ++i)
	{
		m_DDSceneTypeCombo->AddItem(CDDUtil::DDSceneModeTypeToString((DDSceneModeType)i));
		m_DDSceneTypeCombo->SetSelectCallback(this, &CSceneModeWindow::OnSelectSceneType);
	}

	return true;
}

void CSceneModeWindow::OnRefreshSceneModeWidget(class CSceneMode* Mode)
{
	if (!m_SceneModeWidget)
	{
		CreateModeWidget(Mode);
	}

	if (m_SceneModeWidget)
	{
		m_SceneModeWidget->RefreshWidgets();
	}
}

void CSceneModeWindow::OnSelectSceneType(int Idx, const char* Label)
{
	size_t SceneTypeID = CDDUtil::DDSceneModeTypeToTypeID((DDSceneModeType)Idx);

	CScene* CurScene = CSceneManager::GetInst()->GetScene();

	bool Success = CurScene->SetSceneMode(SceneTypeID);

	if (!Success)
	{
		MessageBox(nullptr, TEXT("Scene Mode ���� ����"), TEXT("Error"), MB_OK);
		return;
	}
	
	SAFE_DELETE(m_SceneModeWidget);

	CSceneMode* Mode = CurScene->GetSceneMode();
	CreateModeWidget(Mode);
}

void CSceneModeWindow::CreateModeWidget(class CSceneMode* Mode)
{
	size_t TypeID = Mode->GetTypeID();

	if (TypeID == typeid(CDDSceneMode).hash_code())
	{
		m_SceneModeWidget = AddWidget<CDDSceneModeWidget>("SceneModeWidget", 200.f);
	}
	else if (TypeID == typeid(CDDBossSceneMode).hash_code())
	{
		m_SceneModeWidget = AddWidget<CDDSceneModeWidget>("SceneModeWidget", 200.f);
	}

	if (m_SceneModeWidget)
	{
		m_SceneModeWidget->SetSceneMode((CDDSceneMode*)Mode);
	}
}