#pragma once

#include "Engine/Core/types.h"
#include "Engine/ECS/Base.h"
#include "Engine/Core/Shapes.hpp"
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <math.h>
#include <set>

namespace MyEngine
{
	// Contains all triangles index (for mesh colliders) and entities inside its area
	struct GridAABB : public sAABB
	{
		// Entities that dont need to be checked but need to have the position recalculated
		std::set<Entity> vecPassiveEntities;
		// Entities that dont need to be checked but don't need to have the position recalculated
		std::set<Entity> vecStaticEntities;

		// Entities that needs direct checking against all other rigid bodies
		std::set<Entity> vecActiveEntities;

		// Collision type: Mesh collider
		// Index to mesh triangles inside this AABB
		std::set<sTriangle*> vecTriangles;

		size_t Total()
		{
			return vecPassiveEntities.size() +
				   vecStaticEntities.size() + 
				   vecActiveEntities.size() + 
				   vecTriangles.size();
		}
	};
}
