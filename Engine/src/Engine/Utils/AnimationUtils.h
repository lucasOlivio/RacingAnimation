#pragma once

#include "Engine/Graphics/Animations/AnimationProperties.h"

namespace MyEngine
{
	class AnimationUtils
	{
	public:
		template <typename KeyFrameType>
		static void GetKeyFrames(float currTime, std::vector<KeyFrameType>& vecKeyframes,
								 int& startKeyFrame, int& endKeyFrame)
		{
			if (vecKeyframes.size() == 1)
			{
				startKeyFrame = 0;
				endKeyFrame = 0;
				return;
			}

			// Find start and end key frames based on time position
			endKeyFrame = 0;
			while (endKeyFrame < vecKeyframes.size() && vecKeyframes[endKeyFrame].time <= currTime)
			{
				endKeyFrame++;
			}

			if (endKeyFrame >= vecKeyframes.size())
			{
				// Past last frame, then stays in last frame
				startKeyFrame = (int)vecKeyframes.size() - 1;
				endKeyFrame = startKeyFrame;
				return;
			}

			startKeyFrame = endKeyFrame - 1;
		}

		static float GetAnimationProgress(float currTime, float startTime, 
										  float endTime, eEasingType type = eEasingType::Linear);

		static float GetCurrAnimationValue(float startValue, float endValue, float progress);

		static glm::vec3 GetCurrAnimationValue(glm::vec3 startValue, glm::vec3 endValue, float progress);

		static glm::quat GetCurrAnimationValue(glm::quat startValue, glm::quat endValue, float progress);

		// Calculate and set the animated value based on key frames and current time
		template <typename KeyFrameType, typename ValueType>
		static void InterpolateAndApply(std::vector<KeyFrameType>& vecKeyframes, float currentTime, 
									    int startIndex, int endIndex, ValueType& value)
		{
			if (startIndex == -1)
			{
				// No animation
				return;
			}

			KeyFrameType startKeyFrame = vecKeyframes[startIndex];
			KeyFrameType endKeyFrame = vecKeyframes[endIndex];

            if (startIndex == endIndex)
            {
				value = endKeyFrame.value;
				return;
            }

		    float progress = GetAnimationProgress(currentTime, startKeyFrame.time,
										          endKeyFrame.time, startKeyFrame.easeType);

			value = GetCurrAnimationValue(startKeyFrame.value, endKeyFrame.value, progress);
		}
	};
}
