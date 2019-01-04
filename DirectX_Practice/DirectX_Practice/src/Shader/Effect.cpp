#include "Effect.h"

Effect::Effect()
{
}

Effect::Effect(VERTEX_SHADER_ID vsID, PIXEL_SHADER_ID psID) :
	m_VSShader(ShaderManager::GetInstance()->Get(vsID)),
	m_PSShader(ShaderManager::GetInstance()->Get(psID))
{
}

Effect::Effect(VERTEX_SHADER_ID vsID) :
	m_VSShader(ShaderManager::GetInstance()->Get(vsID)),
	m_PSShader(nullptr)
{
}

Effect::Effect(PIXEL_SHADER_ID psID) :
	m_VSShader(nullptr),
	m_PSShader(ShaderManager::GetInstance()->Get(psID))
{
}

Effect::~Effect()
{
}

void Effect::Load(VERTEX_SHADER_ID vsID, PIXEL_SHADER_ID psID)
{
	m_VSShader = ShaderManager::GetInstance()->Get(vsID);
	m_PSShader = ShaderManager::GetInstance()->Get(psID);
}

void Effect::Begin() const
{
	if (m_VSShader != nullptr)
		m_VSShader->Begin();
	if (m_PSShader != nullptr)
		m_PSShader->Begin();
}

void Effect::End() const
{
	if (m_VSShader != nullptr)
		m_VSShader->End();
	if (m_PSShader != nullptr)
		m_PSShader->End();
}
