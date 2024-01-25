#include "pch.h"

#include "GridBroadPhaseSystem.h"

#include "Engine/ECS/Components.h"
#include "Engine/ECS/Scene/SceneView.hpp"
#include "Engine/ECS/SingletonComponents/PhysicsLocator.h"
#include "Engine/ECS/SingletonComponents/CoreLocator.h"

#include "Engine/Utils/GridUtils.h"
#include "Engine/Utils/TransformUtils.h"
#include "Engine/Utils/CollisionsUtils.h"

namespace MyEngine
{
	typedef std::set< Entity >::iterator itEntities;
	typedef std::map< uint /*index*/, GridAABB* >::iterator itIdxAABB;
	typedef std::pair< uint /*index*/, GridAABB* > pairIdxAABB;

	void GridBroadPhaseSystem::Init()
	{
	}

	void GridBroadPhaseSystem::Start(Scene* pScene)
	{
		// Creating AABBs grid
		GridBroadphaseComponent* pGrid = PhysicsLocator::GetGridBroadphase();

		for (Entity entityId : SceneView<TransformComponent, RigidBodyComponent>(*pScene))
		{
			TransformComponent* pTransform = pScene->Get<TransformComponent>(entityId);
			RigidBodyComponent* pRigidBody = pScene->Get<RigidBodyComponent>(entityId);

			uint idxpos = GridUtils::LocatePoint(pTransform->worldPosition, pGrid->lengthPerBox);
			m_InsertEntity(entityId, idxpos, pRigidBody->bodyType);

			if (pRigidBody->shapeType == eShape::AABB)
			{
				m_InsertAABB(pScene, entityId, pRigidBody->bodyType);
			}
			else if (pRigidBody->shapeType == eShape::SPHERE)
			{
				SphereColliderComponent* pSphere = pScene->Get<SphereColliderComponent>(entityId);
				m_InsertSphere(entityId, idxpos, pTransform->position,
					pSphere->radius, pRigidBody->bodyType, pGrid);
			}
			else if (pRigidBody->shapeType == eShape::MESH_OF_TRIANGLES)
			{
				m_InsertMesh(pScene, entityId, pRigidBody->bodyType);
			}
			else
			{
				LOG_WARNING("Shape type not implemented yet: " + std::to_string(pRigidBody->shapeType));
			}
		}
	}

	void GridBroadPhaseSystem::Update(Scene* pScene, float deltaTime)
	{
		GridBroadphaseComponent* pGrid = PhysicsLocator::GetGridBroadphase();
		NarrowPhaseTestsComponent* pNarrowTests = PhysicsLocator::GetNarrowPhaseTests();

		// Clear active entities first
		for (itIdxAABB it = pGrid->mapAABBs.begin(); it != pGrid->mapAABBs.end(); ++it)
		{
			int key = it->first;
			GridAABB* pAABB = it->second;

			pAABB->vecActiveEntities.clear();
			pAABB->vecPassiveEntities.clear();
		}

		// Clear all test groups
		pNarrowTests->passiveEntitiesToTest.clear();
		pNarrowTests->activeEntitiesToTest.clear();
		pNarrowTests->trianglesToTest.clear();

		// Update aabbs active and passice entities positions
		for (Entity entityId : SceneView<TransformComponent, RigidBodyComponent>(*pScene))
		{
			TransformComponent* pTransform = pScene->Get<TransformComponent>(entityId);
			RigidBodyComponent* pRigidBody = pScene->Get<RigidBodyComponent>(entityId);

			if (pRigidBody->bodyType == eBody::STATIC)
			{
				continue;
			}

			uint idxpos = GridUtils::LocatePoint(pTransform->worldPosition, pGrid->lengthPerBox);
			m_InsertEntity(entityId, idxpos, pRigidBody->bodyType);

			if (pRigidBody->shapeType == eShape::AABB)
			{
				m_InsertAABB(pScene, entityId, pRigidBody->bodyType);
			}
			else if (pRigidBody->shapeType == eShape::SPHERE)
			{
				SphereColliderComponent* pSphere = pScene->Get<SphereColliderComponent>(entityId);
				m_InsertSphere(entityId, idxpos, pTransform->position,
					pSphere->radius, pRigidBody->bodyType, pGrid);
			}
			else
			{
				LOG_WARNING("Shape type not implemented yet: " + std::to_string(pRigidBody->shapeType));
			}

		}

		// Update testing groups for narrow phase
		int i = -1;
		for (itIdxAABB it = pGrid->mapAABBs.begin(); it != pGrid->mapAABBs.end();)
		{
			GridAABB* pAABB = it->second;

			// Only add to narrow phase testing groups if we have active entity on aabb
			if (pAABB->vecActiveEntities.size() > 0)
			{
				std::vector<Entity> vecStatics = {};
				std::vector<Entity> vecPassive = {};
				std::vector<Entity> vecActive = {};
				std::vector<sTriangle*> vecTriangles = {};

				pNarrowTests->staticEntitiesToTest.push_back(vecStatics);
				pNarrowTests->passiveEntitiesToTest.push_back(vecPassive);
				pNarrowTests->activeEntitiesToTest.push_back(vecActive);
				pNarrowTests->trianglesToTest.push_back(vecTriangles);

				i++;

				for (Entity entityId : pAABB->vecActiveEntities)
				{
					pNarrowTests->activeEntitiesToTest[i].push_back(entityId);
				}

				for (Entity entityId : pAABB->vecStaticEntities)
				{
					pNarrowTests->staticEntitiesToTest[i].push_back(entityId);
				}

				for (Entity entityId : pAABB->vecPassiveEntities)
				{
					pNarrowTests->passiveEntitiesToTest[i].push_back(entityId);
				}

				for (sTriangle* triangle : pAABB->vecTriangles)
				{
					pNarrowTests->trianglesToTest[i].push_back(triangle);
				}
			}

			// Check if aabb is empty to remove from mapping
			if (pAABB->Total() == 0)
			{
				delete pAABB;
				it = pGrid->mapAABBs.erase(it);
			}
			else
			{
				++it;
			}
		}
	}

