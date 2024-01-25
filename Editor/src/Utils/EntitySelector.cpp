#include "EntitySelector.h"

namespace MyEngine
{
    // Entity 0 should always be the empty config
    Entity EntitySelector::m_selectedEntity = 1;

    EntitySelector::EntitySelector()
    {
    }

    EntitySelector::~EntitySelector()
    {
    }

    Entity EntitySelector::GetSelectedEntity()
    {
        return m_selectedEntity;
    }

    Entity EntitySelector::NextEntity(Scene* pScene)
    {
        // Check if new selected entity exists and has components, else just go to next until finds one
        EntityManager* pEntityManager = pScene->GetEntitymanager();

        if (pEntityManager->Size() == 0)
        {
            m_selectedEntity = 0;
            return m_selectedEntity;
        }

        // WARNING: Having only empty mask entities could cause infinite loop
        do
        {
            m_selectedEntity++;

            if (m_selectedEntity >= MAX_ENTITIES)
            {
                m_selectedEntity = 1;
            }
        } while (pEntityManager->GetMask(m_selectedEntity) == EntityMask());

        return m_selectedEntity;
    }

    Entity EntitySelector::PrevEntity(Scene* pScene)
    {
        // Check if new selected entity exists and has components, else just go to prev until finds one
        EntityManager* pEntityManager = pScene->GetEntitymanager();

        if (pEntityManager->Size() == 0)
        {
            m_selectedEntity = 0;
            return m_selectedEntity;
        }

        // WARNING: Having only empty mask entities could cause infinite loop
        do
        {
            m_selectedEntity--;

            if (m_selectedEntity < 1)
            {
                m_selectedEntity = MAX_ENTITIES;
            }
        } while (pEntityManager->GetMask(m_selectedEntity) == EntityMask());

        return m_selectedEntity;
    }

    void EntitySelector::SetSelectedEntity(Entity entityId)
    {
        m_selectedEntity = entityId;
    }
}
