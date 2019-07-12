#include "Global_Variables.h"
using std::make_shared;

Global_Variables* Global_Variables::m_instance = nullptr;

Global_Variables* Global_Variables::Instance()
{
	if (m_instance == nullptr)
		m_instance = new Global_Variables();

	return m_instance;
}

Global_Variables::Global_Variables()
{
}