
#include "Node.h"
#include "../BehaviorTree.h"

CNode::CNode()	:
	m_IsEnd(false),
	m_CallStart(false)
{
	SetTypeID(typeid(CNode).hash_code());
}

CNode::CNode(const CNode& Node)
{
}

CNode::~CNode()
{
}

void CNode::SetOwner(CBehaviorTree* Owner)
{
	m_Owner = Owner;
}

CBehaviorTree* CNode::GetOwner()
{
	return m_Owner;
}

bool CNode::Save(FILE* File)
{
	size_t	TypeID = m_TypeID;
	fwrite(&TypeID, sizeof(size_t), 1, File);

	int	Length = (int)m_Name.length();
	fwrite(&Length, sizeof(int), 1, File);
	fwrite(m_Name.c_str(), sizeof(char), Length, File);

	return true;
}


bool CNode::Load(FILE* File)
{
	fread(&m_TypeID, sizeof(size_t), 1, File);

	int Length = 0;

	fread(&Length, sizeof(int), 1, File);

	char	Name[256] = {};

	fread(Name, sizeof(char), Length, File);

	m_Name = Name;


	return true;
}

bool CNode::SaveOnly(FILE* File)
{
	int Length = m_Name.length();

	fwrite(&Length, sizeof(int), 1, File);

	char Name[256] = {};

	strcpy_s(Name, m_Name.c_str());

	fwrite(Name, sizeof(char), Length, File);
	fwrite(&m_TypeID, sizeof(size_t), 1, File);

	fwrite(&m_NodeType, sizeof(Node_Type), 1, File);

	return true;
}

bool CNode::LoadOnly(FILE* File)
{
	int Length = 0;

	fread(&Length, sizeof(int), 1, File);

	char Name[256] = {};

	fread(Name, sizeof(char), Length, File);
	m_Name = Name;

	fread(&m_TypeID, sizeof(size_t), 1, File);

	fread(&m_NodeType, sizeof(Node_Type), 1, File);

	return true;
}
