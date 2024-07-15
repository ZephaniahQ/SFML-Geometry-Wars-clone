#include "EntityManager.h"

EntityManager::EntityManager()
{

}

void EntityManager::update()
{
    // TODO: add entities from m_entitiesToAdd to the proper lcoation(s)
    //          - add them to the vector of all entites
    //          - add them to the vector inside the map, with the tag as a key

    for( auto e : m_entitiesToAdd){
        m_entities.push_back(e);
        m_entityMap[e->tag()].push_back(e);
    }

    //ok this is my implementation of adding entities lets see.
    
    // remove dead entities from the vecotr of all entities

    removeDeadEntities(m_entities);

    // remove dead entiteis from each vector in the entity map
    // C++20 way of iterating through [key, value] pairs in a map

    for (auto& [tag, entityVec] : m_entityMap)
    {
        removeDeadEntities(entityVec);
    }
}

void EntityManager::removeDeadEntities(EntityVec & vec)
{
    // TODO remove all dead entiteis from the input vector
    // this is called by the update() fucntion

    vec.erase(std::remove_if(vec.begin(), vec.end(), [](const std::shared_ptr<Entity> e)
    {
        return !e->isActive();
    }), vec.end());

    //learnt removing elements from vectors the proper way without iterator invalidation, I hope this works

    // note: to do this check if entity is marked dead then remove from vector
    // this is the place where itterator invalidation can happen
}

std::shared_ptr<Entity> EntityManager::addEntity(const std::string & tag)
{
    auto entity = std::shared_ptr<Entity>(new Entity(m_totalEntities++, tag));

    m_entitiesToAdd.push_back(entity);

    return entity;
}

const EntityVec & EntityManager::getEntities()
{
    return m_entities;
}

const EntityVec & EntityManager::getEntities(const std::string & tag)
{
    // TODO this is incorrect, return the correct vecotr from the map;

    //  note: Probably done, testing needed
    return m_entityMap[tag];
}

const EntityMap& EntityManager::getEntityMap()
{
    return m_entityMap;
};