	void GridBroadPhaseSystem::Render(Scene* pScene)
	{
	}

	void GridBroadPhaseSystem::End(Scene* pScene)
	{
		m_ClearAABBs();
	}

	void GridBroadPhaseSystem::Shutdown()
	{
	}

	GridAABB* GridBroadPhaseSystem::m_GetAABB(uint idxAABB)
	{
		GridBroadphaseComponent* pGrid = PhysicsLocator::GetGridBroadphase();

		itIdxAABB itAABB = pGrid->mapAABBs.find(idxAABB);
		if (itAABB == pGrid->mapAABBs.end())
		{
			return nullptr;
		}

		return itAABB->second;
	}

	GridAABB* GridBroadPhaseSystem::m_GetAABB(const glm::vec3& point)
	{
		GridBroadphaseComponent* pGrid = PhysicsLocator::GetGridBroadphase();

		uint idxAABB = GridUtils::LocatePoint(point, pGrid->lengthPerBox);

		return m_GetAABB(idxAABB);
	}

	GridAABB* GridBroadPhaseSystem::m_GetOrCreateAABB(uint idxAABB)
	{
		GridAABB* pAABB = m_GetAABB(idxAABB);
		GridBroadphaseComponent* pGrid = PhysicsLocator::GetGridBroadphase();

		if (pAABB)
		{
			return pAABB;
		}

		pAABB = new GridAABB();
		pAABB->minXYZ = GridUtils::LocatePosition(idxAABB, pGrid->lengthPerBox);
		pAABB->maxXYZ = pAABB->minXYZ + pGrid->lengthPerBox;

		pGrid->mapAABBs[idxAABB] = pAABB;

		return pAABB;
	}

	void GridBroadPhaseSystem::m_InsertAABB(Scene* pScene, Entity entityId, eBody bodyType)
	{
		LOG_WARNING("AABB GRID INSERT NOT IMPLEMENTED YET!");
		AABBColliderComponent* pAABB = pScene->Get<AABBColliderComponent>(entityId);
		if (!pAABB)
		{
			return;
		}
	}

