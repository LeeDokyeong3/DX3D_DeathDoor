#pragma once

#include "Sound.h"

class CSoundManager
{
	friend class CResourceManager;

private:
	CSoundManager();
	~CSoundManager();

private:
	FMOD::System* m_System;
	FMOD::ChannelGroup* m_MasterGroup;
	std::unordered_map<std::string, FMOD::ChannelGroup*>	m_mapGroup;
	std::unordered_map<std::string, CSharedPtr<CSound>>		m_mapSound;
	std::function<void()> m_ChangeCallBack;

public:
	bool Init();
	void Update();

	bool LoadSound(const std::string& ChannelGroupName, bool Loop, const std::string& Name, const char* FileName,
		const std::string& PathName = SOUND_PATH);
	bool CreateSoundChannelGroup(const std::string& Name);
	bool SetVolume(int Volume);
	bool SetVolume(const std::string& ChannelGroupName, int Volume);
	bool SoundPlay(const std::string& Name);
	bool SoundStop(const std::string& Name);
	bool SoundPause(const std::string& Name);
	bool SoundResume(const std::string& Name);
	class CSound* FindSound(const std::string& Name);
	FMOD::ChannelGroup* FindChannelGroup(const std::string& Name);
	void ReleaseSound(const std::string& Name);

public:
	template <typename T>
	void AddResourceChangeCallBack(T* Obj, void(T::* Func)())
	{
		m_ChangeCallBack = std::bind(Func, Obj);
	}
};

