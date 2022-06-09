#include "Timer.h"
#include "Engine.h"

CTimer::CTimer() :
	m_FPS(0.f),
	m_FPSTime(0.f),
	m_Tick(0)
{
	// 고해상도 타이머의 초당 틱을 얻어온다.
	QueryPerformanceFrequency(&m_Second);

	// 고해상도 타이머의 현재 틱을 얻어온다.
	QueryPerformanceCounter(&m_Time);
}

CTimer::~CTimer()
{
}

float CTimer::Update()
{
	LARGE_INTEGER	Time;
	QueryPerformanceCounter(&Time);

	m_DeltaTime = (Time.QuadPart - m_Time.QuadPart) / (float)m_Second.QuadPart;

	m_Time = Time;


	m_FPSTime += m_DeltaTime;
	++m_Tick;

	if (m_Tick == 60)
	{
		m_FPS = 60 / m_FPSTime;
		m_FPSTime = 0.f;
		m_Tick = 0;

  		char	FPS[26] = {};
  
  		sprintf_s(FPS, "FPS : %.5f", m_FPS);
  		SetWindowTextA(CEngine::GetInst()->GetWindowHandle(),
  			FPS);
	}

	return m_DeltaTime;
}
