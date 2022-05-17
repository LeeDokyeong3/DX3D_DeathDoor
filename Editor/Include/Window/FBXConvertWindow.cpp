#include "FBXConvertWindow.h"
#include "../EditorUtil.h"
#include "Engine.h"
#include "PathManager.h"
#include <ShlObj_core.h>

// 출처 : https://pyoungon.tistory.com/25
static int CALLBACK BrowseCallbackProc(HWND hWnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
	switch (uMsg)
	{
	case BFFM_INITIALIZED:      // 폴더 선택 대화상자를 초기화 할 때, 초기 경로 설정
	{
		::SendMessage(hWnd, BFFM_SETSELECTION, TRUE, (LPARAM)lpData);
	}
	break;

	// BROWSEINFO 구조체의 ulFlags 값에 BIF_STATUSTEXT 가 설정된 경우 호출
	// 단, BIF_NEWDIALOGSTYLE 가 설정되어 있을 경우 호출되지 않음
	case BFFM_SELCHANGED:       // 사용자가 폴더를 선택할 경우 대화상자에 선택된 경로 표시
	{
		TCHAR szPath[MAX_PATH] = { 0, };

		::SHGetPathFromIDList((LPCITEMIDLIST)lParam, szPath);
		::SendMessage(hWnd, BFFM_SETSTATUSTEXT, 0, (LPARAM)szPath);
	}
	break;

	// BROWSEINFO 구조체의 ulFlags 값에 BIF_VALIDATE 가 설정된 경우 호출
	// BIF_EDITBOX 와 같이 설정된 경우만 호출됨
	case BFFM_VALIDATEFAILED:   // 에디터 콘트롤에서 폴더 이름을 잘못 입력한 경우 호출
	{
		::MessageBox(hWnd, TEXT("해당 폴더를 찾을 수 없습니다."), TEXT("오류"), MB_ICONERROR | MB_OK);
	}
	break;
	}

	return 0;
}

CFBXConvertWindow::CFBXConvertWindow()	:
	m_ConvertThread(nullptr),
	m_SrcDirText(nullptr),
	m_SetSrcDirButton(nullptr),
	m_ConvertButton(nullptr),
	m_ProgressBar(nullptr),
	m_ConvertLog(nullptr),
	m_SrcDirFullPath{}
{
}

CFBXConvertWindow::~CFBXConvertWindow()
{
	SAFE_DELETE(m_ConvertThread);
}

bool CFBXConvertWindow::Init()
{
	// 변환 스레드
	m_ConvertThread = new CFBXConvertThread;
	m_ConvertThread->Init();
	m_ConvertThread->Start();

	// Widget 생성
	CIMGUIText* text = AddWidget<CIMGUIText>("Text");
	text->SetText("FBX Converter");

	AddWidget<CIMGUISeperator>("sep");

	m_SingleFileModeCheckBox = AddWidget<CIMGUICheckBox>("Single File", 0.f, 0.f);
	m_SingleFileModeCheckBox->AddCheckInfo("Single File");
	
	m_SrcDirText = AddWidget<CIMGUITextInput>("SrcDirText", 300.f);
	AddWidget<CIMGUISameLine>("line");
	m_SetSrcDirButton = AddWidget<CIMGUIButton>("Browse Source", 0.f, 0.f);
	m_ProgressBar = AddWidget<CIMGUIProgressBar>("", 300.f, 0.f);
	AddWidget<CIMGUISameLine>("line");
	m_ConvertButton = AddWidget<CIMGUIButton>("Convert", 0.f, 0.f);

	text = AddWidget<CIMGUIText>("Text");
	text->SetText("Log");
	m_ConvertLog = AddWidget<CIMGUIChild>("Log", 300.f, 500.f);

	// Widget Initial Value
	m_SrcDirText->ReadOnly(true);
	m_SrcDirText->SetHideName(true);
	m_SrcDirText->SetHintText("Set FBX Folder Or File");

	m_ProgressBar->SetPercent(0.f);

	m_ConvertLog->EnableBorder(true);

	// CallBack
	m_SetSrcDirButton->SetClickCallback(this, &CFBXConvertWindow::OnClickSetSrcDirButton);
	m_ConvertButton->SetClickCallback(this, &CFBXConvertWindow::OnClickConvertButton);
	m_SingleFileModeCheckBox->SetCallBackIdx(this, &CFBXConvertWindow::OnSelectSingleFileMode);

	m_ConvertThread->SetLoadingCallBack(this, &CFBXConvertWindow::OnLoading);

	return true;
}

void CFBXConvertWindow::Update(float DeltaTime)
{
	CIMGUIWindow::Update(DeltaTime);
}

void CFBXConvertWindow::OnSelectSingleFileMode(int idx, bool Single)
{
	m_SingleFileMode = Single;
}

