#pragma once

#include "GameObject/GameObject.h"

class C3DCameraObject :
    public CGameObject
{
	friend class CScene;

protected:
	C3DCameraObject();
	C3DCameraObject(const C3DCameraObject& obj);
	virtual ~C3DCameraObject();

public:
	bool Init() override;
	void Update(float DeltaTime) override;

private:
	void OnDragMove(float DeltaTime);
	void OnDragRotate(float DeltaTime);
	void OnZoomIn(float DetlaTime);
	void OnZoomOut(float DetlaTime);
public :

private:
	CSharedPtr<class CSceneComponent> m_FocusPoint;
	CSharedPtr<class CCameraComponent> m_Camera;
};

