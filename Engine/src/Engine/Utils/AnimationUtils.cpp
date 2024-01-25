#include "pch.h"

#include "AnimationUtils.h"

#include <glm/gtx/easing.hpp>

namespace MyEngine
{
	float AnimationUtils::GetAnimationProgress(float currTime, float startTime, 
											   float endTime, eEasingType type)
	{
		float percent = (currTime - startTime) / (endTime - startTime);
		float progress = 0.f;

		switch (type)
		{
		case eEasingType::Linear:
			progress = percent;
			break;

		case eEasingType::SineEaseIn:
			progress = glm::sineEaseIn(percent);
			break;

		case eEasingType::SineEaseOut:
			progress = glm::sineEaseOut(percent);
			break;

		case eEasingType::SineEaseInOut:
			progress = glm::sineEaseInOut(percent);
			break;
		}

		return progress;
	}

	float AnimationUtils::GetCurrAnimationValue(float startValue, float endValue, float progress)
	{
		float delta = endValue - startValue;

		return startValue + delta * progress;
	}

	glm::vec3 AnimationUtils::GetCurrAnimationValue(glm::vec3 startValue, glm::vec3 endValue, float progress)
	{
		glm::vec3 delta = endValue - startValue;

		return startValue + delta * progress;
	}

	glm::quat AnimationUtils::GetCurrAnimationValue(glm::quat startValue, glm::quat endValue, float progress)
	{
		return glm::slerp(startValue, endValue, progress);
	}
}