void CFBXConvertWindow::OnClickSetSrcDirButton()
{
	if (m_SingleFileMode)
	{
		TCHAR   FilePath[MAX_PATH] = {};

		OPENFILENAME    OpenFile = {};

		OpenFile.lStructSize = sizeof(OPENFILENAME);
		OpenFile.hwndOwner = CEngine::GetInst()->GetWindowHandle();
		OpenFile.lpstrFilter = TEXT("FBX File(.fbx)\0*.fbx\0");
		OpenFile.lpstrFile = FilePath;
		OpenFile.nMaxFile = MAX_PATH;
		OpenFile.lpstrInitialDir = CPathManager::GetInst()->FindPath(MESH_PATH)->Path;

		if (GetOpenFileName(&OpenFile) != 0)
		{
			int Length = WideCharToMultiByte(CP_ACP, 0, FilePath, -1, 0, 0, 0, 0);
			WideCharToMultiByte(CP_ACP, 0, FilePath, -1, m_SrcFileFullPath, Length, 0, 0);

			m_SrcDirText->SetText(m_SrcFileFullPath);
		}
	}
	else
	{
		TCHAR Buf[MAX_PATH] = {};

		BROWSEINFO Bi = {};

		Bi.hwndOwner = CEngine::GetInst()->GetWindowHandle();
		Bi.lpszTitle = TEXT("변환할 FBX파일들이 있는 폴더 선택");
		Bi.ulFlags = BIF_USENEWUI | BIF_RETURNONLYFSDIRS | BIF_STATUSTEXT | BIF_VALIDATE;
		Bi.lpfn = BrowseCallbackProc;
	 //	Bi.lParam = (LPARAM)InitialPath;

		LPITEMIDLIST ItemIDList = ::SHBrowseForFolder(&Bi);

		if (::SHGetPathFromIDList(ItemIDList, Buf))
		{
			int length = WideCharToMultiByte(CP_ACP, 0, Buf, -1, nullptr, 0, nullptr, 0);
			WideCharToMultiByte(CP_ACP, 0, Buf, -1, m_SrcDirFullPath, length, nullptr, 0);
			strcat_s(m_SrcDirFullPath, "\\");

			m_SrcDirText->SetText(m_SrcDirFullPath);
		}
	}
}

void CFBXConvertWindow::OnClickConvertButton()
{
	// 예외처리
	if (m_SingleFileMode)
	{
		if (strlen(m_SrcFileFullPath) == 0)
		{
			return;
		}
	}
	else
	{
		if (strlen(m_SrcDirFullPath) == 0)
		{
			return;
		}
	}

	// 로그창 클리어
	m_ConvertLog->ClearWidget();

	CIMGUIText* StartText = m_ConvertLog->AddWidget<CIMGUIText>("Text");
	StartText->SetText("Convert Start...");

	// 파일 하나만 변환하는 경우
	if (m_SingleFileMode)
	{
		m_ConvertThread->AddWork(m_SrcFileFullPath);
	}
	else
	{
		// 폴더 내의 모든 FBX 파일의 경로를 받아옴
		std::vector<std::string> VecFullPath;
		CEditorUtil::GetAllFileFullPathInDir(m_SrcDirFullPath, VecFullPath, ".fbx");

		size_t Size = VecFullPath.size();

		for (size_t i = 0; i < Size; ++i)
		{
			// 스레드에 수행 요청
			m_ConvertThread->AddWork(VecFullPath[i]);
		}
	}
}

void CFBXConvertWindow::OnLoading(const LoadingMessage& msg)
{
	m_ProgressBar->SetPercent(msg.Percent);
	CIMGUIText* Text = m_ConvertLog->AddWidget<CIMGUIText>("Text");
	std::string LoadingMsg = "Convert Success : " + CEditorUtil::FilterFileName(msg.Message);
	Text->SetText(LoadingMsg.c_str());

	if (msg.Complete)
	{
		Text  = m_ConvertLog->AddWidget<CIMGUIText>("OK");
		Text->SetText("Complete!");
		MessageBox(nullptr, TEXT("변환 완료"), TEXT("완료"), MB_OK);
	}
}

void CFBXConvertWindow::OnLoadFail(const std::string& failedPathName)
{
	m_ProgressBar->SetPercent(0.f);

	std::string ErrorText = failedPathName + " 을 로드하는데 실패";

	char Buf[256] = {};
	strcpy_s(Buf, ErrorText.c_str());
	
	TCHAR BufW[MAX_PATH] = {};
	int Length = MultiByteToWideChar(CP_ACP, 0, Buf, -1, nullptr, 0);
	MultiByteToWideChar(CP_ACP, 0, Buf, -1, BufW, Length);

	MessageBox(nullptr, TEXT("오류"), BufW, MB_OK);

	CIMGUIText* Text  = m_ConvertLog->AddWidget<CIMGUIText>("OK");
	std::string LogMsg = "Failed To Load " + failedPathName;
	Text->SetText(LogMsg.c_str());
}
