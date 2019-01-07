#include "ShaderManager.h"

ShaderManager::ShaderManager()
{
	Clear();
}

ShaderManager::~ShaderManager()
{
	Clear();
}

void ShaderManager::Add(VERTEX_SHADER_ID id, const VSPtr shader)
{
	m_VSUMap[id] = shader;
}

void ShaderManager::Add(PIXEL_SHADER_ID id, const PSPtr shader)
{
	m_PSUMap[id] = shader;
}

VSPtr ShaderManager::Get(const VERTEX_SHADER_ID id) const
{
	return m_VSUMap.at(id);
}

PSPtr ShaderManager::Get(const PIXEL_SHADER_ID id) const
{
	return m_PSUMap.at(id);
}

void ShaderManager::Clear()
{
	m_VSUMap.clear();
	m_PSUMap.clear();
}