	void GridBroadPhaseSystem::m_InsertSphere(Entity entityID, uint originIndex,
											  const glm::vec3& position, float radius, 
											  const eBody& bodyType,
											  GridBroadphaseComponent* pGrid)
	{
		// Check collisions in the neighboring cells
		for (int i = -1; i <= 1; ++i)
		{
			float radiusI = radius * i;
			for (int j = -1; j <= 1; ++j)
			{
				float radiusJ = radius * j;
				for (int k = -1; k <= 1; k++)
				{
					if (j == 0 && i == 0 && k == 0)
					{
						// Same aabb
						continue;
					}

					float radiusK = radius * k;

					glm::vec3 currRadius = glm::vec3(radiusI, radiusJ, radiusK);
					glm::vec3 currPoint = position + currRadius;

					uint currIdxpos = GridUtils::LocatePoint(currPoint, pGrid->lengthPerBox);

					if (currIdxpos == originIndex)
					{
						// Same aabb
						continue;
					}

					m_InsertEntity(entityID, currIdxpos, bodyType);
				}
			}
		}
	}

	void GridBroadPhaseSystem::m_InsertMesh(Scene* pScene, Entity entityId, const eBody& bodyType)
	{
		MeshColliderComponent* pMeshCollider = pScene->Get<MeshColliderComponent>(entityId);
		sMesh* pMesh = pMeshCollider->pMesh;
		if (!pMeshCollider || !pMeshCollider->pMesh)
		{
			return;
		}

		// for every triangle in the mesh insert into aabb
		for (uint i = 0; i < pMesh->numberOfTriangles; i++)
		{
			m_InsertMeshTriangle(entityId, &(pMesh->pTriangles[i]));
		}
	}

	void GridBroadPhaseSystem::m_InsertEntity(Entity entityID, uint index, const eBody& bodyType)
	{
		GridAABB* pAABB = m_GetOrCreateAABB(index);
		GridBroadphaseComponent* pGrid = PhysicsLocator::GetGridBroadphase();
		NarrowPhaseTestsComponent* pNarrowTests = PhysicsLocator::GetNarrowPhaseTests();

		if (bodyType == eBody::STATIC)
		{
			pAABB->vecStaticEntities.insert(entityID);
		}
		else if (bodyType == eBody::PASSIVE)
		{
			pAABB->vecPassiveEntities.insert(entityID);
		}
		else
		{
			pAABB->vecActiveEntities.insert(entityID);
		}

		return;
	}

	void GridBroadPhaseSystem::m_InsertMeshTriangle(Entity entityId, sTriangle* pTriangle)
	{
		GridBroadphaseComponent* pGrid = PhysicsLocator::GetGridBroadphase();
		pTriangle->calcNormal();

		// Locate each vertex
		uint idxV1 = GridUtils::LocatePoint(pTriangle->vertices[0], pGrid->lengthPerBox);
		uint idxV2 = GridUtils::LocatePoint(pTriangle->vertices[0], pGrid->lengthPerBox);
		uint idxV3 = GridUtils::LocatePoint(pTriangle->vertices[0], pGrid->lengthPerBox);

		// TODO: When a pTriangle edge passes 3 or more aabbs it should be in those aabbs too
		// Insert pTriangle into all AABBs that it intersects
		GridAABB* pAABB = m_GetOrCreateAABB(idxV1);
		pAABB->vecTriangles.insert(pTriangle);

		if (idxV2 != idxV1)
		{
			GridAABB* pAABB2 = m_GetOrCreateAABB(idxV2);
			pAABB->vecTriangles.insert(pTriangle);
		}

		if (idxV3 != idxV1 && idxV3 != idxV2)
		{
			GridAABB* pAABB3 = m_GetOrCreateAABB(idxV3);
			pAABB->vecTriangles.insert(pTriangle);
		}

		return;
	}

	void GridBroadPhaseSystem::m_ClearAABBs()
	{
		GridBroadphaseComponent* pGrid = PhysicsLocator::GetGridBroadphase();

		for (pairIdxAABB pairAABB : pGrid->mapAABBs)
		{
			delete pairAABB.second;
		}

		pGrid->mapAABBs.clear();
	}

	size_t GridBroadPhaseSystem::m_RemoveAABB(uint idxAABB)
	{
		GridAABB* pAABB = m_GetAABB(idxAABB);
		GridBroadphaseComponent* pGrid = PhysicsLocator::GetGridBroadphase();

		if (!pAABB)
		{
			return 0;
		}

		size_t left = pGrid->mapAABBs.erase(idxAABB);
		delete pAABB;

		return left;
	}
}
