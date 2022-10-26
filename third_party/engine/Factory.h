/******************************************************************************/
/*!
\file   Factory.h
\author Hun Yang
\par    email: hun.yang8456@gmail.com
\par    GAM150 demo
\date   2019/03/26

This is dynamic factory, which is templated class.

*****YOU CANNOT MODIFY THIS FILE!!*****
*/
/******************************************************************************/
#pragma once
#include <unordered_map>
//#include "Debug.h"

class Component;
enum class eComponentTypes;

/*
 * This is Dynamic factory
 */
template <  typename EnumType,
	        typename BuilderType,
	        typename ReturnType>
class Factory
{
public:
	~Factory();
	void		AddBuilder(EnumType type, BuilderType* pBuilder);
	void		RemoveBuilder(EnumType type);
	void		ClearBuilders(void);
	ReturnType* Build(EnumType type);
private:
	using BuilderMap = std::unordered_map<EnumType, BuilderType*>;
	using MapItor = typename BuilderMap::iterator;

	BuilderMap m_builderMap;
};

template <typename EnumType, typename BuilderType, typename ReturnType>
Factory<EnumType, BuilderType, ReturnType>::~Factory()
{
	ClearBuilders();
}

/**
 * \brief 
 * Add a builder to Factory.
 * 
 * \param type 
 * \param pBuilder 
 */
template <typename EnumType, typename BuilderType, typename ReturnType>
void Factory<EnumType, BuilderType, ReturnType>::AddBuilder(EnumType type, BuilderType* pBuilder)
{
	std::pair<MapItor, bool> itor = m_builderMap.insert(std::make_pair(type, pBuilder));
	//TODO: DEBUG_ASSERT(!itor.second, "Trying to add builder that already exist!");
}

/**
 * \brief 
 * Remove a builder from the Factory.
 * \param type 
 */
template <typename EnumType, typename BuilderType, typename ReturnType>
void Factory<EnumType, BuilderType, ReturnType>::RemoveBuilder(EnumType type)
{
	MapItor itor = m_builderMap.find(type);
	//TODO: DEBUG_ASSERT(itor == m_builderMap.end(), "Trying to remove builder that does not eixst!");

	delete itor->second;
	itor->second = nullptr;
	m_builderMap.erase(itor);
}

/**
 * \brief 
 * Remove all builders from the Factory.
 */
template <typename EnumType, typename BuilderType, typename ReturnType>
void Factory<EnumType, BuilderType, ReturnType>::ClearBuilders()
{
	for (auto itor : m_builderMap)
	{
		delete itor.second;
		itor.second = nullptr;
	}
	m_builderMap.clear();


}

/**
 * \brief
 * Return a new ReturnType based on the builder
 * 
 * \param type 
 * \return 
 */
template <typename EnumType, typename BuilderType, typename ReturnType>
ReturnType* Factory<EnumType, BuilderType, ReturnType>::Build(EnumType type)
{
	MapItor itor = m_builderMap.find(type);
	//TODO: DEBUG_ASSERT(itor == m_builderMap.end(), "Trying to use a builder that does not exist!");

	return itor->second->Build();
}
