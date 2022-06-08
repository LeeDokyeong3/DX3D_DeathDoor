#pragma once

#include "SceneComponentWidget.h"

class CStaticMeshComponentWidget final :
    public CSceneComponentWidget
{
public:
    CStaticMeshComponentWidget();
    virtual ~CStaticMeshComponentWidget();

public:
    virtual bool Init() override;
    
public:
    virtual void SetSceneComponent(class CSceneComponent* Com) override;

private:
    void OnClickLoadMesh();
    void OnSelectMaterialSlotCombo(int Idx, const char* Label);
    void OnEditBaseColor(const Vector3& Color);
    void OnEditAmbientColor(const Vector3& Color);
    void OnEditSpecluarColor(const Vector3& Color);
    void OnEditSpecluarPower(float Power);
    void OnEditEmissiveColor(const Vector3& Color);
    void OnCheckTransparency(int Idx, bool Check);
    void OnEditOpacity(float Opacity);
    void OnCheckOutlineEnable(int Idx, bool Enable);
    void OnEditOutlineThickness(float Val);
    void OnChangeOutlineColor(const Vector3& Color);

private:
    void RefreshMeshWidget(class CMesh* Mesh);

private:
    class CIMGUITextInput* m_MeshName;
    class CIMGUIButton* m_LoadMeshButton;
    class CIMGUIComboBox* m_MaterialSlotCombo;
    class CIMGUIColor3* m_BaseColorEdit;
    class CIMGUIColor3* m_AmbientColorEdit;
    class CIMGUIColor3* m_SpecularColorEdit;
    class CIMGUIInputFloat* m_SpecluarPowerEdit;
    class CIMGUIColor3* m_EmissiveColorEdit;
    class CIMGUICheckBox* m_TransparencyEdit;
    class CIMGUISliderFloat* m_OpacityEdit;
    class CIMGUICheckBox* m_OutlineEnable;
    class CIMGUISliderFloat* m_OutlineThickness;
    class CIMGUIColor3* m_OutlineColor;
};

