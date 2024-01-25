#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace MyEngine
{
	enum class eEasingType
	{
		Linear,
		SineEaseIn,
		SineEaseOut,
		SineEaseInOut
	};

	struct KeyFrame
	{
		eEasingType easeType;
		float time;
		bool isKeyEvent;
	};

	struct PositionKeyFrame : KeyFrame
	{
		glm::vec3 value;
	};

	struct ScaleKeyFrame : KeyFrame
	{
		float value;
	};

	struct RotationKeyFrame : KeyFrame
	{
		glm::quat value;
	};
}
